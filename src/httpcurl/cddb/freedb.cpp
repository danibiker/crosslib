#include "freedb.h"

Freedb::Freedb()
{
    this->query = NULL;
    this->cdTrackList = NULL;
    //ctor
}

Freedb::~Freedb()
{
    //dtor
}

uint32_t Freedb::searchCd(){
    if (this->query != NULL && this->cdTrackList != NULL)
        searchCd();
    return 0;
}

/**
*
*/
int Freedb::searchCd(FreedbQuery *query, vector<CdTrackInfo *> *cdTrackList){
    HttpUtil util;
    string postData;
    int code = 0;

    if (query->offsets.size() > 0){
        postData = "cmd=cddb+query+"+query->discId+"+"+Constant::TipoToStr(query->offsets.size());
        for (int i=0; i < query->offsets.size(); i++){
            postData.append("+"+Constant::TipoToStr(query->offsets.at(i)));
        }
        postData.append("+"+Constant::TipoToStr(query->totalSeconds));
        postData.append("&hello=" +query->username+"+"+query->hostname+"+"+query->clientname+"+"+query->version);
        postData.append("&proto=" + Constant::TipoToStr(FREEDBPROTO));

        map<string, string> v_post;
        v_post.insert( make_pair("Referer", FREEDBURL));
        Traza::print("Enviando: " + postData, W_DEBUG);
        util.post(FREEDBURL, postData, &v_post);
        Traza::print(util.getData(), W_DEBUG);

        int i = 0;
        std::istringstream f(util.getData());
        std::string line;

        while (std::getline(f, line)) {
            if (line.at(0) != '#'){
                if (i==0){
                    code = Constant::strToTipo<int>(line.substr(0,3));
                    if (code == 200){
                        CdTrackInfo *cdTrack = new CdTrackInfo();
                        line.erase(line.find_last_not_of(" \t\f\v\n\r")+1);
                        line = line.find(" ") != string::npos ? line.substr(line.find(" ") + 1) : "";
                        cdTrack->genre = line.substr(0, line.find(" "));
                        line = line.find(" ") != string::npos ? line.substr(line.find(" ") + 1) : "";
                        cdTrack->discId = line.substr(0, line.find(" "));
                        line = line.find(" ") != string::npos ? line.substr(line.find(" ") + 1) : "";
                        cdTrack->albumName = line;
                        cdTrackList->push_back(cdTrack);
                    }

                } else {
                    if (line.compare(".") != 0 && line.find(" ") != string::npos){
                        line.erase(line.find_last_not_of(" \t\f\v\n\r")+1);
                        CdTrackInfo *cdTrack = new CdTrackInfo();
                        cdTrack->genre = line.substr(0, line.find(" "));
                        line = line.find(" ") != string::npos ? line.substr(line.find(" ") + 1) : "";
                        cdTrack->discId = line.substr(0, line.find(" "));
                        line = line.find(" ") != string::npos ? line.substr(line.find(" ") + 1) : "";
                        cdTrack->albumName = line;
                        cdTrackList->push_back(cdTrack);
                    }
                }
            }
            i++;
        }

    }
    return code;
}

/**
*
*/
int Freedb::getCdInfo(FreedbQuery *query, CdTrackInfo *cdTrack){
    HttpUtil util;

    string postData = "cmd=cddb+read+"+query->categ+"+"+query->discId;
    postData.append("&hello=" +query->username+"+"+query->hostname+"+"+query->clientname+"+"+query->version);
    postData.append("&proto=" + Constant::TipoToStr(FREEDBPROTO));

    map<string, string> v_post;
    v_post.insert( make_pair("Referer", FREEDBURL));
    Traza::print("Enviando: " + postData, W_DEBUG);
    util.post(FREEDBURL, postData, &v_post);
    Traza::print(util.getData(), W_DEBUG);

    int i = 0;
    std::istringstream f(util.getData());
    std::string line;
    int code = 0;
    string value;

    while (std::getline(f, line)) {
        if (line.at(0) != '#'){
            if (i==0){
                code = Constant::strToTipo<int>(line.substr(0,3));
            } else {
//                Traza::print(line, W_DEBUG);
                line.erase(line.find_last_not_of(" \t\f\v\n\r")+1);
                value = line.substr(line.find("=") + 1);
                if (line.find("DISCID") != string::npos)
                    cdTrack->discId = value;
                else if (line.find("DTITLE") != string::npos)
                    cdTrack->albumName = value;
                else if (line.find("DYEAR") != string::npos)
                    cdTrack->year = value;
                else if (line.find("DGENRE") != string::npos)
                    cdTrack->genre = value;
                else if (line.find("TTITLE") != string::npos)
                    cdTrack->titleList.push_back(Constant::toAnsiString(value));
            }
        }
        i++;
    }
    return code;
}
