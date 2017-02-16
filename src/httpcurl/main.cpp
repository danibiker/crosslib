#include "streamcloud.h"
#include "allmyvideos.h"
#include <iostream>
//#include "thegamesdb.h"
#include "util/ConstantHttp.h"
#include "servers/dropbox.h"
#include "filecipher.h"
#include "servers/allmyvideos.h"
#include "lyrics/MusixMatch.h"
#include "lyrics/lyricswikia.h"
#include "lyrics/darklyrics.h"
#include "lyrics/lyricsbase.h"
#include "servers/googledrive.h"
#include "cddb/freedb.h"

using namespace std;

const string filtroOGG = ".ogg";

void uploadMusicToGoogle(string ruta, string parentid, string accessToken){
    Dirutil dir;

    if (dir.existe(ruta) && !accessToken.empty()){
        listaSimple<FileProps> *filelist = new listaSimple<FileProps>();
        dir.listarDir(ruta.c_str(), filelist, filtroOGG);
        FileProps file;
        GoogleDrive google;
        int countFile = 0;
        string rutaLocal, upid, nombreAlbum;

        if (filelist->getSize() > 0){
            //Se crea el directorio que contiene los ficheros
            file = filelist->get(0);
            nombreAlbum = file.dir.substr(file.dir.find_last_of(tempFileSep) + 1);
            if (!google.fileExist(nombreAlbum, parentid, accessToken).empty()){
                string idNewAlbum = google.mkdir(nombreAlbum, parentid, accessToken);
                for (int i=0; i < filelist->getSize(); i++){
                    file = filelist->get(i);
                    rutaLocal = file.dir + tempFileSep + file.filename;

                    if (filtroOGG.find(dir.getExtension(file.filename)) != string::npos && file.filename.compare("..") != 0){
                        countFile++;
                        int percent = (countFile/(float)(filelist->getSize()-1))*100;
                        Traza::print("Subiendo fichero...", W_DEBUG);
                        upid = google.chunckedUpload(rutaLocal, idNewAlbum, accessToken);
                        Traza::print("Confirmada subida del album...", W_DEBUG);
                        //Si habiamos convertido el fichero, lo borramos
                    }
                }
            }
        }
    }
}




/// Converts a unicode code-point to UTF-8.
inline std::string codePointToUTF8(unsigned int cp) {
  std::string result;
  // based on description from http://en.wikipedia.org/wiki/UTF-8
  if (cp <= 0x7f) {
    result.resize(1);
    result[0] = static_cast<char>(cp);
  } else if (cp <= 0x7FF) {
    result.resize(2);
    result[1] = static_cast<char>(0x80 | (0x3f & cp));
    result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
  } else if (cp <= 0xFFFF) {
    result.resize(3);
    result[2] = static_cast<char>(0x80 | (0x3f & cp));
    result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
    result[0] = static_cast<char>(0xE0 | (0xf & (cp >> 12)));
  } else if (cp <= 0x10FFFF) {
    result.resize(4);
    result[3] = static_cast<char>(0x80 | (0x3f & cp));
    result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
    result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
    result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
  }

  return result;
}

string toUTF8(string s){
    string salida;
    string c;

    char datos [s.length() + 1];
    strcpy(datos, s.c_str());

    int leter;
    for (unsigned int i=0; i<s.length(); i++){
            leter = (int)datos[i];
            if (leter < 0) leter = 255 + leter;
            cout << "datos[i]: " << datos[i] << " (int): " << leter;
            c = codePointToUTF8(leter);
            cout << " c: " << c << endl;
            salida += c;

    }
    return salida;
}


int main(int argc, char *argv[]){
    Dirutil dir;
    Constant::setAppDir(dir.getDirActual());
    Constant::setPROXYIP("10.129.8.100");
    Constant::setPROXYPORT("8080");
    Constant::setPROXYUSER("dmarcobo");
    Constant::setPROXYPASS("bC6E4X0V3c");

//    toUTF8("Calc - copi�a");


    //Streamcloud streamcloud;
    //cout << streamcloud.download("http://streamcloud.eu/512zjmlchbr7") << endl;
    //cout << streamcloud.download("http://www.google.es") << endl;
//
//    cout << "bajando imagen" << endl;
//    HttpUtil util;
//    util.download("http://www.walpaper.es/images/wallpapers/espectro-de-colores-8349.jpeg", "C:\\espectro-de-colores-8349.jpeg");

//    cout << "bajando firefox" << endl;
//    util.download("http://ftp.mozilla.org/pub/mozilla.org/firefox/releases/latest/win32/en-US/Firefox%20Setup%2038.0.1.exe");
//    util.writeToFile("C:\\FIREFOX.exe");


    //Allmyvideos allmyvideos;
    //cout << allmyvideos.download("http://allmyvideos.net/8cf8gd6ez70t") << endl;
    //cout << vk("http://vk.com/video_ext.php?oid=195176000&id=164918704&hash=d64de7555258983a&hd=2") << endl;
    //dump_to_stdout( "querys.xml" );
/**  Pruebas de descarga de informaci�n de juegos*/

    HttpUtil util;
    util.download("http://thegamesdb.net/api/GetGame.php?platform=Sega%20Master%20System&name=star%20wars");
    cout << util.getDataLength() << endl;
    cout << util.getData() << endl;


//    RequestQuery queryGame;
//    queryGame.id = "2684";
//    queryGame.name = "Alex Kidd in Miracle World";
//    queryGame.name = "Xenon 2 - Megablast";
//    queryGame.platform = "Sega Master System";
//    queryGame.exactName = "Xenon 2 - Megablast";
//    TheGamesDB gamesDB;
//    ResponseQuery response;
//
//    int petOK = gamesDB.download2(&queryGame, &response);
//    cout << "Recuperados: " << response.gameList.size() << endl;
//
//    for (int i=0; i < response.gameList.size(); i++){
//            cout << "El id es: " << response.gameList.at(i)->id << endl;
//            cout << "El titulo es: " << response.gameList.at(i)->gameTitle << endl;
//            cout << "El platformId es: " << response.gameList.at(i)->platformId << endl;
//            cout << "El platform es: " << response.gameList.at(i)->platform << endl;
//            cout << "El overview es: " << response.gameList.at(i)->overview << endl;
//            cout << "El releaseDate es: " << response.gameList.at(i)->releaseDate << endl;
//            cout << "El genres es: " << response.gameList.at(i)->genres.size() << endl;
//            cout << "El players es: " << response.gameList.at(i)->players << endl;
//            cout << "El publisher es: " << response.gameList.at(i)->publisher << endl;
//            cout << "El developer es: " << response.gameList.at(i)->developer << endl;
//            cout << "El rating es: " << response.gameList.at(i)->rating << endl;
//            cout << "El baseImgUrl es: " << response.gameList.at(i)->baseImgUrl << endl;
//            cout << "El boxart es: " << response.gameList.at(i)->boxart.size() << endl;
//            cout << "El fanart es: " << response.gameList.at(i)->fanart.size() << endl;
//            cout << "******************************" << endl << endl;
//    }
//
//    switch (petOK){
//        case QUERYOK :
//            cout << "Peticion OK" << endl;
//            break;
//        case GAMENOTFOUND:
//            cout << "Juego no encontrado: " << response.error << endl;
//            break;
//        case CONNECTERROR:
//            cout << "Problema de conexion" << endl;
//            break;
//        default:
//            cout << "Error no determinado" << endl;
//            break;
//    }
//
//        //Prueba ok de subida y carga de ficheros en dropbox
//        string postData="";
//        map<string, string> cabeceras;

    /**Pruebas para carga en dropbox*/
//        string cliendid="";
//        string secret="";
//        const int salClave[32] = {0xba,0xa3,0x7,0x99,0x1b,0x56,0x66,0xb6,0x2a,0x36,0xe4,0x89,0xd2,0x36,0x55,0x55,0x54,0x46,0x9a,0x95,0x15,0x6c,0xad,0xf7,0,0xc4,0xf5,0x63,0x4a,0xb0,0xfb,0xf0};
//        const int salIv[16] = {0xc3,0x49,0xf0,0xa0,0x72,0x22,0xc,0x63,0xf1,0xd3,0xe8,0x87,0x90,0x85,0,0xa5};
//        unsigned char k[32];
//        unsigned char iv[16];
//        Dirutil dir;
//        Constant::setAppDir(dir.getDirActual());
//        Dropbox dropbox;
//        filecipher cifrador;

//        cout << "introduce password para cifrar el fichero enviado" << endl;
//        string password;
//        cin >> password;
//
//        if (!password.empty()){
//            //Generamos la clave de 32 bytes
//            cifrador.generarHash(password, (char *)salClave, 32, k);
//            //Generamos la semilla de 16 bytes
//            cifrador.generarHash("alssj8289", (char *)salIv, 16, iv);
//            cifrador.setClave(k, 32);
//            cifrador.setIV(iv, 16);
//        }
//
//
////        cout << "Comprobando autorizacion..." << endl;
//
//
        Traza *traza = new Traza();
//        DWORD ret = dropbox.authenticate();
//        string accessToken = dropbox.getAccessToken();
//
//        if (accessToken.empty()){
//            dropbox.launchAuthorize(cliendid);
//            string code;
//            cout << "Introduzca el codigo obtenido de la web de dropbox" << endl;
//            cin >> code;
//            cout << "Negociando access token..." << endl;
//            if (!code.empty()){
//                accessToken = dropbox.storeAccessToken(cliendid, secret, code);
//            }
//        }
//        Traza::print("accessToken: " + accessToken,W_DEBUG);

//        /**Pruebas para subir y bajar fichero cifrado*/
//        cout << "Cifrando fichero..." << endl;
//        cifrador.cifrar("C:\\calc.exe", "C:\\calc.exe.cif");
//        cout << "Subiendo fichero..." << endl;
//        string upid = dropbox.chunckedUpload("C:\\calc.exe.cif", accessToken);
//        cout << "Confirmando subida..." << endl;
//        dropbox.commitChunkedUpload("calc.exe", accessToken, upid);
//        cout << "Fin de la subida. Descargando..." << endl;
//        dropbox.getFile("C:\\ejemplo.exe.cif", "calc.exe", accessToken);
//        cout << "Descifrando fichero..." << endl;
//        cifrador.descifrar("C:\\ejemplo.exe.cif", "C:\\ejemplo.exe");
//        dropbox.getFile("C:\\loneeee.ogg", "/Music/The Black Keys - El Camino/01-Lonely Boy.ogg", accessToken);
//
//
        //dropbox.deleteFiles("/Music/Johnny Cash Greatest CD2", accessToken);

        /**Pruebas para listar un directorio o fichero*/
//        CloudFiles files;
//        dropbox.listFiles("/Music", accessToken, &files);
//
//        for (int i=0; i < files.fileList.size(); i++){
//            cout << ((CloudFiles *)files.fileList.at(i))->path << endl;
//        }
//
//        files.fileList.clear();
//        dropbox.listFiles("/Music", accessToken, &files);
//        for (int i=0; i < files.fileList.size(); i++){
//            cout << ((DropboxFiles *)files.fileList.at(i))->path << endl;
//        }

//        string textoPlano = "Del barco del chanquete no nos moveran";
//        string cifrado = cifrador.encodeEasy(textoPlano, password);
//        cout << cifrador.decodeEasy(cifrado, password) << endl;

        /**Pruebas de carga de json*/
//        Json::Value root;   // will contains the root value after parsing.
//        Json::Reader reader;
//        Fileio file;
//        file.loadFromFile("C:\\Johnny Cash Greatest CD2\\metadata2.txt");
//        bool parsingSuccessful = reader.parse( file.getFile(), root );
//        if ( !parsingSuccessful ){
//            cout << "Failed to parse configuration" << reader.getFormattedErrorMessages() << endl;
//        } else {
//            string songFileName;
//            string atributeName;
//            string atributeValue;
//
//            for (int i=0; i < root.getMemberNames().size(); i++){
//                songFileName = root.getMemberNames().at(i);
//                Json::Value value;
//                value = root[root.getMemberNames().at(i)];
//                for (int j=0; j < value.getMemberNames().size(); j++){
//                    atributeName = value.getMemberNames().at(j);
//                    atributeValue = value[atributeName].asString();
//                    cout << atributeName << ":" << atributeValue << endl;
//
//                }
//            }
//        }

//        string dato = "Music/�Te lo dije!/07-Un D�a M�s.ogg";
//        cout << "dato   : " << dato << endl;
//        string coded = Constant::uencodeUTF8(dato);
//        cout << "coded  : " <<coded << endl;
//        string decoded = Constant::udecodeUTF8(coded);
//        cout << "decoded: " << decoded << endl;
//
//        string prueba2 = "Un%20D%C3%83%C2%ADa%20M%C3%83%C2%A1s";
//        cout << prueba2 << endl;
//        cout << Constant::udecodeUTF8(prueba2) << endl;

        /** Pruebas de musixmatch*/

//        MusixMatch musicScraper;
//        vector <TrackInfo *> info;
//        musicScraper.trackSearch("Until It Sleeps","Metallica", &info);
//        if (info.size() > 0){
//            musicScraper.trackLyrics(info.at(0));
//            cout << info.at(0)->lyrics_body << endl;
//        }




        /** Pruebas en lyricswikia*/
        /** Pruebas en darklyrics*/

//        LyricsWikia musicScraper;
//        LyricsBase *arr[2] = {new LyricsWikia(), new Darklyrics()};
//        vector <TrackInfo *> info;
//        int ret = NOTFOUND;
//
//        for (int i=0; i < 2 && ret != SINERROR; i++){
//            cout << "Buscando en " << i << endl;
//            ret = arr[i]->trackSearch("pure lust", "Cadavrul", &info);
//            if (ret == SINERROR){
//                cout << info.at(0)->url << endl;
//                ret = arr[i]->trackLyrics(info.at(0));
//                if (ret == SINERROR)
//                    cout << info.at(0)->lyrics_body << endl;
//            }
//        }



//{
//  "access_token" : "",
//  "token_type" : "Bearer",
//  "expires_in" : 3600,
//  "refresh_token" : ""
//}
//
//        GoogleDrive *gDrive = new GoogleDrive();
//        gDrive->setClientid("");
//        gDrive->setSecret("");
//
//        /**Para refrescar el token al iniciar el programa*/
//        int error = gDrive->authenticate();
//        if (error == ERRORREFRESHTOKEN){
//            gDrive->storeAccessToken(gDrive->getClientid(), gDrive->getSecret(), gDrive->getRefreshToken(), true);
//        } else if (error == ERRORACCESSTOKEN){
//            gDrive->launchAuthorize(gDrive->getClientid());
//            string code;
//            cout << "Introduzca el codigo de google" << endl;
//            cin >> code;
//            gDrive->storeAccessToken(gDrive->getClientid(), gDrive->getSecret(), code, false);
//        }
//        cout << gDrive->getAccessToken() << endl;
//        cout << gDrive->getRefreshToken() << endl;

//        gDrive->launchAuthorize(gDrive->getClientid());
//        cout << "Introduce el c�digo que has obtenido del explorador" << endl;
//        string input;
//        cin >> input;
//        //Obteniendo el access token
//        gDrive->launchAccessToken(gDrive->getClientid(), gDrive->getSecret(), input, false);
//        //Refrescando el accesstoken
//        gDrive->launchAccessToken(gDrive->getClientid(), gDrive->getSecret(), gDrive->getRefreshToken(), true);
//        string idDir = gDrive->mkdir("ONMUSIK", "", gDrive->getAccessToken());
//        cout << idDir << endl;
//        idDir = gDrive->mkdir("Music", idDir, gDrive->getAccessToken());
        //gDrive->chunckedUpload("C:\\calc.exe", "", gDrive->getAccessToken());
//        uploadMusicToGoogle("C:\\Johnny Cash Greatest CDS", "0BylhO6pH87NvRUhWQ2R0ZmNHQ2c", gDrive->getAccessToken());
//        CloudFiles files;
//        gDrive->listFiles("0BylhO6pH87NvLXVKeWRUREtrV2c", gDrive->getAccessToken(), &files);

//        if (gDrive->deleteFiles("0BylhO6pH87NvdUdxUWdvdWpFbjQ", gDrive->getAccessToken())){
//            cout << "Fichero Borrado con exito" << endl;
//        } else {
//            cout << "No se ha podido borrar el fichero" << endl;
//        }
//        gDrive->listFiles("0BylhO6pH87NvRUhWQ2R0ZmNHQ2c", gDrive->getAccessToken(), &files);

        /**Pruebas refresco de token*/
        //string idDir = gDrive->mkdir("ONMUSIK4", "", gDrive->getAccessToken());
//        gDrive->chunckedUpload("C:\\calc - copi�a.exe", "0BylhO6pH87NvYnZmeUo0R0xaNTA", gDrive->getAccessToken());
//        string fileid = gDrive->fileExist("Music", "", gDrive->getAccessToken());

//        Traza::print("Fileid: " + fileid, W_DEBUG);
//        gDrive->listFiles("0BylhO6pH87NvYnZmeUo0R0xaNTA", gDrive->getAccessToken(), &files);
//        for (int i=0; i < files.fileList.size(); i++){
//            cout << files.fileList.at(i)->path << endl;
//        }
//        gDrive->getFile("C:\\calcame.exe", "0BylhO6pH87NvN3hLNFhkVUh1QWs", gDrive->getAccessToken() );
//        gDrive->deleteFiles("0BylhO6pH87NvN3hLNFhkVUh1QWs", gDrive->getAccessToken() );



//        delete gDrive;
/*
        Freedb cddb;
        CdTrackInfo cdTrack;
        FreedbQuery query;
        query.discId = "4984848";
        query.username = "dani";
        query.hostname = "dani.web.com";
        query.clientname = "dani";
        query.version = "1";
        query.categ = "rock";
        query.totalSeconds = 4679;

        query.offsets.push_back(0);
        query.offsets.push_back(15913);
        query.offsets.push_back(35915);
        query.offsets.push_back(48643);
        query.offsets.push_back(70545);
        query.offsets.push_back(88055);
        query.offsets.push_back(96585);
        query.offsets.push_back(115339);
        query.offsets.push_back(140543);
        query.offsets.push_back(160413);
        query.offsets.push_back(184698);
        query.offsets.push_back(212533);
        query.offsets.push_back(231798);
        query.offsets.push_back(253833);
        query.offsets.push_back(273200);
        query.offsets.push_back(302997);


        vector<CdTrackInfo *> cdTrackList;
        Traza::print("Buscando album", W_DEBUG);
        int code = cddb.searchCd(&query, &cdTrackList);

        for (int i=0; i < cdTrackList.size(); i++){
            Traza::print("albumName: " + cdTrackList.at(i)->albumName, W_DEBUG);
            Traza::print("genre: " + cdTrackList.at(i)->genre, W_DEBUG);
            Traza::print("discId: " + cdTrackList.at(i)->discId, W_DEBUG);
        }
*/

//        int code = cddb.getCdInfo(&query, &cdTrack);
//        Traza::print("Codigo", code, W_DEBUG);
//        for (int i=0; i < cdTrack.titleList.size(); i++){
//            Traza::print(cdTrack.titleList.at(i), W_DEBUG);
//        }


        return 0;
}

