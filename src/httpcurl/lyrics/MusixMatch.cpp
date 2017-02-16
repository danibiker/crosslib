#include "MusixMatch.h"

MusixMatch::MusixMatch(){
    //ctor
}

MusixMatch::~MusixMatch(){
    //dtor
}

/**
*
*/
DWORD MusixMatch::trackLyrics(TrackInfo *trackinfo){

    DWORD retorno = SINERROR;
    //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
    string url = "http://api.musixmatch.com/ws/1.1/track.lyrics.get?";
    url.append("apikey=");
    url.append(APIKEY);
    url.append("&track_id=");
    url.append(trackinfo->trackId);

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
    bool parsingSuccessful = reader.parse(respuesta, root );

    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("MusixMatch::trackLyrics: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
    } else {
        Json::Value header = root["message"]["header"];
        string status_code = header.get("status_code","").asString();
        //cout << "status_code: " << status_code << endl;
        Json::Value lyrics = root["message"]["body"]["lyrics"];

        for ( int index = 0; index < lyrics.size(); index++ ){  // Iterates over the sequence elements.
           trackinfo->lyrics_body = lyrics.get("lyrics_body","").asString();
        }
    }
    return retorno;
}

/**
*
*/
DWORD MusixMatch::trackSearch(string track, string artist, vector <TrackInfo *> *info){
    DWORD retorno = SINERROR;
    //Comprobamos que podemos obtener info del usuario para saber si el accesstoken es valido
    string url = "http://api.musixmatch.com/ws/1.1/track.search?";
    url.append("apikey=");
    url.append(APIKEY);
    url.append("&q_track=");
    url.append(Constant::uencodeUTF8(track));
    url.append("&q_artist=");
    url.append(Constant::uencodeUTF8(artist));

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
    bool parsingSuccessful = reader.parse(respuesta, root );

    if ( !parsingSuccessful ){
         // report to the user the failure and their locations in the document.
        Traza::print("MusixMatch::trackSearch: Failed to parse configuration. " + reader.getFormattedErrorMessages(), W_ERROR);
        retorno = ERRORCONNECT;
    } else {
        //Json::Value track = root["message"]["body"]["track_list"]["track"];
        Json::Value header = root["message"]["header"];
        string status_code = header.get("status_code","").asString();
        //cout << "status_code: " << status_code << endl;
        Json::Value tracklist = root["message"]["body"]["track_list"];
        //cout << "tracklist: " << tracklist.size() << endl;

        for ( int index = 0; index < tracklist.size(); index++ ){  // Iterates over the sequence elements.
           Json::Value track = tracklist[index]["track"];
           TrackInfo *trackElem = new TrackInfo();
           trackElem->album_name = track.get("album_name","").asString();
           trackElem->trackId = track.get("track_id","").asString();
           trackElem->album_coverart_100x100 = track.get("album_coverart_100x100","").asString();
           trackElem->album_coverart_350x350 = track.get("album_coverart_350x350","").asString();
           trackElem->track_name = track.get("track_name","").asString();
           info->push_back(trackElem);
        }
    }
    return retorno;
}
