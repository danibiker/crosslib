#ifndef MUSIXMATCH_H
#define MUSIXMATCH_H

#include "httputil.h"
#include "json/json.h"
#include "util/ConstantHttp.h"

static const string APIKEY = "47a903c05793baee56e17dc4b14c6b19";

class MusixMatch
{
    public:
        MusixMatch();
        virtual ~MusixMatch();
        DWORD trackLyrics(TrackInfo *trackinfo);
        DWORD trackSearch(string track, string artist, vector <TrackInfo *> *info);

    protected:

    private:
        HttpUtil util;
};

#endif // MUSIXMATCH_H
