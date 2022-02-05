/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChartLyrics.cpp
 * Author: Dani
 * 
 * Created on 4 de febrero de 2022, 17:47
 */

#include "chartLyrics.h"

ChartLyrics::ChartLyrics() {
}

ChartLyrics::ChartLyrics(const ChartLyrics& orig) {
}

ChartLyrics::~ChartLyrics() {
}

/**
*
*/
uint32_t ChartLyrics::trackLyrics(TrackInfo *trackinfo){
    uint32_t retorno = SINERROR;
    return retorno;
}

std::string ChartLyrics::getTagName(const char* data) {

    int len = strlen(data);
    int i=0;
    int ini = 0;
    int fin = 0;
    string tagName = "";
    char tmp[2] = {' ','\0'};

    while(i < len && fin == 0){
        if (data[i] == '<')
            ini = i;
        else if (data[i] == '>')
            fin = i;
        else {
            tmp[0] = data[i];
            tagName.append(tmp);
        }
        i++;
    }
    return tagName;
}

std::string ChartLyrics::getNodeInfo(GumboNode* node, unique_ptr<TrackInfo> *elemInfo) {
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        string nombreTag = "";
        if (node->v.element.tag == GUMBO_TAG_UNKNOWN){
            nombreTag = getTagName(node->v.element.original_tag.data);
        }
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = getNodeInfo((GumboNode*) children->data[i], elemInfo);
            if (nombreTag.compare("Lyric") == 0){
                Traza::print("ChartLyrics::Lyric. text: " + text, W_DEBUG);
                elemInfo->get()->lyrics_body = text;
            }
        }
    }
    return "";
}

/**
*
*/
uint32_t ChartLyrics::trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info){
    uint32_t retorno = NOTFOUND;
    //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
    string url = "http://api.chartlyrics.com/apiv1.asmx/SearchLyricDirect?";
    url.append("artist=");
    url.append(Constant::uencodeUTF8(artist));
    url.append("&song=");
    url.append(Constant::uencodeUTF8(track));

    map<string, string> cabeceras;
    cabeceras.insert( make_pair("Accept", "*/*"));
    cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
    cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
    cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
    
    Httputil2 util2;
    MemoryStruct *chunk = util2.initDownload();
    Traza::print("ChartLyrics::trackSearch. url: " + url, W_DEBUG);
    util2.httpGet(url, &cabeceras, chunk);
    string respuesta = util2.getRawData(chunk);
    util2.endDownload(&chunk);
    
    Traza::print("ChartLyrics::trackSearch. Respuesta: " + respuesta, W_DEBUG);
    
    map<string, unique_ptr<vector<string>> > output;
    XmlParser::parseString(respuesta, output);
    
    auto trackElem = make_unique<TrackInfo>();
    trackElem->urlInfo = XmlParser::getFirst("LyricUrl", output);
    trackElem->album_coverart_100x100 = XmlParser::getFirst("LyricCovertArtUrl", output);
    trackElem->album_coverart_350x350 = trackElem->album_coverart_100x100;
    trackElem->trackId = XmlParser::getFirst("LyricId", output);
    trackElem->track_name = XmlParser::getFirst("LyricSong", output);
    trackElem->lyrics_body = XmlParser::getFirst("Lyric", output);
    
    if (!trackElem->lyrics_body.empty()){
        retorno = SINERROR;
        Traza::print("ChartLyrics::LyricUrl: " + trackElem->lyrics_body, W_DEBUG);
        info->push_back(std::move(trackElem));
    }
    
//    auto trackElem = make_unique<TrackInfo>();
//    GumboOutput* output = gumbo_parse(respuesta.c_str());
//    if (output != NULL){
//        getNodeInfo(output->root, &trackElem);
//        retorno = SINERROR;
//    }
    
    
//    TiXmlDocument doc;
//    const char* parseResult = doc.Parse(respuesta.c_str(), 0 , TIXML_ENCODING_UTF8);
//    if(parseResult == NULL && doc.Error()){
//        return retorno;
//    }
//
//    TiXmlElement* root = doc.FirstChildElement();
//    if(root == NULL){
//        Traza::print("ChartLyrics::Failed to load file: No root element.", W_DEBUG);
//        return ERRORCONNECT;
//    }
//    auto trackElem = make_unique<TrackInfo>();
//    TiXmlText* text;
//    for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()){
//        string elemName = elem->Value();
//        if (elemName.compare("LyricId") == 0){
//            text = elem->FirstChild()->ToText();
//            trackElem->trackId = text->Value();
//        } else if (elemName.compare("LyricCovertArtUrl") == 0){
//            text = elem->FirstChild()->ToText();
//            trackElem->album_coverart_100x100 = text->Value();
//            trackElem->album_coverart_350x350 = text->Value();
//        } else if (elemName.compare("Lyric") == 0){
//            text = elem->FirstChild()->ToText();
//            trackElem->lyrics_body = text->Value();
//        } else if (elemName.compare("LyricSong") == 0){
//            text = elem->FirstChild()->ToText();
//            trackElem->track_name = text->Value();
//        } else if (elemName.compare("LyricUrl") == 0){
//            text = elem->FirstChild()->ToText();
//            trackElem->urlInfo = text->Value();
//        }
//        retorno = SINERROR;
//    }
//    Traza::print("ChartLyrics::trackSearch. lyrics_body: " + trackElem->lyrics_body, W_DEBUG);
//    info->push_back(std::move(trackElem));
    
    return retorno;
}

