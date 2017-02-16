#include "lyricsbase.h"

LyricsBase::LyricsBase()
{
    setServiceName("");
}

LyricsBase::~LyricsBase()
{
    //dtor
}

DWORD LyricsBase::trackSearch(string track, string artist, vector <TrackInfo *> *info){
    return NOTFOUND;
}

DWORD LyricsBase::trackLyrics(TrackInfo *trackinfo){
    return NOTFOUND;
}
