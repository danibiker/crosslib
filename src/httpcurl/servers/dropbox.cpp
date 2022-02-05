#include "dropbox.h"



/**
*
*/
Dropbox::Dropbox(){

}


/**
*
*/
Dropbox::~Dropbox(){
}


uint32_t Dropbox::authenticate(){
    string strAccessToken;
    string strRefreshToken;
    bool error=false;
    string display_name, email;
    uint32_t retorno = SINERROR;
    string errorText;

    try{
        Traza::print("Dropbox::authenticate", W_DEBUG);
        if (decodeTokens(strAccessToken, strRefreshToken, DROPBOXACCESSTOKENSTR, DROPBOXREFRESHTOKENSTR) == SINERROR){
            //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
            string AuthOauth2 = "Bearer " + strAccessToken;
            map<string, string> cabeceras;

            cabeceras.clear();
            cabeceras.insert( make_pair("Authorization", AuthOauth2));
            cabeceras.insert( make_pair("Accept", "*/*"));
            cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
            cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
            cabeceras.insert( make_pair("Content-Type", "application/json"));
            //DROPBOXAPIV2
            
            
            Traza::print("Dropbox intentando autenticar", W_DEBUG);
            MemoryStruct *chunk = util.initDownload();
            Json::Value postMsg; //Necesario para enviar algun dato a null en el post en formato json
            string postData = postMsg.toStyledString();
            
            util.httpPostStrlen(DROPBOXACCOUNTINFO, postData.c_str(), &cabeceras, chunk);
            string str = util.getData(chunk);
            long httpCode = util.getHttp_code(chunk);
            util.endDownload(&chunk);
            
            Traza::print("Dropbox httpCode", httpCode, W_DEBUG);
            Traza::print("Dropbox respuesta: " + str, W_DEBUG);
            
            //DROPBOXAPIV2
            Json::Value root;   // will contains the root value after parsing.
            bool parsingSuccessful = false;
            string errorJson;
            
            if (httpCode == 200 || httpCode == 401){
                parsingSuccessful = JsonParser::parseJson(&root, str, &errorJson);
            } else {
                error = true;
                retorno = ERRORCONNECT;
            }
            
            if ( !parsingSuccessful ){
                 // report to the user the failure and their locations in the document.
                Traza::print("Dropbox::authenticate: Failed to parse configuration. " + errorJson, W_ERROR);
                error = true;
                retorno = ERRORCONNECT;
            } else {
                errorText = root.get("error","").toStyledString();
                display_name = root.get("display_name","").asString();
                email = root.get("email","").asString();
                string strCleanError = Constant::removeEspecialCharsAll(errorText);
                error = !strCleanError.empty();
            }
        } else {
            Traza::print("Dropbox::authenticate: No se ha encontrado el fichero de autorizacion", W_ERROR);
            error = true;
            retorno = ERRORACCESSTOKEN;
        }
        //Comprobamos que el access token funciona
        //Si hemos detectado error lanzamos el proceso de autorizacion
        if (error){
            strAccessToken = "";
            Traza::print("Dropbox::authenticate: Mensaje de error " + errorText, W_DEBUG);
            if (errorText.find("OAuth 2 \"Authorization\" header is not well-formed") != string::npos
                || errorText.find("Invalid OAuth2 token") != string::npos)
            {
                retorno = ERRORACCESSTOKEN;
            } else if (errorText.find("expired_access_token") != string::npos){
                retorno = ERRORREFRESHTOKEN;
            } else if (retorno == SINERROR) {
                //Si no hemos conseguido identificar el error, especificamos error de conexion
                retorno = ERRORCONNECT;
                Traza::print("Dropbox::authenticate: Error: " + errorText, W_ERROR);
            }
        } else {
            Traza::print("Usuario " + display_name + " con correo " + email + " autenticado con exito.", W_DEBUG);
        }
    } catch (Excepcion &e){
        Traza::print("Error al cargar la configuracion", W_ERROR);
        retorno = ERRORACCESSTOKEN;
    }

    this->setAccessToken(strAccessToken);
    this->setRefreshToken(strRefreshToken);
    setOauthStatus(retorno);
    return retorno;
}

string Dropbox::storeAccessToken(string codeOrRefreshToken, bool refresh){
    return storeToken(DROPBOXACCESSTOKENSTR, DROPBOXREFRESHTOKENSTR, codeOrRefreshToken, refresh);
}

/**
*
*/
//string Dropbox::storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
//    filecipher cifrador;
//    Traza::print("Negociando access token...", W_DEBUG);
//    launchAccessToken(clientid, secret, codeOrRefreshToken, refresh);
//    
//    if (!this->accessToken.empty()){
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
//            if (!this->getAccessToken().empty()){
//                this->addToken(DROPBOXACCESSTOKENSTR, accessTokenCipherB64, config);
//            }
//            if (!this->getRefreshToken().empty()){
//                this->addToken(DROPBOXREFRESHTOKENSTR, refreshTokenCipherB64, config);
//            }
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
void Dropbox::launchAuthorize(string clientid){
    string tmpUrl = "\"" + DROPBOXURLAUTH + "?response_type=code&token_access_type=offline&client_id=" + clientid + "\"";
    //string cmd = CMD_LAUNCH_BROWSER + " \"" + tmpUrl + "\"";
    //system(cmd.c_str());
    openExplorer(tmpUrl);
}

/**
* Obtiene el access token para hacer subsiguientes peticiones
*/
string Dropbox::launchAccessToken(string clientid, string secret, string code, bool refresh){
    string url = DROPBOXURLTOKEN;
    string s = clientid + ":" + secret;
    string authenticationBasic = "Basic " + Constant::base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
    map<string, string> cabeceras;
    string postData;

    if (refresh){
        Traza::print("Dropbox::launchAccessToken: refreshing token", W_DEBUG);
        postData="refresh_token=" + code + "&grant_type=refresh_token";
    } else {
        Traza::print("Dropbox::launchAccessToken: obtaining token", W_DEBUG);
        postData="code=" + code + "&grant_type=authorization_code";
    }
    
    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", authenticationBasic));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
    
    MemoryStruct *chunk = util.initDownload();
    util.httpPost(url, postData.c_str(), postData.length(), &cabeceras, chunk);
    string datos = util.getData(chunk);
    util.endDownload(&chunk);

    Json::Value root;
    string errorJson;
    bool parsingSuccessful = JsonParser::parseJson(&root, datos, &errorJson);
    Traza::print("Dropbox::launchAccessToken: received" + datos, W_DEBUG);
    
    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("Dropbox::launchAccessToken: Failed to parse configuration. " + errorJson, W_ERROR);
    } else {
        // Get the value of the member of root named 'hash', return '' if there is no
        // such member.
        this->accessToken = root.get("access_token","").asString();
        if (!refresh){
            this->refreshToken = root.get("refresh_token","").asString();
        }
    }

    return this->accessToken;
}

/**
* Sube un fichero con la limitacion de que sea menor a 150MB
*/
int Dropbox::putFile(string filesystemPath, string dropboxPath, string accessToken){
    map<string, string> cabeceras;

    string url = DROPBOXURLPUT + dropboxPath;
    string AuthOauth2 = "Bearer " + accessToken;
    //postData = "Ejemplo automatizado de generacion del token";
    size_t tam;
    char * memblock;
    ifstream file (filesystemPath.c_str(), ios::in|ios::binary|ios::ate);
    std::string test;
    if (file.is_open()){
        tam = file.tellg();
        if (tam > 0){
            memblock = new char [tam];
            file.seekg (0, ios::beg);
            file.read (memblock, tam);
            Traza::print("the entire file content is in memory. Size=" + tam, W_DEBUG);

            for(size_t i=0; i < tam -1; i++){
                test = test + memblock[i];
            }
            test += '\0';
        }
        file.close();
    }
    else {
        Traza::print("Unable to open file", W_DEBUG);
        return -1;
    }

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));
    cabeceras.insert( make_pair("overwrite", "true"));
    
    if (tam > 0){
        MemoryStruct *chunk = util.initDownload();
        util.httpPost(url, memblock, tam, &cabeceras, chunk);
        util.endDownload(&chunk);
        free(memblock);
    }
    return 0;
}

/**
*
*/
bool Dropbox::chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken){
    map<string, string> cabeceras;
    string AuthOauth2 = "Bearer " + accessToken;
    //postData = "Ejemplo automatizado de generacion del token";
    size_t tam;
    string upId = "";
    size_t offsetForDropbox = 0;
    Json::Value root;   // will contains the root value after parsing.
    bool ret = false;

    int iteraciones = 0;
    size_t chunkFileSize = 0;
    
    static const size_t DROPBOXCHUNK = 150 * 1024 * 1024; //500KB

    if (!accessToken.empty()){
        ifstream file (filesystemPath.c_str(), ios::in|ios::binary|ios::ate);
        if (file.is_open()){
            tam = file.tellg();
            iteraciones = tam / DROPBOXCHUNK;
        } else {
            Traza::print("Dropbox::chunckedUpload. No se ha podido abrir el fichero: " + filesystemPath, W_ERROR);
            return false;
        }
        file.close();
        
        cabeceras.clear();
        cabeceras.insert( make_pair("Authorization", AuthOauth2));
        //DROPBOXAPIV2
        cabeceras.insert( make_pair("Content-Type", "application/octet-stream"));
        
//        Json::Value postArg;
//        postArg["close"] = false;
//        Json::FastWriter fastWriter;
//        std::string startoutput = fastWriter.write(postArg);
//        Traza::print(startoutput, W_DEBUG);
//        cabeceras.insert( make_pair("Dropbox-API-Arg", startoutput));      
        //util.post(DROPBOXURLPUTSTART, "", &cabeceras);
        
        offsetForDropbox = 0;   
        
        MemoryStruct *chunk = util.initDownload();
        util.httpPostWithOffset(DROPBOXURLPUTSTART, filesystemPath.c_str(), (tam < DROPBOXCHUNK) ? tam : DROPBOXCHUNK, offsetForDropbox, &cabeceras, chunk);
        string resp = util.getData(chunk);
        long httpCode = util.getHttp_code(chunk);
        util.endDownload(&chunk);
        
        string errorJson;
        bool parsingSuccessful = JsonParser::parseJson(&root, resp, &errorJson);
        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            //Traza::print("Dropbox::chunckedUpload: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
            Traza::print("Dropbox::chunckedUpload: " + resp, W_ERROR);
        } else {
            upId = root.get("session_id","").asString();
            Traza::print("Obtenido un session_id: " + upId, W_DEBUG);
        }
        //DROPBOXAPIV2

        offsetForDropbox += (tam < DROPBOXCHUNK) ? tam : DROPBOXCHUNK;     
        while (offsetForDropbox < tam){
            Traza::print("Subiendo %" , (offsetForDropbox / (float) tam) *100, W_DEBUG);
            Traza::print("Offset: " + Constant::TipoToStr(offsetForDropbox) 
                         + " tam: " + Constant::TipoToStr(chunkFileSize), W_DEBUG);

            cabeceras.clear();
            cabeceras.insert( make_pair("Authorization", AuthOauth2));
            cabeceras.insert( make_pair("Content-Type", "application/octet-stream"));
            cabeceras.insert( make_pair("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-GB; rv:1.8.1.14) Gecko/20080404 Firefox/2.0.0.14"));
            cabeceras.insert( make_pair("Accept", "*/*"));


            Json::Value postArg;
            Json::Value parmCursor;
            parmCursor["offset"] = (Json::UInt64)offsetForDropbox;
            parmCursor["session_id"]= upId.empty() ? "" : upId;
//                parmCursor["validateResponse"] = false;
//                parmCursor["file"] = cloudIdPath;
            postArg["close"] = false;
            postArg["cursor"] = parmCursor;
            //Json::FastWriter fastWriter;
            //std::string output = fastWriter.write(postArg);
            std::string output;
            JsonParser::parseJsonToString(&output, postArg);
            
            Traza::print(output, W_DEBUG);
            cabeceras.insert( make_pair("Dropbox-API-Arg", output));
            
            MemoryStruct *chunk = util.initDownload();
            util.httpPostWithOffset(DROPBOXURLPUTCHUNKED, filesystemPath.c_str(), chunkFileSize, offsetForDropbox, &cabeceras, chunk);
            httpCode = util.getHttp_code(chunk);
            Traza::print("retorno subida", httpCode, W_DEBUG);
            if (httpCode != 200){
                resp = util.getData(chunk);
                Traza::print(resp, W_DEBUG);
            }
            util.endDownload(&chunk);
            offsetForDropbox += DROPBOXCHUNK;
        }
        
        if (httpCode == 200) {
            chunkFileSize = tam - offsetForDropbox;
            Traza::print("FINAL. Offset: " + Constant::TipoToStr(offsetForDropbox) 
                             + " tam: " + Constant::TipoToStr(chunkFileSize), W_DEBUG);
            
            ret = commitChunkedUpload(filesystemPath, cloudIdPath, accessToken, upId, offsetForDropbox, chunkFileSize);
        }
    } else {
        Traza::print("Upload aborted. Access token empty", W_ERROR);
    }
    return ret;
}

/**
*
*/
bool Dropbox::commitChunkedUpload(string filesystemPath, string dropboxPath, string accessToken, string upId, size_t offset, size_t tam){
    string AuthOauth2 = "Bearer " + accessToken;
    map<string, string> cabeceras;
    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Content-Type", "application/octet-stream"));
    
    Json::Value postArg;
    Json::Value parmCursor;
    Json::Value partCommit;
    parmCursor["offset"] = (Json::UInt64)offset;
    parmCursor["session_id"]= upId.empty() ? "" : upId;
    partCommit["path"] = dropboxPath;
    partCommit["mode"] = "add";
    partCommit["autorename"] = false;
    partCommit["mute"] = false;    
    postArg["cursor"] = parmCursor;
    postArg["commit"] = partCommit;
    
    //Json::FastWriter fastWriter;
    //std::string output = fastWriter.write(postArg);
    std::string output;
    JsonParser::parseJsonToString(&output, postArg);
    Traza::print(output, W_DEBUG);
    cabeceras.insert( make_pair("Dropbox-API-Arg", output));
    //Envio del commit
    MemoryStruct *chunk = util.initDownload();
    util.httpPostWithOffset(DROPBOXURLCOMMITCHUNKED, filesystemPath.c_str(), tam, offset, &cabeceras, chunk);
    long httpCode = util.getHttp_code(chunk);
    string res = util.getData(chunk);
    util.endDownload(&chunk);
    
    //util.post(DROPBOXURLCOMMITCHUNKED, "", &cabeceras);
    Traza::print("retorno commit", httpCode, W_DEBUG);
    
    if (httpCode == 200){
        return true;
    } else {
        Traza::print("Dropbox::commitChunkedUpload. Error in commit: " + res, W_ERROR);
        return false;
    }
    
}

/**
*
*/
bool Dropbox::deleteFiles(string fileid, string accessToken){
    string url = DROPBOXURLDELETE;
    string AuthOauth2 = "Bearer " + accessToken;
    map<string, string> cabeceras;
    bool result = false;

    if (!fileid.empty()){
        cabeceras.clear();
        cabeceras.insert( make_pair("Authorization", AuthOauth2));
        cabeceras.insert( make_pair("Content-Type", "application/json"));
        Json::Value postArg;
        postArg["path"] = fileid;
//        Json::FastWriter fastWriter;
//        std::string output = fastWriter.write(postArg);
        std::string output;
        JsonParser::parseJsonToString(&output, postArg);       
        Traza::print("Ruta a eliminar: " + fileid, W_DEBUG);
        
        MemoryStruct *chunk = util.initDownload();
        util.httpPostStrlen(url, output.c_str(), &cabeceras, chunk);
        string resp = util.getData(chunk);
        util.endDownload(&chunk);
        Traza::print(resp, W_DEBUG);
        
        Json::Value root;   // will contains the root value after parsing.
        string errorJson;
        bool parsingSuccessful = JsonParser::parseJson(&root, resp, &errorJson);
        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            Traza::print("Dropbox::deleteFiles: Failed to parse configuration. " + errorJson, W_ERROR);
        } else {
            // Get the value of the member of root named 'hash', return '' if there is no
            // such member.
            string error = root.get("error","").asString();
            if (error.empty()){
                result = true;
            }
        }
        return result;

    } else {
        Traza::print("Delete aborted. Access token empty", W_ERROR);
        return false;
    }
}

/**
* Se baja un fichero
*/
int Dropbox::getFile(string filesystemPath, string cloudIdPath, string accessToken){
    string AuthOauth2 = "Bearer " + accessToken;
    map<string, string> cabeceras;
    Json::Value postArg;
    //Json::FastWriter fastWriter;
    cabeceras.clear();
    
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));
    postArg["path"] = cloudIdPath;
    //std::string output = fastWriter.write(postArg);
    std::string output;
    JsonParser::parseJsonToString(&output, postArg);

    cabeceras.insert( make_pair("Dropbox-API-Arg", output));
    
    Traza::print(string("Descargando ") + cloudIdPath + " en " + filesystemPath, W_DEBUG);
    
    MemoryStruct *chunk = util.initDownload();
    util.httpPostDownload(DROPBOXURLGET, filesystemPath.c_str(), &cabeceras, chunk);
    long httpCode = util.getHttp_code(chunk);
    util.endDownload(&chunk);
    
    if (httpCode != 200){
        Traza::print(string("Error descargando ") + cloudIdPath + " en " + filesystemPath, W_ERROR);
        return -1;
    }
        
    
    return 0;
}

/**
* Obtiene informacion de la ruta indicada
*/
string Dropbox::getJSONListDropbox(string filesystemPath, string accessToken){
    map<string, string> cabeceras;
    string responseMetadata;
    string AuthOauth2 = "Bearer " + accessToken;

    if (!accessToken.empty()){
        cabeceras.clear();
        cabeceras.insert( make_pair("Authorization", AuthOauth2));
        cabeceras.insert( make_pair("Content-Type", "application/json; charset=utf-8"));
        Json::Value postArg;
        postArg["path"] = filesystemPath;
        postArg["recursive"] = false;
        postArg["include_media_info"] = false;
        postArg["include_deleted"] = false;
        postArg["include_has_explicit_shared_members"] = false;
        postArg["include_mounted_folders"] = false;
        //Json::FastWriter fastWriter;
        //std::string output = fastWriter.write(postArg);
        std::string output;
        JsonParser::parseJsonToString(&output, postArg);

        MemoryStruct *chunk = util.initDownload();
        util.httpPostStrlen(DROPBOXURLLIST, output.c_str(), &cabeceras, chunk);
        responseMetadata = util.getData(chunk);
        util.endDownload(&chunk);

        Traza::print(responseMetadata, W_DEBUG);
    }
    return responseMetadata;
}

/**
* Obtiene el resto de elementos
*/
string Dropbox::getJSONListContinueDropbox(string cursor, string accessToken){
    map<string, string> cabeceras;
    string responseMetadata;
    string AuthOauth2 = "Bearer " + accessToken;

    if (!accessToken.empty()){
        cabeceras.clear();
        cabeceras.insert( make_pair("Authorization", AuthOauth2));
        cabeceras.insert( make_pair("Content-Type", "application/json"));
        Json::Value postArg;
        postArg["cursor"] = cursor;
        
        std::string output;
        JsonParser::parseJsonToString(&output, postArg);
//        Json::FastWriter fastWriter;
//        std::string output = fastWriter.write(postArg);
        
        MemoryStruct *chunk = util.initDownload();
        util.httpPostStrlen(DROPBOXURLLISTNEXT, output.c_str(), &cabeceras, chunk);
        responseMetadata = util.getData(chunk);
        util.endDownload(&chunk);
        Traza::print(responseMetadata, W_DEBUG);
    }
    return responseMetadata;
}

/**
*
*/
bool Dropbox::listFiles(string filesystemPath, string accessToken, CloudFiles *files){
    
    bool hasMore = false;
    string cursor = ""; //Campo para obtener mas registros si es necesario
    int controlBucle = 0;
    
    string responseMetadata = getJSONListDropbox(filesystemPath, accessToken);
    
    do{
        Json::Value root;   // will contains the root value after parsing.
        string errorJson;
        bool parsingSuccessful = JsonParser::parseJson(&root, responseMetadata, &errorJson);

        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            Traza::print("Dropbox::listFiles. Failed to parse configuration: " + errorJson, W_ERROR);
        } else {
            // Get the value of the member of root named 'hash', return '' if there is no
            // such member.
    //        files->strHash = root.get("hash","").asString();
    //        files->bytes = root.get("bytes",0).asDouble();
    //        files->path = root.get("path","").asString();
    //        files->revision = root.get("rev","0").asString();
    //        files->root = root.get("root","").asString();
    //        files->isDir = root.get("is_dir", true).asBool();
    //
    //        string out = "listFiles: " + files->strHash + ", " + Constant::TipoToStr(files->bytes)
    //                     + ", " + files->path + ", " + files->revision + ", "
    //                     + files->root + ", " + string(files->isDir ? "S":"N");
    //
    //        Traza::print(out, W_DEBUG);

            hasMore = root.get("has_more", false).asBool();
            cursor = root.get("cursor","").asString();
            const Json::Value contents = root["entries"];

            for ( int index = 0; index < contents.size(); index++ ){  // Iterates over the sequence elements.
                CloudFiles *childFile = new CloudFiles();
    //            childFile->revision = contents[index].get("rev","0").asString();
                childFile->strHash = contents[index].get("id","").asString();
    //            childFile->bytes = contents[index].get("bytes",0).asDouble();
                childFile->path = contents[index].get("path_lower","").asString();
    //            childFile->root = contents[index].get("root","").asString();
                childFile->isDir = contents[index].get(".tag","").asString() == "folder" ? true : false;
                files->fileList.push_back(childFile);
            }
            controlBucle++;
        } 
        
        if (hasMore) 
            responseMetadata = getJSONListDropbox(filesystemPath, accessToken);
        
    } while (hasMore && controlBucle < 10);
    
    return true;
}
