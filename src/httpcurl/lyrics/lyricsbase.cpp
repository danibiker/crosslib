#include "lyricsbase.h"

LyricsBase::LyricsBase()
{
    setServiceName("");
}

LyricsBase::~LyricsBase()
{
    //dtor
}

uint32_t LyricsBase::trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info){
    return NOTFOUND;
}

uint32_t LyricsBase::trackLyrics(TrackInfo *trackinfo){
    return NOTFOUND;
}
