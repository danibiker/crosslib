#ifndef DARKLYRICS_H
#define DARKLYRICS_H

#include "htmlparser.h"
#include "lyricsbase.h"

class Darklyrics : public LyricsBase
{
    public:
        Darklyrics();
        virtual ~Darklyrics();

        uint32_t trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info);
        uint32_t trackLyrics(TrackInfo *trackinfo);
        string search_for_elem(GumboNode* node, Tags *tag, string atributo, string valAtributo) ;

    protected:

    private:
};

#endif // DARKLYRICS_H
