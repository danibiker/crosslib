#include "darklyrics.h"

/**
*
*/
Darklyrics::Darklyrics(){
    setServiceName("www.darklyrics.com");
}

/**
*
*/
Darklyrics::~Darklyrics(){
}

/**
*
*/
uint32_t Darklyrics::trackLyrics(TrackInfo *trackinfo){
    HttpUtil util;
    if (util.download(trackinfo->url) && trackinfo->url.find("#") != string::npos){
        HtmlParser parser;
        string songanchor = trackinfo->url.substr(trackinfo->url.find("#") + 1);
        string datos = util.getData();
        trackinfo->lyrics_body = parser.searchString(&datos, "<a name=\"" + songanchor + "\">", "<h3>", false);
        if (trackinfo->lyrics_body.empty()){
            trackinfo->lyrics_body = parser.searchString(&datos, "<a name=\"" + songanchor + "\">", "<div", false);
        }

        size_t pos = trackinfo->lyrics_body.find("</h3>");
        if (pos != string::npos){
            trackinfo->lyrics_body = trackinfo->lyrics_body.substr(pos + 5);
            trackinfo->lyrics_body = Constant::replaceAll(trackinfo->lyrics_body, "<br />", "");
            return SINERROR;
        } else {
            return NOTFOUND;
        }
    }
    return ERRORCONNECT;
}


/**
*
*/
uint32_t Darklyrics::trackSearch(string track, string artist, vector <unique_ptr<TrackInfo>> *info){
    uint32_t retorno = ERRORCONNECT;
    //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
    if (info != NULL){
        info->clear();
        string url = "http://www.darklyrics.com/search?q=";
        string searchStr = artist + " " + track;
        url.append(Constant::uencodeUTF8(searchStr));

//        cout << url << endl;

        map<string, string> cabeceras;
        cabeceras.clear();
        cabeceras.insert( make_pair("Accept", "*/*"));
        cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
        cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
        cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
        util.get(url, &cabeceras);

        char* respuesta = util.getData();
        Traza::print("Darklyrics::trackSearch: response: " + string(respuesta), W_DEBUG);

        HtmlParser parser;
        parser.setObtainContentTag(true);
        Tags tag;
        tag.htmlTag = GUMBO_TAG_A;
        parser.buscarElementos(respuesta, &tag);
        string href, content;
        map<string, string> *v;
        map<string, string> elements;
        string enlaceEncontrado;

        //Se busca en la pagina todos los campos href y se recoge el valor de cada uno
        int posEnlace = -1;
        int lastNWords = 0;
        int nWords = 0;
        for (unsigned int i=0; i < tag.tagElement.size(); i++){
            v = &tag.tagElement.at(i).attrList;
            href = (v->find("href") !=  v->end()) ? v->find("href")->second : "";
            nWords = Constant::contarPalabrasIguales(searchStr, tag.tagElement.at(i).content);
            if (nWords > lastNWords){
                posEnlace = i;
                lastNWords = nWords;
            }
        }

        if (posEnlace != -1){
            v = &tag.tagElement.at(posEnlace).attrList;
            href = (v->find("href") !=  v->end()) ? v->find("href")->second : "";
            auto trackElem = make_unique<TrackInfo>();
            trackElem->url = "http://www.darklyrics.com/" + href;
            info->push_back(std::move(trackElem));
            retorno = SINERROR;
        } else {
            retorno = NOTFOUND;
        }
    }
    return retorno;
}

