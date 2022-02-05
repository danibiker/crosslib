#ifndef LYRICSBASE_H
#define LYRICSBASE_H

#include "util/ConstantHttp.h"
#include <Constant.h>
#include "httputil.h"
#include "htmlparser.h"

class LyricsBase
{
    public:
        /** Default constructor */
        LyricsBase();
        /** Default destructor */
        virtual ~LyricsBase();

        virtual uint32_t trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info);
        virtual uint32_t trackLyrics(TrackInfo *trackinfo);
        void setTrack(string var){track = var;}
        void setArtist(string var){artist = var;}
        string getServiceName(){return serviceName;}
        void setServiceName(string var){serviceName = var;}

    protected:
        HttpUtil util;
        string track;
        string artist;
        vector <TrackInfo *> info;

    private:
        string serviceName;

};

#endif // LYRICSBASE_H
