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
DWORD IOauth2::authenticate(){
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
    util.abort();
}

/**
*
*/
string IOauth2::chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken){
    return "";
}

/**
*
*/
string IOauth2::storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh){
    return "";
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
