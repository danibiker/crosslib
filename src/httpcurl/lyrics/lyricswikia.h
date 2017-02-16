#ifndef LYRICSWIKIA_H
#define LYRICSWIKIA_H

#include "json/json.h"
#include "lyricsbase.h"


using namespace std;

class LyricsWikia : public LyricsBase
{
    public:
        LyricsWikia();
        virtual ~LyricsWikia();
        DWORD trackLyrics(TrackInfo *trackinfo);
        DWORD trackSearch(string track, string artist, vector <TrackInfo *> *info);

    protected:

    private:

};

#endif // LYRICSWIKIA_H
