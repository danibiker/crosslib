/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   onedrive.cpp
 * Author: Timonet
 * 
 * Created on 28 de octubre de 2017, 14:43
 */

#include "onedrive.h"

Onedrive::Onedrive() {
}

Onedrive::~Onedrive() {
}

/**
 * 
 * @return 
 */
uint32_t Onedrive::authenticate(){
    string strAccessToken;
    string strRefreshToken;
    filecipher cifrador;
    string accessTokenCipher;
    string refreshTokenCipher;
    Dirutil dir;
    string display_name, email;
    uint32_t retorno = SINERROR;

    try{
        Traza::print("Onedrive::authenticate. rutaini: " + rutaIni, W_DEBUG);
        if (dir.existe(rutaIni)){
            //Obtenemos el access token almacenado en el fichero de configuracion
            ListaIni<Data> *config = new ListaIni<Data>();
            config->loadFromFile(rutaIni);
            config->sort();
            
            Data elem;
            int pos = config->find(ONEDRIVEACCESSTOKENSTR);
            if (pos >= 0){
                elem = config->get(pos);
                accessTokenCipher = elem.getValue();
                strAccessToken = cifrador.decodeEasy(accessTokenCipher, passwordAT);
            }
            
            if (pos >= 0){
                pos = config->find(ONEDRIVEFRESHTOKENSTR);
                elem = config->get(pos);
                refreshTokenCipher = elem.getValue();
                strRefreshToken = cifrador.decodeEasy(refreshTokenCipher, passwordAT);
            }

            string url = "https://graph.microsoft.com/v1.0/me";
            string AuthOauth2 = "Bearer " + strAccessToken;
            map<string, string> cabeceras;

            cabeceras.clear();
            cabeceras.insert( make_pair("Authorization", AuthOauth2));
            cabeceras.insert( make_pair("Accept", "*/*"));
//            cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
//            cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
            //cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
            cabeceras.insert( make_pair("Content-Type", "application/json"));
            util.get(url, &cabeceras);

            //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
            Json::Value root;   // will contains the root value after parsing.
//            Json::Reader reader;
            
            string ret = util.getData();
            Traza::print(ret, W_DEBUG);
            
            if (!ret.empty()){
                retorno = checkOauthErrors(util.getData(), &root);
            }
            if(retorno == SINERROR){
                display_name = root.get("displayName","").asString();
                email = root.get("userPrincipalName","").asString();
                Traza::print("Usuario " + display_name + " con correo " + email + " autenticado con exito.", W_DEBUG);
            } else {
                Traza::print("Onedrive::authenticate. Se ha producido un error en el proceso de autorizacion", W_ERROR);
            }
        } else {
            Traza::print("Onedrive::authenticate. No se ha encontrado el fichero de configuracion para oauth", W_ERROR);
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
 * @param clientid
 */
void Onedrive::launchAuthorize(string clientid){
    
    string tmpUrl = ONEDRIVEURLAUTH
                    + "?client_id=" + Constant::url_encode(clientid)
                    + "&scope=" + Constant::url_encode("files.readwrite offline_access User.Read")
                    + "&response_type=code"
                    //+ "&redirect_uri=" + Constant::url_encode("urn:ietf:wg:oauth:2.0:oob")
                    + "&redirect_uri=" + Constant::url_encode("https://login.live.com/oauth20_desktop.srf")
//                    + "&redirect_uri=" + Constant::url_encode("https://login.microsoftonline.com/common/oauth2/nativeclient")
                    ;
    string cmd = CMD_LAUNCH_BROWSER + " \"" + tmpUrl + "\"";
    system(cmd.c_str());
}

/**
 * 
 * @param clientid
 * @param secret
 * @param code
 * @param refresh
 * @return 
 */
string Onedrive::launchAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
    map<string, string> cabeceras;
    string postData;

    cabeceras.clear();
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));

    postData= "client_id=" + clientid
            //+ "&client_secret=" + secret
            ;

    if (refresh){
        postData += "&refresh_token=" + codeOrRefreshToken + "&grant_type=refresh_token";
    } else {
        postData += "&code=" + codeOrRefreshToken + "&grant_type=authorization_code"
            //+ "&redirect_uri=" + Constant::url_encode("urn:ietf:wg:oauth:2.0:oob");
            + "&redirect_uri=" + Constant::url_encode("https://login.live.com/oauth20_desktop.srf");
//            + "&redirect_uri=" + Constant::url_encode("https://login.microsoftonline.com/common/oauth2/nativeclient");
    }

    Traza::print(postData, W_DEBUG);
    util.post(ONEDRIVEURLTOKEN, postData.c_str(),postData.length(), &cabeceras);
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    Traza::print(util.getData(), W_DEBUG);

    bool parsingSuccessful = reader.parse( util.getData(), root );
    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("GoogleDrive::launchAccessToken: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
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
 * 
 * @param fileid
 * @param accessToken
 * @return 
 */
bool Onedrive::deleteFiles(string fileid, string accessToken){
    map<string, string> cabeceras;
    string postData = "";
    string AuthOauth2 = "Bearer " + accessToken;
    string url = "";
    
    if (((int)fileid.find_first_of("!")) >= 0) {
        url = ONEDRIVEURLDELETE + Constant::url_encode(fileid) ;
    } else {
        return false;
    }
    
    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));

    util.del(url, &cabeceras);
    Traza::print( "code", util.getHttp_code(), W_DEBUG);
    
    return util.getHttp_code() == 204;
}

/**
 * 
 * @param filesystemPath
 * @param cloudIdPath
 * @param accessToken
 * @return 
 */
bool Onedrive::chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken){
    map<string, string> cabeceras;
    string postData = "";
    string AuthOauth2 = "Bearer " + accessToken;
    string url = "";
    bool ret = false;
    
    if (((int)cloudIdPath.find_first_of("!")) >= 0) {
        url = ONEDRIVEURLPUT + "/items/" + Constant::url_encode(cloudIdPath) + "/createUploadSession";
    } else {
        url = ONEDRIVEURLPUT + "root:/" + Constant::url_encode(cloudIdPath) + ":/createUploadSession";
    }
    
    Traza::print( "url: " + url, W_DEBUG);
    
    Dirutil dir;
    Json::Value postArg;
    Json::Value parmItem;
    parmItem["@microsoft.graph.conflictBehavior"] = "replace"; //fail, replace, or rename
    parmItem["name"] = dir.getFileName(filesystemPath);
    postArg["item"] = parmItem;

    Json::FastWriter fastWriter;
    postData = fastWriter.write(postArg);

    size_t tam = 0;
    unsigned long iteraciones = 0;
    ifstream file (filesystemPath.c_str(), ios::in|ios::binary|ios::ate);
    if (file.is_open()){
        tam = file.tellg();
        iteraciones = tam / ONEDRIVECHUNK;
    }
    file.close();

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/json; charset=UTF-8"));

    util.post(url, postData.c_str(), postData.length(), &cabeceras);
    Traza::print( "code", util.getHttp_code(), W_DEBUG);

    //Control error de token caducado de OAUTH2
    if (util.getHttp_code() != 200){
        Json::Value root;   // will contains the root value after parsing.
        uint32_t oauthOut = checkOauthErrors(util.getData(), &root);
        if (oauthOut == ERRORREFRESHTOKEN){
            this->storeAccessToken(this->getClientid(), this->getSecret(), this->getRefreshToken(), true);
            //Utilizando recursividad
            return chunckedUpload(filesystemPath, cloudIdPath, this->getAccessToken());
        }
    }
    //Control error de token caducado de OAUTH2
    string resp = util.getData();
    string location;
    
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    Traza::print(resp, W_DEBUG);
    bool parsingSuccessful = reader.parse( resp, root );
    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("Onedrive::chunckedUpload: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
    } else {
        // Get the value of the member of root named 'hash', return '' if there is no
        // such member.
        location = root.get("uploadUrl","").asString();
    }
    
    if (!location.empty()){
        Traza::print("Enviando al location:'" + location, W_DEBUG);
        size_t pos = 0;
        bool resp = true; 
        while (pos < tam && resp){
            resp = resumableChunckedUpload(filesystemPath, location, pos, tam, accessToken);
            pos += ONEDRIVECHUNK;
            ret = resp;
        }
    }
    return ret;
}

/**
 * 
 * @param filesystemPath
 * @param url
 * @param offset
 * @param tam
 * @param accessToken
 * @return 
 */
bool Onedrive::resumableChunckedUpload(string filesystemPath, string url, size_t offset, size_t tam, string accessToken){
    map<string, string> cabeceras;
    string AuthOauth2 = "Bearer " + accessToken;
    Dirutil dir;
    size_t chunkFileSize = 0;

    chunkFileSize = ONEDRIVECHUNK;
    if (offset + chunkFileSize > tam)
        chunkFileSize = tam - offset;

    cabeceras.clear();
    //A request that sends the first 524,288 bytes might look like this:
    //cabeceras.insert( make_pair("Content-Range", "bytes 0-524287/2000000"));
    string contentRange = "bytes " + Constant::TipoToStr(offset)+"-"
                                +Constant::TipoToStr(offset + chunkFileSize - 1) + "/" + Constant::TipoToStr(tam);
    Traza::print(contentRange, W_DEBUG);
    cabeceras.insert( make_pair("Content-Range", contentRange));
//    cabeceras.insert( make_pair("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.112 Safari/537.36"));
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Content-Type", "application/octet-stream"));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    //util.setSendContentLength(false);
    //Realizamos el put
    util.put(url, filesystemPath.c_str(), chunkFileSize, offset, &cabeceras);
    Traza::print("Codigo", util.getHttp_code(), W_DEBUG);
    Traza::print(util.getData(), W_DEBUG);

    //Control error de token caducado de OAUTH2
    Json::Value root;   // will contains the root value after parsing.
    uint32_t oauthOut = checkOauthErrors(util.getData(), &root);
    //Control error de token caducado de OAUTH2
    if (oauthOut == ERRORREFRESHTOKEN){
        this->storeAccessToken(this->getClientid(), this->getSecret(), this->getRefreshToken(), true);
        //Utilizando recursividad
        return resumableChunckedUpload(filesystemPath, url, offset, tam, this->getAccessToken());
    }
    
    //HTTP/1.1 100 Continue, HTTP/1.1 202 Accepted
    return (util.getHttp_code() == 100 || util.getHttp_code() == 202);
}

/**
 * 
 * @param clientid
 * @param secret
 * @param codeOrRefreshToken
 * @param refresh
 * @return 
 */
string Onedrive::storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
    Traza::print("Onedrive::storeAccessToken. Negociando access token...", W_DEBUG);
    filecipher cifrador;
    launchAccessToken(clientid, secret, codeOrRefreshToken, refresh);

    string accessTokenCipherB64 = cifrador.encodeEasy(this->getAccessToken(), passwordAT);
    string refreshTokenCipherB64 = cifrador.encodeEasy(this->getRefreshToken(), passwordAT);

    ListaIni<Data> *config = new ListaIni<Data>();
    try{
        Dirutil dir;
        if (dir.existe(rutaIni)){
            config->loadFromFile(rutaIni);
            config->sort();
        }
        this->addToken(ONEDRIVEACCESSTOKENSTR, accessTokenCipherB64, config);
        this->addToken(ONEDRIVEFRESHTOKENSTR, refreshTokenCipherB64, config);
        config->writeToFile(rutaIni);

    } catch (Excepcion &e){
        Traza::print("Onedrive::storeAccessToken. Error al cargar la configuracion", W_ERROR);
    }

    return this->getAccessToken();
}

/**
 * 
 * @param fileid
 * @param accessToken
 * @param nextPageToken
 * @return 
 */
string Onedrive::getJSONList(string fileid, string accessToken, string nextPageToken){
    map<string, string> cabeceras;
    string postData;
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
            url = ONEDRIVEURLLIST;
        } else if (nextPageToken.empty() && ((int)fileid.find_first_of("!")) >= 0) {
            url = ONEDRIVEURLLISTCHILDREN + "/" + Constant::url_encode(fileid) + "/children";
        } else if (nextPageToken.empty()){
            url = ONEDRIVEURLLISTCHILDRENPATH + ":/" + Constant::url_encode(fileid) + ":/children";
        } else {
            url = nextPageToken;
        }
        util.get(url, &cabeceras);
        return util.getData();
    }
    return responseMetadata;
}

/**
 * 
 * @param filesystemPath
 * @param accessToken
 * @param files
 * @return 
 */
bool Onedrive::listFiles(string filesystemPath, string accessToken, CloudFiles *files){
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    string nextPageToken;
    string resp;
    int controlBucle = 0;
    
    do{
        resp = getJSONList(filesystemPath, accessToken, nextPageToken);
        nextPageToken = "";
        Traza::print(resp, W_DEBUG);
        //Obtenemos el id del directorio de musica
        uint32_t oauthOut = checkOauthErrors(resp, &root);
        if (oauthOut == ERRORREFRESHTOKEN){
            this->storeAccessToken(this->getClientid(), this->getSecret(), this->getRefreshToken(), true);
            //Utilizando recursividad
            return listFiles(filesystemPath, this->getAccessToken(), files);
        }
        bool parsingSuccessful = (oauthOut == SINERROR);

        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            Traza::print("GoogleDrive::listFiles: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
        } else {
            //En el caso del refresh, no se devuelve el mismo token
            const Json::Value arrFiles = root["value"];
            for ( int index = 0; index < arrFiles.size(); index++ ){
                CloudFiles *childFile = new CloudFiles();
                childFile->strHash = arrFiles[index].get("id","").asString();
                childFile->path = arrFiles[index].get("name","").asString();
                childFile->root = arrFiles[index].get("parentReference","").get("id","").asString();
                childFile->isDir = arrFiles[index]["folder"].isObject() ;
                files->fileList.push_back(childFile);
            }
            nextPageToken = root.get("@odata.nextLink","").asString();
        }
        controlBucle++;
    } while (!nextPageToken.empty() && controlBucle < 10);
    return true;
}

/**
 * 
 * @param filesystemPath
 * @param cloudIdPath
 * @param accessToken
 * @return 
 */
int Onedrive::getFile(string filesystemPath, string cloudIdPath, string accessToken){
    map<string, string> cabeceras;
    string url = ONEDRIVEURLGET + Constant::uencodeUTF8(cloudIdPath) + "/content";
    string AuthOauth2 = "Bearer " + accessToken;
    Traza::print(url, W_DEBUG);
    
    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));
    util.download(url, filesystemPath, &cabeceras);
    Traza::print("Onedrive::getFile. Code", util.getHttp_code(), W_DEBUG);
    Traza::print(util.getData(), W_DEBUG);
    
    if (util.getHttp_code() == 401){
        this->storeAccessToken(this->getClientid(), this->getSecret(), this->getRefreshToken(), true);
        //Utilizando recursividad
        return getFile(filesystemPath, cloudIdPath, this->getAccessToken());
    } if (util.getHttp_code() != 200){
        Traza::print(string("Error descargando ") + cloudIdPath + " en " + filesystemPath, W_ERROR);
        return -1;
    }
    return 0;
}

/**
*
*/
uint32_t Onedrive::checkOauthErrors(string data, Json::Value *root){
    uint32_t retorno = SINERROR;
    //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
    Json::Reader reader;
    if (!data.empty()){
        bool parsingSuccessful = reader.parse( data, *root);
        string errorText;

        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            Traza::print("GoogleDrive::checkOauthErrors: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
            retorno = ERRORACCESSTOKEN;
        } else {
            Json::Value error = (*root)["error"];
            errorText = error.get("message","").asString();
            string code = error.get("code","").asString();
            if (code.compare("InvalidAuthenticationToken") == 0){
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

/**
*
*/
string Onedrive::mkdir(string dirname, string parentid, string accessToken){
    map<string, string> cabeceras;
    string AuthOauth2 = "Bearer " + accessToken;
    //string url = ONEDRIVEMKDIR + Constant::uencodeUTF8(parentid) + "/children";
    string url;
    string id;
    
    if ( parentid.compare("/") == 0) {
        url = url = ONEDRIVEMKDIR + "root/" + "/children";
    } else {
        url = ONEDRIVEMKDIR + "items/" + Constant::uencodeUTF8(parentid) + "/children";
    }
    Traza::print("url: " + url, W_DEBUG);
 
    string postData = "{\"name\": \"" + dirname + "\""
               + "," + "\"folder\": { }"
               + ",\"@microsoft.graph.conflictBehavior\": \"replace\"" //fail, replace, or rename
               + "}";

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/json; charset=UTF-8"));

    Traza::print(postData, W_DEBUG);
    util.post(url, postData.c_str(), postData.length(), &cabeceras);
    Traza::print("code", util.getHttp_code(), W_DEBUG);

    Json::Value root;
    Json::Reader reader;

    //Control error de token caducado de OAUTH2
    string resp = util.getData();
    Traza::print(resp, W_DEBUG);
    
    uint32_t oauthOut = checkOauthErrors(resp, &root);
    if (oauthOut == ERRORREFRESHTOKEN){
        this->storeAccessToken(this->getClientid(), this->getSecret(), this->getRefreshToken(), true);
        //Utilizando recursividad
        return mkdir(dirname, parentid, this->getAccessToken());
    }
    bool parsingSuccessful = (oauthOut == SINERROR);
    //Control error de token caducado de OAUTH2
    //bool parsingSuccessful = reader.parse( util.getData(), root );

    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("Onedrive::mkdir: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
    } else {
        //En el caso del refresh, no se devuelve el mismo token
        id = root.get("id","").asString();
    }
    return id;
}

/**
 * 
 * @param filename
 * @param parentid
 * @param accessToken
 * @return 
 */
string Onedrive::fileExist(string filename, string parentid, string accessToken){
    string resp = getJSONList(parentid, accessToken, "");
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    string retorno;
    bool encontrado = false;

    //Obtenemos el id del directorio de musica
    //bool parsingSuccessful = reader.parse( resp, root );

    uint32_t oauthOut = checkOauthErrors(resp, &root);
    if (oauthOut == ERRORREFRESHTOKEN){
        this->storeAccessToken(this->getClientid(), this->getSecret(), this->getRefreshToken(), true);
        //Utilizando recursividad
        return fileExist(filename, parentid, this->getAccessToken());
    }
    bool parsingSuccessful = (oauthOut == SINERROR);

    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("Onedrive::listFiles: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
    } else {
        //En el caso del refresh, no se devuelve el mismo token
        const Json::Value arrFiles = root["value"];
        string tmpFileName;
        for ( int index = 0; index < arrFiles.size() && encontrado == false; index++){
            tmpFileName = arrFiles[index].get("name","").asString();
            if (tmpFileName.compare(filename) == 0){
                retorno = arrFiles[index].get("id","").asString();
                encontrado = true;
            }
        }
    }
    return retorno;
}

int Onedrive::getShared(string accessToken){
    map<string, string> cabeceras;
    string url = "https://graph.microsoft.com/v1.0/me/drive/sharedWithMe";
    string AuthOauth2 = "Bearer " + accessToken;
    Traza::print(url, W_DEBUG);
    
    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));
    
    util.get(url, &cabeceras);
    
    Traza::print("Onedrive::getShared. Code", util.getHttp_code(), W_DEBUG);
    Traza::print(util.getData(), W_DEBUG);
    
    if (util.getHttp_code() == 401){
        this->storeAccessToken(this->getClientid(), this->getSecret(), this->getRefreshToken(), true);
        //Utilizando recursividad
        return getShared(this->getAccessToken());
    } if (util.getHttp_code() != 200){
        Traza::print("Error getShared ", W_ERROR);
        return -1;
    }
    return 0;
}