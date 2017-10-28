#ifndef CONSTANT_HTTP_H_INCLUDED
#define CONSTANT_HTTP_H_INCLUDED

#include <sstream>
#include <string.h>
#include <vector>
#include <typeinfo>
#include <ctime>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <algorithm>  //transform
#include <cctype>
#include <iomanip>

#ifdef WIN
    #include <windows.h>
#endif // WIN

using namespace std;

static const char* DIAS[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char* MESES[] = {"Jan" , "Feb" , "Mar" , "Apr" , "May" , "Jun" , "Jul" , "Aug" , "Sep" , "Oct" , "Nov" , "Dec" };
static const char* METACHARS[] = {" ", "*", "?", "[", "]", "'","\"" ,"$", ";", "&", "(", ")", "|", "^", "<", ">"};

static const string DROPBOXACCOUNTINFO = "https://api.dropboxapi.com/2/users/get_current_account";
static const string DROPBOXURLAUTH = "https://www.dropbox.com/oauth2/authorize";
static const string DROPBOXURLTOKEN = "https://api.dropboxapi.com/oauth2/token";
static const string DROPBOXURLPUT = "https://content.dropboxapi.com/1/files_put/auto/";
static const string DROPBOXURLPUTSTART = "https://content.dropboxapi.com/2/files/upload_session/start";
static const string DROPBOXURLPUTCHUNKED = "https://content.dropboxapi.com/2/files/upload_session/append_v2";
static const string DROPBOXURLCOMMITCHUNKED = "https://content.dropboxapi.com/2/files/upload_session/finish";
static const string DROPBOXURLDELETE = "https://api.dropboxapi.com/2/files/delete";
static const string DROPBOXURLGET = "https://content.dropboxapi.com/2/files/download";
static const string DROPBOXURLLIST = "https://api.dropboxapi.com/2/files/list_folder";
static const string DROPBOXURLLISTNEXT = "https://api.dropboxapi.com/2/files/list_folder/continue";


//https://accounts.google.com/o/oauth2/auth
static const string GOOGLEURLAUTH =  "https://accounts.google.com/o/oauth2/auth";
static const string GOOGLEURLTOKEN = "https://accounts.google.com/o/oauth2/token";
static const string GOOGLEURLPUT =   "https://www.googleapis.com/upload/drive/v3/files";
static const string GOOGLEURLMKDIR = "https://www.googleapis.com/drive/v3/files";
static const string GOOGLEURLLIST =  "https://www.googleapis.com/drive/v3/files";
static const string GOOGLEURLGET =   "https://www.googleapis.com/drive/v3/files/";
static const string GOOGLEURLLISTCHILDREN = "https://www.googleapis.com/drive/v2/files"; //folderId/children
static const string GOOGLEURLDELETE = "https://www.googleapis.com/drive/v3/files/";


static const size_t CHUNCKSIZE = 5000 * 1024; //5000KB

typedef enum {HTTP_POST, HTTP_GET, HTTP_PUT, HTTP_DELETE, HTTP_POST2, HTTP_POST3} petHttp;

class TrackInfo{
    public:
        TrackInfo(){
        }
        string album_name;
        string trackId;
        string album_coverart_100x100;
        string album_coverart_350x350;
        string track_name;
        string lyrics_body;
        string url;
        string urlInfo;
};

typedef enum { SINERROR,
        ERRORACCESSTOKEN,
        ERRORREFRESHTOKEN,
        ERRORCONNECT,
        NOTFOUND,
        MAXERRORDRIVE
} erroresConnect;

class CloudFiles{
    public:
        CloudFiles(){
            bytes = 0;
            isDir = false;
        }

        void clear(){
            strHash = "";
            bytes = 0;
            path = "";
            isDir = false;
            root = "";
            revision = "";
            fileList.clear();
        }

        string strHash;
        size_t bytes;
        string path;
        bool isDir;
        string root;
        string revision;
        vector<CloudFiles *> fileList;


};

#endif // CONSTANT_HTTP_H_INCLUDED
