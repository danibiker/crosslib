/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ChartLyrics.h
 * Author: Dani
 *
 * Created on 4 de febrero de 2022, 17:47
 */

#ifndef CHARTLYRICS_H
#define CHARTLYRICS_H

#include "httputil.h"
#include "httputil2.h"
#include "util/ConstantHttp.h"
#include "lyricsbase.h"
#include <gumbo.h>
#include "util/XmlParser.h"

#define TIXML_USE_STL
#include <tinyxml.h>


class ChartLyrics : public LyricsBase{
public:
    ChartLyrics();
    ChartLyrics(const ChartLyrics& orig);
    virtual ~ChartLyrics();

    uint32_t trackLyrics(TrackInfo *trackinfo);
    uint32_t trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info);

private:
    std::string getTagName(const char* data);
    std::string getNodeInfo(GumboNode* node, unique_ptr<TrackInfo> *elemInfo);

};

#endif /* CHARTLYRICS_H */

