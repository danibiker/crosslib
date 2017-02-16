#ifndef DARKLYRICS_H
#define DARKLYRICS_H

#include "htmlparser.h"
#include "lyricsbase.h"

class Darklyrics : public LyricsBase
{
    public:
        Darklyrics();
        virtual ~Darklyrics();

        DWORD trackSearch(string track, string artist, vector <TrackInfo *> *info);
        DWORD trackLyrics(TrackInfo *trackinfo);
        string search_for_elem(GumboNode* node, Tags *tag, string atributo, string valAtributo) ;

    protected:

    private:
};

#endif // DARKLYRICS_H
