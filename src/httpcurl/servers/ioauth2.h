#ifndef IOAUTH2_H
#define IOAUTH2_H

#include "uiobjects/Constant.h"
#include "httpcurl/httputil2.h"
#include "uiobjects/Dirutil.h"
#include "httpcurl/util/ConstantHttp.h"
#include "json/json.h"
#include "uiobjects/listaIni.h"
#include "uiobjects/Launcher.h"
#include "uiobjects/filelaunch.h"
#include "../util/JsonParser.h"
#include "rijndael/filecipher.h"



static const string passwordAT = "klYcZaKgGWO36d1";

class IOauth2
{
    public:
        /** Default constructor */
        IOauth2();
        /** Default destructor */
        virtual ~IOauth2();

        IOauth2(IOauth2 *var);

        /**Metodos que deben ser implementados en cada servicio*/
        virtual uint32_t authenticate();
        virtual void launchAuthorize(string clientid);
        virtual string launchAccessToken(string clientid, string secret, string code, bool refresh);
        virtual bool deleteFiles(string fileid, string accessToken);
//        virtual string storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh);
        virtual string storeAccessToken(string codeOrRefreshToken, bool refresh);
        virtual bool chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken);
        virtual bool listFiles(string fileid, string accessToken, CloudFiles *files);
        virtual int getFile(string filesystemPath, string cloudIdPath, string accessToken);
        virtual string fileExist(string filename, string parentid, string accessToken);


        /**Metodos comunes que todos heredan*/
        void abortDownload();
        void setProperties(IOauth2 *var);
        void addToken(string parmName, string parmValue, ListaIni<Data> *config);
        string storeToken(string configParmAccessToken, string configParmRefreshtoken, string codeOrRefreshToken, bool refresh);


        void setClientid(string clientid) { this->clientid = clientid; }
        string getClientid() { return this->clientid; }
        void setAccessToken(string accessToken) { this->accessToken = accessToken; }
        string getAccessToken() { return this->accessToken; }
        void setSecret(string secret) { this->secret = secret; }
        string getSecret() { return this->secret; }
        void setRefreshToken(string refreshToken) { this->refreshToken = refreshToken; }
        string getRefreshToken() { return this->refreshToken; }

        uint32_t getOauthStatus(){return oauthStatus;}
        void setOauthStatus(uint32_t var){oauthStatus = var;}
        void openExplorer(string);

    protected:
        Httputil2 util;
        string rutaIni;
        string clientid;
        string accessToken;
        string refreshToken;
        string tokenType;
        string expiresIn;
        string secret;
        uint32_t oauthStatus;

        uint32_t decodeTokens(string &accessToken, string &refreshToken, string configAccessTokenName, string configRefreshTokenName);

    private:
};

#endif // IOAUTH2_H
