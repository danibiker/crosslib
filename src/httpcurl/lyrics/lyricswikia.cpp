#include "lyricswikia.h"

/**
*
*/
LyricsWikia::LyricsWikia(){
    setServiceName("lyrics.wikia.com");
}

/**
*
*/
LyricsWikia::~LyricsWikia(){
}

/**
*
*/
DWORD LyricsWikia::trackLyrics(TrackInfo *trackinfo){
    HttpUtil util;
    trackinfo->lyrics_body = "";
    trackinfo->urlInfo = "";

    if (util.download(trackinfo->url)){
        HtmlParser parser;
        Tags tag;
        tag.htmlTag = GUMBO_TAG_DIV;
        trackinfo->lyrics_body = parser.buscarElem(util.getData(), &tag, "class", "lyricbox");

        if (trackinfo->lyrics_body.empty()){
            return NOTFOUND;
        }

        //Primero identificamos el div en el que esta el enlace
        tag.htmlTag = GUMBO_TAG_DIV;
        //Especificamos que elemento del div queremos recuperar
        Tags tag2;
        tag2.htmlTag = GUMBO_TAG_A;
        //Afinamos los datos a buscar
        parser.buscarElem2(util.getData(), &tag, "class", "noprint song-badge", &tag2, "href");

        for (int i=0; i < parser.getListUrlInfoSong()->size(); i++){
//            cout << "****" << parser.getListUrlInfoSong()->at(i) << "****" << endl;
            if (parser.getListUrlInfoSong()->at(i).find("wikipedia") != string::npos)
                trackinfo->urlInfo = parser.getListUrlInfoSong()->at(i);
        }
//        cout << "****" << trackinfo->urlInfo << "****" << endl;
        return SINERROR;
    }
    return ERRORCONNECT;
}

/**
*
*/
DWORD LyricsWikia::trackSearch(string track, string artist, vector <TrackInfo *> *info){
    DWORD retorno = SINERROR;
    //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
    if (info != NULL){
        info->clear();
        string url = "http://lyrics.wikia.com/api.php?func=getSong&fmt=realjson";
        url.append("&artist=");
        url.append(Constant::uencodeUTF8(artist));
        url.append("&song=");
        url.append(Constant::uencodeUTF8(track));

        map<string, string> cabeceras;
        cabeceras.clear();
        cabeceras.insert( make_pair("Accept", "*/*"));
        cabeceras.insert( make_pair("Accept-Encoding", "deflate"));
        cabeceras.insert( make_pair("Accept-Language", "es-ES,es;q=0.8,en;q=0.6,fr;q=0.4,zh-CN;q=0.2,zh;q=0.2,gl;q=0.2"));
        cabeceras.insert( make_pair("Content-Type", "application/x-www-form-urlencoded"));
        util.get(url, &cabeceras);

        Json::Value root;   // will contains the root value after parsing.
        Json::Reader reader;

        string respuesta = util.getData();
//        cout << respuesta << endl;
        bool parsingSuccessful = reader.parse(respuesta, root );
        if ( !parsingSuccessful ){
             // report to the user the failure and their locations in the document.
            Traza::print("LyricsWikia::trackSearch: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
            retorno = ERRORCONNECT;
        } else {
            Json::Value url = root["url"];
            TrackInfo *trackElem = new TrackInfo();
            trackElem->url = root.get("url","").asString();
            info->push_back(trackElem);
            if (trackElem->url.empty()){
                retorno = NOTFOUND;
            }
        }
    }
    return retorno;
}
