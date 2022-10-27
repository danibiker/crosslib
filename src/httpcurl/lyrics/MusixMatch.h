#ifndef MUSIXMATCH_H
#define MUSIXMATCH_H

#include "httpcurl/httputil.h"
#include "json/json.h"
#include "httpcurl/util/ConstantHttp.h"
#include "../util/JsonParser.h"
#include "lyricsbase.h"

static const string APIKEY = "47a903c05793baee56e17dc4b14c6b19";

class MusixMatch : public LyricsBase
{
    public:
        MusixMatch();
        virtual ~MusixMatch();
        uint32_t trackLyrics(TrackInfo *trackinfo);
        uint32_t trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info);

    protected:

    private:
        HttpUtil util;
};

#endif // MUSIXMATCH_H
