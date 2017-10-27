#ifndef DROPBOX_H
#define DROPBOX_H

#include <iostream>
#ifdef WIN
    #include <windows.h>
#endif // WIN
#include "util/ConstantHttp.h"
#include <vector>
#include <map>
#include "httputil.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <Constant.h>
#include "json/json.h"
#include <vector>
#include "Dirutil.h"
#include "thread.h"
#include "ioauth2.h"

using namespace std;

static const string DROPBOXACCESSTOKENSTR = "accesstoken";

class Dropbox : public IOauth2
{
    public:
        /** Default constructor */
        Dropbox();
        Dropbox(IOauth2 *var) : IOauth2(var){};

        /** Default destructor */
        virtual ~Dropbox();


        /**Metodos comunes de la interfaz entre google y dropbox*/
        uint32_t authenticate();
        void launchAuthorize(string clientid);
        string launchAccessToken(string clientid, string secret, string code, bool refresh);
        bool deleteFiles(string fileid, string accessToken);
        string chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken);
        string storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh);
        bool listFiles(string filesystemPath, string accessToken, CloudFiles *files);
        int getFile(string filesystemPath, string cloudIdPath, string accessToken);

    protected:

    private:
        /**Metodos propios para dropbox*/
        bool commitChunkedUpload(string dropboxPath, string accessToken, string upId, size_t offset);
        string getJSONListDropbox(string filesystemPath, string accessToken);
        string getJSONListContinueDropbox(string cursor, string accessToken);
        int putFile(string filesystemPath, string dropboxPath, string accessToken);
};



#endif // DROPBOX_H
