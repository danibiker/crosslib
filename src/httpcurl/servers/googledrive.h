#ifndef GOOGLEDRIVE_H
#define GOOGLEDRIVE_H

#include "ioauth2.h"

static const int GOOGLECHUNK = 262144;
static const string GOOGLEACCESSTOKENSTR = "googleaccesstoken";
static const string GOOGLEREFRESHTOKENSTR = "googlerefreshtoken";

class GoogleDrive : public IOauth2
{
    public:
        /** Default constructor */
        GoogleDrive();
        GoogleDrive(IOauth2 *var) : IOauth2(var){};

        /** Default destructor */
        virtual ~GoogleDrive();

        /**Metodos comunes de la interfaz entre google y dropbox*/
        uint32_t authenticate();
        void launchAuthorize(string clientid);
        string launchAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh);
        bool deleteFiles(string fileid, string accessToken);
        string storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh);
        string chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken);
        bool listFiles(string fileid, string accessToken, CloudFiles *files);
        int getFile(string filesystemPath, string cloudIdPath, string accessToken);

        /**Metodos propios para google*/
        string mkdir(string dirname, string parentid, string accessToken);
        string fileExist(string filename, string parentid, string accessToken);

    protected:
        long resumableChunckedUpload(string filesystemPath, string url, size_t pos, size_t tam, string accessToken);
        string getJSONList(string fileid, string accessToken, string nextPageToken);
        uint32_t checkOauthErrors(string data, Json::Value *root);



    private:
};

#endif // GOOGLEDRIVE_H
