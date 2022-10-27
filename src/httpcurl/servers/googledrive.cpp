#include "googledrive.h"

GoogleDrive::GoogleDrive(){
}

GoogleDrive::~GoogleDrive(){
}

uint32_t GoogleDrive::authenticate(){
    string strAccessToken;
    string strRefreshToken;
    string display_name, email;
    uint32_t retorno = SINERROR;

    try{
        Traza::print("GoogleDrive::authenticate. rutaini: " + rutaIni, W_DEBUG);
        if (decodeTokens(strAccessToken, strRefreshToken, GOOGLEACCESSTOKENSTR, GOOGLEREFRESHTOKENSTR) == SINERROR){
            string url = "https://www.googleapis.com/drive/v3/about?fields=user";
            string AuthOauth2 = "Bearer " + strAccessToken;
            map<string, string> cabeceras;

            cabeceras.clear();
            cabeceras.insert( make_pair("Authorization", AuthOauth2));
            cabeceras.insert( make_pair("Accept", "*/*"));
            cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
            cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
            cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));

            MemoryStruct *chunk = util.initDownload();
            util.httpGet(url, &cabeceras, chunk);
            string res = util.getData(chunk);
            util.endDownload(&chunk);

            //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
            Json::Value root;   // will contains the root value after parsing.
            if (!res.empty()){
                retorno = checkOauthErrors(res, &root);
            }

            if(retorno == SINERROR){
                Json::Value user = root["user"];
                display_name = user.get("displayName","").asString();
                email = user.get("emailAddress","").asString();
                Traza::print("Usuario " + display_name + " con correo " + email + " autenticado con exito.", W_DEBUG);
            } else {
                Traza::print("GoogleDrive::authenticate. Se ha producido un error en el proceso de autorizacion", W_ERROR);
            }
        } else {
            Traza::print("GoogleDrive::authenticate. No se ha encontrado el fichero de configuracion para oauth", W_ERROR);
            retorno = ERRORACCESSTOKEN;
        }

    } catch (Excepcion &e){
        Traza::print("GoogleDrive::authenticate. Error al cargar la configuracion", W_ERROR);
        retorno = ERRORACCESSTOKEN;
    }

    this->setAccessToken(strAccessToken);
    this->setRefreshToken(strRefreshToken);
    setOauthStatus(retorno);
    return retorno;
}

/**
*
*/
uint32_t GoogleDrive::checkOauthErrors(string data, Json::Value *root){
    uint32_t retorno = SINERROR;
    //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
    if (!data.empty()){
        string err;
        bool parsingSuccessful = JsonParser::parseJson(root, data, &err);
        string errorText;

        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            //Traza::print("GoogleDrive::checkOauthErrors: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
            Traza::print("GoogleDrive::checkOauthErrors: Failed to parse configuration." + err, W_ERROR);
            retorno = ERRORACCESSTOKEN;
        } else {
            Json::Value error = (*root)["error"];
            errorText = error.get("message","").asString();
            string code = error.get("code","").asString();
            if (code.compare("401") == 0){
                retorno = ERRORREFRESHTOKEN;
            } else if (!code.empty()){
                retorno = ERRORACCESSTOKEN;
            }
        }

        //Comprobamos que el access token funciona
        //Si hemos detectado error lanzamos el proceso de autorizacion
        if (retorno != SINERROR){
            Traza::print("GoogleDrive::checkOauthErrors: " + data, W_ERROR);
            if (errorText.find("OAuth 2 \"Authorization\" header is not well-formed") != string::npos
                || errorText.find("Invalid OAuth2 token") != string::npos)
            {
                retorno = ERRORACCESSTOKEN;
            }
        }
    } else {
        Traza::print("GoogleDrive::checkOauthErrors: Empty data", W_DEBUG);
    }

    return retorno;
}

string GoogleDrive::storeAccessToken(string codeOrRefreshToken, bool refresh){
    return storeToken(GOOGLEACCESSTOKENSTR, GOOGLEREFRESHTOKENSTR, codeOrRefreshToken, refresh);
}


/**
*
*/
//string GoogleDrive::storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
//    Traza::print("GoogleDrive::storeAccessToken. Negociando access token...", W_DEBUG);
//    filecipher cifrador;
//    launchAccessToken(clientid, secret, codeOrRefreshToken, refresh);
//
//    if (!this->getAccessToken().empty()){
//        string accessTokenCipherB64 = cifrador.encodeEasy(this->getAccessToken(), passwordAT);
//        string refreshTokenCipherB64 = cifrador.encodeEasy(this->getRefreshToken(), passwordAT);
//
//        ListaIni<Data> *config = new ListaIni<Data>();
//        try{
//            Dirutil dir;
//            if (dir.existe(rutaIni)){
//                config->loadFromFile(rutaIni);
//                config->sort();
//            }
//            this->addToken(GOOGLEACCESSTOKENSTR, accessTokenCipherB64, config);
//            this->addToken(GOOGLEREFRESHTOKENSTR, refreshTokenCipherB64, config);
//            config->writeToFile(rutaIni);
//
//        } catch (Excepcion &e){
//            Traza::print("GoogleDrive::storeAccessToken. Error al cargar la configuracion", W_ERROR);
//        }
//    }
//    return this->getAccessToken();
//}

/**
* lanza la autorizacion desde el explorador para obtener un code
*/
void GoogleDrive::launchAuthorize(string clientid){
    //https://developers.google.com/identity/protocols/OAuth2InstalledApp#choosingredirecturi
    string tmpUrl = "\"" + GOOGLEURLAUTH
                    + "?response_type=code"
                    + "&client_id=" + Constant::url_encode(clientid)
                    + "&redirect_uri=" + Constant::url_encode("urn:ietf:wg:oauth:2.0:oob")
                    + "&scope=" + Constant::url_encode("https://www.googleapis.com/auth/drive.file")
                    + "\"";

    //string cmd = CMD_LAUNCH_BROWSER + " \"" + tmpUrl + "\"";
    //system(cmd.c_str());
    openExplorer(tmpUrl);
}

/**
* Obtiene el access token para hacer subsiguientes peticiones. Si tenemos el refresh_token,
* tambien lo podemos utilizar para renovar el access_token sin necesidad de obtener el code
*/
string GoogleDrive::launchAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
    string url = GOOGLEURLTOKEN;
    map<string, string> cabeceras;
    string postData;

    cabeceras.clear();
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));

    postData= "client_id=" + clientid
            + "&client_secret=" + secret;

    if (refresh){
        postData += "&refresh_token=" + codeOrRefreshToken + "&grant_type=refresh_token";
    } else {
        postData += "&code=" + codeOrRefreshToken + "&grant_type=authorization_code"
            + "&redirect_uri=" + Constant::url_encode("urn:ietf:wg:oauth:2.0:oob");

    }

    Traza::print(postData, W_DEBUG);

    MemoryStruct *chunk = util.initDownload();
    util.httpPost(url, postData.c_str(),postData.length(), &cabeceras, chunk);
    string str = util.getData(chunk);
    util.endDownload(&chunk);

    Traza::print(str, W_DEBUG);
    Json::Value root;   // will contains the root value after parsing.
    bool parsingSuccessful = JsonParser::parseJson(&root, str);

    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("GoogleDrive::launchAccessToken: Failed to parse configuration", W_ERROR);
    } else {
        //En el caso del refresh, no se devuelve el mismo token
        if (!refresh) refreshToken = root.get("refresh_token","").asString();
        //caso de autorizacion
        accessToken = root.get("access_token","").asString();
        tokenType = root.get("token_type","").asString();
        expiresIn = root.get("expires_in","").asString();
    }

    return accessToken;
}

/**
* Sube un fichero con la limitacion de que sea menor a 5120GB
*/
bool GoogleDrive::chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken){
    map<string, string> cabeceras;
    string postData = "";
    string AuthOauth2 = "Bearer " + accessToken;
    string url = GOOGLEURLPUT + "?uploadType=resumable";
    Dirutil dir;
    bool ret = false;

    postData = "{\"name\": \"" + Constant::toUTF8(dir.getFileName(filesystemPath)) + "\""
           + "," + "\"mimeType\": \"application/octet-stream\""
           + string(cloudIdPath.empty() ? "" : ",\"parents\": [\"" + cloudIdPath + "\"]")
           + "}";

    size_t tam = 0;
//    unsigned long iteraciones = 0;
    ifstream file (filesystemPath.c_str(), ios::in|ios::binary|ios::ate);
    if (file.is_open()){
        tam = file.tellg();
//        iteraciones = tam / GOOGLECHUNK;
    }
    file.close();

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/json; charset=UTF-8"));
    cabeceras.insert( make_pair("X-Upload-Content-Type", "application/octet-stream"));
    cabeceras.insert( make_pair("X-Upload-Content-Length", Constant::TipoToStr(tam)));

    MemoryStruct *chunk = util.initDownload();
    util.httpPost(url, postData.c_str(),postData.length(), &cabeceras, chunk);
    string str = util.getData(chunk);
    long httpCode = util.getHttp_code(chunk);
    Traza::print( "code", httpCode, W_DEBUG);

    //Control error de token caducado de OAUTH2
    if (httpCode != 200){
        Json::Value root;   // will contains the root value after parsing.
        uint32_t oauthOut = checkOauthErrors(str, &root);
        if (oauthOut == ERRORREFRESHTOKEN){
            this->storeAccessToken("", true);
            util.endDownload(&chunk);
            //Utilizando recursividad
            return chunckedUpload(filesystemPath, cloudIdPath, this->getAccessToken());
        }
    }

    //Control error de token caducado de OAUTH2
    string location;
    std::map<string,string>::iterator it;
    it = chunk->cabecerasResp.find("Location");
    if (it != chunk->cabecerasResp.end()){
        location = it->second;
    }

    if (!location.empty()){
        Traza::print("Enviando al location:'" + location, W_DEBUG);
        size_t pos = 0;
        while (pos < tam){
            ret = resumableChunckedUpload(filesystemPath, location, pos, tam, accessToken);
            pos += GOOGLECHUNK;
        }
    }
    util.endDownload(&chunk);
    return ret;
}

/**
*
*/
bool GoogleDrive::resumableChunckedUpload(string filesystemPath, string url, size_t offset, size_t tam, string accessToken){
    map<string, string> cabeceras;
    string AuthOauth2 = "Bearer " + accessToken;
    size_t chunkFileSize = 0;

    chunkFileSize = GOOGLECHUNK;
    if (offset + chunkFileSize > tam)
        chunkFileSize = tam - offset;

    cabeceras.clear();
    //A request that sends the first 524,288 bytes might look like this:
    //cabeceras.insert( make_pair("Content-Range", "bytes 0-524287/2000000"));
    string contentRange = "bytes " + Constant::TipoToStr(offset)+"-"
                                +Constant::TipoToStr(offset + chunkFileSize - 1) + "/" + Constant::TipoToStr(tam);
    Traza::print(contentRange, W_DEBUG);
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Content-Type", "application/octet-stream"));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));

    //Realizamos el put
    MemoryStruct *chunk = util.initDownload();
    util.setSendContentLength(chunk, 0);
    util.httpPut(url, filesystemPath.c_str(), chunkFileSize, offset, &cabeceras, chunk);
    string str = util.getData(chunk);
    long httpCode = util.getHttp_code(chunk);
    util.endDownload(&chunk);

    Traza::print("Codigo", httpCode, W_DEBUG);
    Traza::print(str, W_DEBUG);

    //Control error de token caducado de OAUTH2
    Json::Value root;   // will contains the root value after parsing.
    uint32_t oauthOut = checkOauthErrors(str, &root);
    if (oauthOut == ERRORREFRESHTOKEN){
        this->storeAccessToken("", true);
        //Utilizando recursividad
        return resumableChunckedUpload(filesystemPath, url, offset, tam, this->getAccessToken());
    }
    //Control error de token caducado de OAUTH2

    return (httpCode == 200);
}

/**
*
*/
string GoogleDrive::mkdir(string dirname, string parentid, string accessToken){
    map<string, string> cabeceras;
    string postData = "";
    string AuthOauth2 = "Bearer " + accessToken;
    string url = GOOGLEURLMKDIR + "?uploadType=multipart";
    string id;

    postData = "{\"name\": \"" + dirname + "\""
               + "," + "\"mimeType\": \"application/vnd.google-apps.folder\""
               + string(parentid.empty() ? "" : ",\"parents\": [\"" + parentid + "\"]")
               + "}";

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/json; charset=UTF-8"));
    cabeceras.insert( make_pair("X-Upload-Content-Type", "application/vnd.google-apps.folder"));

    Traza::print(postData, W_DEBUG);

    MemoryStruct *chunk = util.initDownload();
    util.httpPost(url, postData.c_str(), postData.length(), &cabeceras, chunk);
    string str = util.getData(chunk);
    long httpCode = util.getHttp_code(chunk);
    util.endDownload(&chunk);
    Traza::print("code", httpCode, W_DEBUG);

    Json::Value root;
    //Control error de token caducado de OAUTH2
    uint32_t oauthOut = checkOauthErrors(str, &root);
    if (oauthOut == ERRORREFRESHTOKEN){
        this->storeAccessToken("", true);
        //Utilizando recursividad
        return mkdir(dirname, parentid, this->getAccessToken());
    }
    bool parsingSuccessful = (oauthOut == SINERROR);
    //Control error de token caducado de OAUTH2

    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("GoogleDrive::mkdir: Failed to parse configuration", W_ERROR);
    } else {
        //En el caso del refresh, no se devuelve el mismo token
        id = root.get("id","").asString();
    }

    return id;
}

/**
*
*/
string GoogleDrive::getJSONList(string fileid, string accessToken, string nextPageToken){
    map<string, string> cabeceras;
    string responseMetadata;
    string url = "";
    string AuthOauth2 = "Bearer " + accessToken;

    if (!accessToken.empty()){
            cabeceras.clear();
            cabeceras.insert( make_pair("Authorization", AuthOauth2));
            cabeceras.insert( make_pair("Accept", "*/*"));
            cabeceras.insert( make_pair("Accept-Encoding", "gzip,deflate"));
            cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
            cabeceras.insert( make_pair("Content-Type", "text/plain"));

            if (fileid.empty()){
                url = GOOGLEURLLIST + "?corpus=user&spaces=drive&orderBy=folder,name";
            } else {

                string q = "'" + fileid + "' in parents";
                q.append(" and trashed = false");

                url = GOOGLEURLLIST + "?corpus=user&spaces=drive&orderBy=folder,name&q=" + Constant::url_encode(q);
                if (!nextPageToken.empty()){
                    url.append("&pageToken=" + Constant::url_encode(nextPageToken));
                }
            }

            Traza::print("Llamando a la url: " + url, W_DEBUG);
            MemoryStruct *chunk = util.initDownload();
            util.httpGet(url, &cabeceras, chunk);
            string str = util.getData(chunk);
            Traza::print("Bytes obtenidos", util.getDataLength(chunk), W_DEBUG);
            util.endDownload(&chunk);
            responseMetadata = str;
    }
    return responseMetadata;
}

/**
*
*/
bool GoogleDrive::listFiles(string fileid, string accessToken, CloudFiles *files){
    Json::Value root;   // will contains the root value after parsing.
    string nextPageToken;
    string resp;
    int controlBucle = 0;

    do{
        resp = getJSONList(fileid, accessToken, nextPageToken);
        nextPageToken = "";
        Traza::print(resp, W_DEBUG);
        //Obtenemos el id del directorio de musica
        uint32_t oauthOut = checkOauthErrors(resp, &root);
        if (oauthOut == ERRORREFRESHTOKEN){
            this->storeAccessToken("", true);
            //Utilizando recursividad
            return listFiles(fileid, this->getAccessToken(), files);
        }
        bool parsingSuccessful = (oauthOut == SINERROR);

        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            Traza::print("GoogleDrive::listFiles: Failed to parse configuration", W_ERROR);
        } else {
            //En el caso del refresh, no se devuelve el mismo token
            const Json::Value arrFiles = root["files"];
            for (unsigned int index = 0; index < arrFiles.size(); index++ ){
                CloudFiles *childFile = new CloudFiles();
    //            childFile->revision = contents[index].get("rev","0").asString();
    //            childFile->bytes = contents[index].get("bytes",0).asDouble();
                childFile->strHash = arrFiles[index].get("id","").asString();
                childFile->path = Constant::toAnsiString(arrFiles[index].get("name","").asString());
                childFile->root = fileid;
                childFile->isDir = arrFiles[index].get("mimeType","").asString().compare("application/vnd.google-apps.folder") == 0;
                files->fileList.push_back(childFile);
            }
            nextPageToken = root.get("nextPageToken","").asString();
        }
        controlBucle++;
    } while (!nextPageToken.empty() && controlBucle < 10);
    return true;
}

/**
*
*/
string GoogleDrive::fileExist(string filename, string parentid, string accessToken){
    string resp = getJSONList(parentid, accessToken, "");
    Json::Value root;   // will contains the root value after parsing.
    string retorno;
    bool encontrado = false;

    //Obtenemos el id del directorio de musica
    uint32_t oauthOut = checkOauthErrors(resp, &root);
    if (oauthOut == ERRORREFRESHTOKEN){
        this->storeAccessToken("", true);
        //Utilizando recursividad
        return fileExist(filename, parentid, this->getAccessToken());
    }
    bool parsingSuccessful = (oauthOut == SINERROR);
    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("GoogleDrive::listFiles: Failed to parse configuration", W_ERROR);
    } else {
        //En el caso del refresh, no se devuelve el mismo token
        const Json::Value arrFiles = root["files"];
        for (unsigned int index = 0; index < arrFiles.size() && encontrado == false; index++){
            if (arrFiles[index].get("name","").asString().compare(filename) == 0){
                retorno = arrFiles[index].get("id","").asString();
                encontrado = true;
            }
        }
    }
    return retorno;
}

/**
* Se baja un fichero
*/
int GoogleDrive::getFile(string filesystemPath, string cloudIdPath, string accessToken){
    map<string, string> cabeceras;
    string url = GOOGLEURLGET + Constant::uencodeUTF8(cloudIdPath) + "?alt=media";
    string AuthOauth2 = "Bearer " + accessToken;

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));

    MemoryStruct *chunk = util.initDownload();
    util.downloadToDiskHeaders(url, filesystemPath, &cabeceras, chunk);
    long httpCode = util.getHttp_code(chunk);
    util.endDownload(&chunk);

    Traza::print("GoogleDrive::getFile. Code", httpCode, W_DEBUG);
    if (httpCode == 401){
        this->storeAccessToken("", true);
        //Utilizando recursividad
        return getFile(filesystemPath, cloudIdPath, this->getAccessToken());
    } else if (httpCode != 200){
        Traza::print(string("Error descargando ") + cloudIdPath + " en " + filesystemPath, W_ERROR);
        return -1;
    }
    return 0;
}

/**
*
*/
bool GoogleDrive::deleteFiles(string fileid, string accessToken){

    map<string, string> cabeceras;
    string url = GOOGLEURLDELETE + Constant::uencodeUTF8(fileid);
    string AuthOauth2 = "Bearer " + accessToken;

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));

    MemoryStruct *chunk = util.initDownload();
    util.httpDel(url, &cabeceras, chunk);
    long httpCode = util.getHttp_code(chunk);

    if (httpCode == 401){
        this->storeAccessToken("", true);
        //Utilizando recursividad
        return deleteFiles(fileid, this->getAccessToken());
    }

    return httpCode == 204;
}
