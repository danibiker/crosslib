#include "ioauth2.h"

/**
*
*/
IOauth2::IOauth2(){
    rutaIni = Constant::getAppDir() + Constant::getFileSep() + TOKENFILENAME;
    oauthStatus = ERRORACCESSTOKEN;
}


IOauth2::IOauth2(IOauth2 *var){
    IOauth2();
    setProperties(var);
}


/**
*
*/
IOauth2::~IOauth2(){
}

/**
*
*/
uint32_t IOauth2::authenticate(){
    return 0;
}

/**
*
*/
void IOauth2::launchAuthorize(string clientid){

}

/**
*
*/
string IOauth2::launchAccessToken(string clientid, string secret, string code, bool refresh){
    return "";
}

/**
*
*/
bool IOauth2::deleteFiles(string fileid, string accessToken){
    return false;
}

/**
*
*/
void IOauth2::abortDownload(){
    util.abortDownload();
}

/**
*
*/
bool IOauth2::chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken){
    return false;
}

/**
*
*/
//string IOauth2::storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
//    return "";
//}

string IOauth2::storeAccessToken(string codeOrRefreshToken, bool refresh){
    return "";
}

string IOauth2::storeToken(string configParmAccessToken, string configParmRefreshtoken, string codeOrRefreshToken, bool refresh){
    filecipher cifrador;
    Traza::print("Negociando access token...", W_DEBUG);
    launchAccessToken(this->getClientid(), this->getSecret(), (refresh) ? this->getRefreshToken() : codeOrRefreshToken, refresh);
    
    if (!this->accessToken.empty()){
        string accessTokenCipherB64 = cifrador.encodeEasy(this->getAccessToken(), passwordAT);
        string refreshTokenCipherB64 = cifrador.encodeEasy(this->getRefreshToken(), passwordAT);
        
        ListaIni<Data> *config = new ListaIni<Data>();
        try{
            Dirutil dir;
            if (dir.existe(rutaIni)){
                config->loadFromFile(rutaIni);
                config->sort();
            }
            if (!this->getAccessToken().empty()){
                this->addToken(configParmAccessToken, accessTokenCipherB64, config);
            }
            if (!this->getRefreshToken().empty()){
                this->addToken(configParmRefreshtoken, refreshTokenCipherB64, config);
            }
            config->writeToFile(rutaIni);

        } catch (Excepcion &e){
            Traza::print("IOauth2::storeToken. Error al cargar la configuracion", W_ERROR);
        }
    }
    return this->getAccessToken();
}

/**
*
*/
int IOauth2::getFile(string filesystemPath, string cloudIdPath, string accessToken){
    return 0;
}

/**
*
*/
bool IOauth2::listFiles(string fileid, string accessToken, CloudFiles *files){
    return true;
}

string IOauth2::fileExist(string filename, string parentid, string accessToken){
    return "";
}

/**
*
*/
void IOauth2::setProperties(IOauth2 *var){
    this->setAccessToken(var->getAccessToken());
    this->setRefreshToken(var->getRefreshToken());
    this->setClientid(var->getClientid());
    this->setSecret(var->getSecret());
}

void IOauth2::addToken(string parmName, string parmValue, ListaIni<Data> *config){
    Data dato;
    int pos = config->find(parmName);
    if (pos != -1){
        dato.setKeyValue(parmName, parmValue);
        //Para simular expiracion de token
        //dato.setKeyValue(parmName, "lqWlOI+9lnvDL0sK1IulkhXV+IKEWHHw4oJCDMQGlGDUGdh8ureDDE4DKH4rdKP56j2ZAjtWij9r9Yn194ze4nGpfw1jTG4j2z3qSgEQkww=");
        config->set(pos, &dato);
    } else {
        dato.setKeyValue(parmName, parmValue);
        config->add(dato);
    }
}

/**
 * 
 * @param accessToken
 * @param refreshToken
 * @param configAccessTokenName
 * @param configRefreshTokenName
 * @return 
 */
uint32_t IOauth2::decodeTokens(string &accessToken, string &refreshToken, string configAccessTokenName, string configRefreshTokenName){
    
    uint32_t ret = ERRORACCESSTOKEN;
    filecipher cifrador;
    string accessTokenCipher;
    string refreshTokenCipher;
    Dirutil dir;
    
    if (dir.existe(rutaIni)){
        //Obtenemos el access token almacenado en el fichero de configuracion
        ListaIni<Data> *config = new ListaIni<Data>();
        config->loadFromFile(rutaIni);
        config->sort();

        int pos = config->find(configAccessTokenName);
        Data elem = config->get(pos);
        accessTokenCipher = elem.getValue();
        accessToken = cifrador.decodeEasy(accessTokenCipher, passwordAT);

        pos = config->find(configRefreshTokenName);
        if (pos >= 0){
            elem = config->get(pos);
            refreshTokenCipher = elem.getValue();
            refreshToken = cifrador.decodeEasy(refreshTokenCipher, passwordAT);
        }

        if (accessToken.empty() || refreshToken.empty()){
            Traza::print("Dropbox::authenticate. No se lanza autorizacion porque no se ha encontrado token almacenado", W_ERROR);
            ret = ERRORACCESSTOKEN;
            setOauthStatus(ERRORACCESSTOKEN);
        } else {
            ret = SINERROR;
        }
        delete config;
    }
    return ret;
}

/**
 * 
 * @param url
 */
void IOauth2::openExplorer(string url){
        if (!url.empty()){
#ifdef WIN
            char infoBuf[MAX_PATH];
            GetWindowsDirectory( infoBuf, MAX_PATH );

            Launcher lanzador;
            FileLaunch emulInfo;
            emulInfo.rutaexe = infoBuf;
            emulInfo.fileexe = "explorer.exe";
            emulInfo.parmsexe = url;
            bool resultado = lanzador.lanzarProgramaUNIXFork(&emulInfo);
#endif

#ifdef UNIX
            
//            string cmd = CMD_LAUNCH_BROWSER + " \"" + url + "\"";
//            system(cmd.c_str());
            Launcher lanzador;
            FileLaunch emulInfo;
            emulInfo.rutaexe = "/usr/bin";
            emulInfo.fileexe = CMD_LAUNCH_BROWSER;
            emulInfo.parmsexe = url;
            bool resultado = lanzador.lanzarProgramaUNIXFork(&emulInfo);
#endif            
        }
}
