#ifndef LYRICSWIKIA_H
#define LYRICSWIKIA_H

#include <json/json.h>
#include "lyricsbase.h"
#include "../util/JsonParser.h"


using namespace std;

class LyricsWikia : public LyricsBase
{
    public:
        LyricsWikia();
        virtual ~LyricsWikia();
        uint32_t trackLyrics(TrackInfo *trackinfo);
        uint32_t trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info);

    protected:

    private:

};

#endif // LYRICSWIKIA_H
