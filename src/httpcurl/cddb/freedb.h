#ifndef FREEDB_H
#define FREEDB_H

#include "../httputil.h"
#include "../htmlparser.h"
#include "util/ConstantHttp.h"

class CdTrackInfo{
    public:
        CdTrackInfo(){
        }
        string albumName;
        string discId;
        string year;
        string genre;
        vector <string> titleList;
};

struct FreedbQuery{
    string discId;
    string categ;
    string username;
    string hostname;
    string clientname;
    string version;
    vector <DWORD> offsets;
    DWORD totalSeconds;
};

static const int FREEDBPROTO = 6;
static const string FREEDBURL = "http://freedb.freedb.org/~cddb/cddb.cgi";

class Freedb
{
    public:
        Freedb();
        virtual ~Freedb();
        int searchCd(FreedbQuery *query, vector<CdTrackInfo *> *cdTrackList);
        int getCdInfo(FreedbQuery *query, CdTrackInfo *cdTrack);

        DWORD searchCd();
        vector<CdTrackInfo *> * getCdTrackList(){return this->cdTrackList;}
        void setCdTrackList(vector<CdTrackInfo *> *cdTrackList){this->cdTrackList = cdTrackList;}
        void setQuery (FreedbQuery *query){this->query = query;}
    protected:

    private:
        vector<CdTrackInfo *> *cdTrackList;
        FreedbQuery *query;
};

#endif // FREEDB_H
