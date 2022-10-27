/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   onedrive.h
 * Author: Timonet
 *
 * Created on 28 de octubre de 2017, 14:43
 */

#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include <iostream>
#include "httpcurl/util/ConstantHttp.h"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "uiobjects/Constant.h"
#include "json/json.h"
#include <vector>
#include "uiobjects/Dirutil.h"
#include "uiobjects/thread.h"
#include "httpcurl/servers/ioauth2.h"
#include "../util/JsonParser.h"

using namespace std;

static const int ONEDRIVECHUNK = 3276800; //DEBE ser un multiplo de 320 KiB (327 680 bytes). Le ponemos 3200KiB
static const string ONEDRIVEACCESSTOKENSTR = "onedriveaccesstoken";
static const string ONEDRIVEFRESHTOKENSTR = "onedriverefreshtoken";

class Onedrive : public IOauth2{
public:
    Onedrive();
    Onedrive(IOauth2 *var) : IOauth2(var){};
    Onedrive(const Onedrive& orig);
    virtual ~Onedrive();

    /**Metodos comunes de la interfaz entre google y dropbox*/
    uint32_t authenticate();
    void launchAuthorize(string clientid);
    string launchAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh);
    bool deleteFiles(string fileid, string accessToken);
    bool chunckedUpload(string filesystemPath, string cloudIdPath, string accessToken);
//    string storeAccessToken(string clientid, string secret, string codeOrRefreshToken, bool refresh);
    string storeAccessToken(string codeOrRefreshToken, bool refresh);
    bool listFiles(string filesystemPath, string accessToken, CloudFiles *files);
    int getFile(string filesystemPath, string cloudIdPath, string accessToken);
    string fileExist(string filename, string parentid, string accessToken);

    string getJSONListSharepoint(string fileid, string accessToken);


    string mkdir(string dirname, string parentid, string accessToken);

    int getShared(string accessToken);

private:
    uint32_t checkOauthErrors(string data, Json::Value *root);
    string getJSONList(string fileid, string accessToken, string nextPageToken);
    bool resumableChunckedUpload(string filesystemPath, string url, size_t offset, size_t tam, string accessToken);
    bool isOneDriveId(string path);
};

#endif /* ONEDRIVE_H */

