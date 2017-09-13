#include "dropbox.h"
#include "listaIni.h"


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
    filecipher cifrador;
    string accessTokenCipherB64;
    string accessTokenCipher;
    Dirutil dir;
    bool error=false;
    string display_name, email;
    uint32_t retorno = SINERROR;
    string errorText;

    try{
        Traza::print("rutaini: " + rutaIni, W_DEBUG);
        if (dir.existe(rutaIni)){
            //Obtenemos el access token almacenado en el fichero de configuracion
            ListaIni<Data> *config = new ListaIni<Data>();
            config->loadFromFile(rutaIni);
            config->sort();
            int pos = config->find(DROPBOXACCESSTOKENSTR);
            Data elem = config->get(pos);
            accessTokenCipher = elem.getValue();
            strAccessToken = cifrador.decodeEasy(accessTokenCipher, passwordAT);
            //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
            string url = "https://api.dropboxapi.com/1/account/info";
            string AuthOauth2 = "Bearer " + strAccessToken;
            map<string, string> cabeceras;

            cabeceras.clear();
            cabeceras.insert( make_pair("Authorization", AuthOauth2));
            cabeceras.insert( make_pair("Accept", "*/*"));
            cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
            cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
            cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
            util.get(url, &cabeceras);
            Json::Value root;   // will contains the root value after parsing.
            Json::Reader reader;
            bool parsingSuccessful = reader.parse( util.getData(), root );
            Traza::print("util.getHttp_code()", util.getHttp_code(), W_DEBUG);
            Traza::print("util.getData(): " + string(util.getData()), W_DEBUG);

            if ( !parsingSuccessful ){
                 // report to the user the failure and their locations in the document.
                Traza::print("Dropbox::authenticate: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
                error = true;
                retorno = ERRORCONNECT;
            } else {
                errorText = root.get("error","").asString();
                display_name = root.get("display_name","").asString();
                email = root.get("email","").asString();
                error = !errorText.empty();
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
            if (errorText.find("OAuth 2 \"Authorization\" header is not well-formed") != string::npos
                || errorText.find("Invalid OAuth2 token") != string::npos)
            {
                retorno = ERRORACCESSTOKEN;
            } else if (retorno == SINERROR) {
                //Si no hemos conseguido identificar el error, especificamos error de conexion
                retorno = ERRORCONNECT;
            }
        } else {
            Traza::print("Usuario " + display_name + " con correo " + email + " autenticado con exito.", W_DEBUG);
        }
    } catch (Excepcion &e){
        Traza::print("Error al cargar la configuracion", W_ERROR);
        retorno = ERRORACCESSTOKEN;
    }

    this->setAccessToken(strAccessToken);
    setOauthStatus(retorno);
    return retorno;
}

/**
*
*/
string Dropbox::storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
    filecipher cifrador;
    Traza::print("Negociando access token...", W_DEBUG);
    launchAccessToken(clientid, secret, codeOrRefreshToken, refresh);
    string accessTokenCipherB64 = cifrador.encodeEasy(this->getAccessToken(), passwordAT);
    string datos = DROPBOXACCESSTOKENSTR + "=" + accessTokenCipherB64;
    ListaIni<Data> *config = new ListaIni<Data>();
    try{
        Dirutil dir;
        if (dir.existe(rutaIni)){
            config->loadFromFile(rutaIni);
            config->sort();
        }
        this->addToken(DROPBOXACCESSTOKENSTR, accessTokenCipherB64, config);
        config->writeToFile(rutaIni);

    } catch (Excepcion &e){
        Traza::print("GoogleDrive::storeAccessToken. Error al cargar la configuracion", W_ERROR);
    }

    return this->getAccessToken();
}

/**
* lanza la autorizacion desde el explorador para obtener un code
*/
void Dropbox::launchAuthorize(string clientid){
    string tmpUrl = DROPBOXURLAUTH + "?response_type=code&client_id=" + clientid;
    string cmd = CMD_LAUNCH_BROWSER + " \"" + tmpUrl + "\"";
    system(cmd.c_str());
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

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", authenticationBasic));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
    postData="code=" + code + "&grant_type=authorization_code";
    util.post(url, postData.c_str(),postData.length(), &cabeceras);

    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    
    string datos = util.getData();
    Traza::print(datos, W_DEBUG);
    
    bool parsingSuccessful = reader.parse( util.getData(), root );
    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("Dropbox::launchAccessToken: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
    } else {
        // Get the value of the member of root named 'hash', return '' if there is no
        // such member.
        accessToken = root.get("access_token","").asString();
    }

    return accessToken;
}

/**
* Sube un fichero con la limitacion de que sea menor a 150MB
*/
int Dropbox::putFile(string filesystemPath, string dropboxPath, string accessToken){
    map<string, string> cabeceras;
    string postData;

    string url = DROPBOXURLPUT + dropboxPath;
    string AuthOauth2 = "Bearer " + accessToken;
    //postData = "Ejemplo automatizado de generacion del token";
    size_t tam;
    char * memblock;
    ifstream file (filesystemPath.c_str(), ios::in|ios::binary|ios::ate);
    std::string test;
    if (file.is_open()){
        tam = file.tellg();
        memblock = new char [tam];
        file.seekg (0, ios::beg);
        file.read (memblock, tam);
        Traza::print("the entire file content is in memory. Size=" + tam, W_DEBUG);

        for(size_t i=0; i < tam -1; i++){
            test = test + memblock[i];
        }
        test += '\0';
        file.close();
    }
    else {
        Traza::print("Unable to open file", W_DEBUG);;
    }

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));
    cabeceras.insert( make_pair("overwrite", "true"));
    util.post(url,memblock,tam,&cabeceras);
    //cout << util.getData() << endl;
    delete[] memblock;

    return 0;
}

/**
*
*/
string Dropbox::chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken){
    string upId = chunckedUpload(filesystemPath, accessToken);
    commitChunkedUpload(cloudIdPath, accessToken, upId);
    return upId;
}


/**
* Sube un fichero sin limitacion de tamanyo
*/
string Dropbox::chunckedUpload(string filesystemPath, string accessToken){
    map<string, string> cabeceras;
    string postData;

    string url = "";
    string AuthOauth2 = "Bearer " + accessToken;
    //postData = "Ejemplo automatizado de generacion del token";
    size_t tam;
    string upId = "";
    size_t offsetForDropbox = 0;

    int iteraciones = 0;
    size_t chunkFileSize = 0;
    struct stat file_info;

    if (!accessToken.empty()){
        ifstream file (filesystemPath.c_str(), ios::in|ios::binary|ios::ate);
        if (file.is_open()){
            tam = file.tellg();
            iteraciones = tam / CHUNCKSIZE;
        }
        file.close();

        for (int posIt = 0; posIt <= iteraciones; posIt++){
            chunkFileSize = (posIt < iteraciones) ? CHUNCKSIZE : tam % CHUNCKSIZE;
            if (chunkFileSize > 0){

                Traza::print("Subiendo: " , (offsetForDropbox / (float) tam) *100, W_DEBUG);
                cabeceras.clear();
                cabeceras.insert( make_pair("Authorization", AuthOauth2));
                cabeceras.insert( make_pair("Accept", "*/*"));
                cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
                cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
                cabeceras.insert( make_pair("Content-Type", "text/plain"));
                cabeceras.insert( make_pair("Expect", ""));
                //cabeceras.insert( make_pair("Content-length", Constant::TipoToStr(chunkFileSize)));

                //cabeceras.insert( make_pair("Transfer-Encoding","chunked"));

                url = DROPBOXURLPUTCHUNKED + "?offset="
                        + Constant::TipoToStr(offsetForDropbox)
                        + ((!upId.empty()) ? ("&upload_id=" + upId) : "");

                util.put(url, filesystemPath.c_str(), chunkFileSize, offsetForDropbox, &cabeceras);
                Json::Value root;   // will contains the root value after parsing.
                Json::Reader reader;
                bool parsingSuccessful = reader.parse( util.getData(), root );
                if ( !parsingSuccessful ){
                     // report to the user the failure and their locations in the document.
                    Traza::print("Dropbox::chunckedUpload: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
                } else {
                    // Get the value of the member of root named 'hash', return '' if there is no
                    // such member.
                    upId = root.get("upload_id","").asString();
                    offsetForDropbox = Constant::strToTipo<size_t>(root.get("offset","").asString());
                }
            }
        }

        if (tam > 0){
            Traza::print( "Subiendo: ",
                    (offsetForDropbox / (float) tam) *100 , W_DEBUG);
        }
    } else {
        Traza::print("Upload aborted. Access token empty", W_ERROR);
    }

    return upId;
}

/**
*
*/
bool Dropbox::commitChunkedUpload(string dropboxPath, string accessToken, string upId){
    string url = DROPBOXURLCOMMITCHUNKED + Constant::uencodeUTF8(dropboxPath);
    string AuthOauth2 = "Bearer " + accessToken;
    map<string, string> cabeceras;
    string postData;


    if (!upId.empty()){
        cabeceras.clear();
        cabeceras.insert( make_pair("Authorization", AuthOauth2));
        cabeceras.insert( make_pair("Accept", "*/*"));
        cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
        cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
        cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
        cabeceras.insert( make_pair("Expect", ""));
        postData="overwrite=true&upload_id=" + upId;
        Traza::print( "Confirmando con la url: " + url + "?" + postData, W_DEBUG);
        bool res = util.post(url, postData.c_str(),postData.length(), &cabeceras);
        Traza::print("Resultado de la confirmacion: " + string(util.getData()), W_DEBUG);
        return res;
    } else {
        Traza::print("Commit aborted. Access token empty", W_ERROR);
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
    string postData;
    bool result = false;

    if (!fileid.empty()){
        cabeceras.clear();
        cabeceras.insert( make_pair("Authorization", AuthOauth2));
        cabeceras.insert( make_pair("Accept", "*/*"));
        cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
        cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
        cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
        cabeceras.insert( make_pair("Expect", ""));
        postData="root=auto&path=" + fileid;

        Traza::print("Ruta a eliminar: " + fileid, W_DEBUG);
        util.post(url, postData.c_str(),postData.length(), &cabeceras);

        Json::Value root;   // will contains the root value after parsing.
        Json::Reader reader;
        string resp = util.getData();
        Traza::print(resp, W_DEBUG);
        bool parsingSuccessful = reader.parse( resp, root );
        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            Traza::print("Dropbox::deleteFiles: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
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
    map<string, string> cabeceras;
    string postData;
    string url = DROPBOXURLGET + Constant::uencodeUTF8(cloudIdPath);
    string AuthOauth2 = "Bearer " + accessToken;
    size_t tam;

    cabeceras.clear();
    cabeceras.insert( make_pair("Authorization", AuthOauth2));
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "text/plain"));
    //util.get(url, &cabeceras);
    //util.writeToFile(filesystemPath.c_str());
    util.download(url, filesystemPath, &cabeceras);

    return 0;
}

/**
* Obtiene informacion de la ruta indicada
*/
string Dropbox::getJSONListDropbox(string filesystemPath, string accessToken){
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
            cabeceras.insert( make_pair("Expect", ""));
            url = DROPBOXURLMETA + Constant::uencodeUTF8(filesystemPath);
            Traza::print("Llamando a la url: " + url, W_DEBUG);
            util.get(url, &cabeceras);
            Traza::print("Bytes obtenidos", util.getDataLength(), W_DEBUG);
            return util.getData();
    }
    return responseMetadata;
}

/**
*
*/
bool Dropbox::listFiles(string filesystemPath, string accessToken, CloudFiles *files){
    string responseMetadata = getJSONListDropbox(filesystemPath, accessToken);

    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( responseMetadata, root );

    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("Dropbox::listFiles. Failed to parse configuration: " + reader.getFormattedErrorMessages(), W_ERROR);
    } else {
        // Get the value of the member of root named 'hash', return '' if there is no
        // such member.
        files->strHash = root.get("hash","").asString();
        files->bytes = root.get("bytes",0).asDouble();
        files->path = root.get("path","").asString();
        files->revision = root.get("rev","0").asString();
        files->root = root.get("root","").asString();
        files->isDir = root.get("is_dir", true).asBool();

        string out = "listFiles: " + files->strHash + ", " + Constant::TipoToStr(files->bytes)
                     + ", " + files->path + ", " + files->revision + ", "
                     + files->root + ", " + string(files->isDir ? "S":"N");

        Traza::print(out, W_DEBUG);

        const Json::Value contents = root["contents"];

        for ( int index = 0; index < contents.size(); index++ ){  // Iterates over the sequence elements.
            CloudFiles *childFile = new CloudFiles();
            childFile->revision = contents[index].get("rev","0").asString();
            childFile->strHash = contents[index].get("hash","").asString();
            childFile->bytes = contents[index].get("bytes",0).asDouble();
            childFile->path = contents[index].get("path","").asString();
            childFile->root = contents[index].get("root","").asString();
            childFile->isDir = contents[index].get("is_dir",false).asBool();
            files->fileList.push_back(childFile);

            Traza::print(childFile->strHash + ", " + Constant::TipoToStr(childFile->bytes)
             + ", " + childFile->path + ", " + childFile->revision + ", "
             + childFile->root + ", " + string(childFile->isDir ? "S":"N"), W_DEBUG);
        }
    }

    return true;
}
