
#include "object.h"
#include "uilist.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "joymapper.h"
#include <map>
#include "zlib.h"
#include "Launcher.h"
#include "listaIni.h"


void * getClass(Object * obj){
    if (obj->getObjectType() == GUILISTBOX){
        return dynamic_cast<UIList*>(obj);
    }
    return NULL;
}

/**
* Extrae el contenido de una seccion. Hay que tener en cuenta que siempre se tienen
* que informar las secciones en orden. Sino no las encontrara
*/
string extractSection(ifstream *myfile, string *line, string section){
    bool continueReading = true;
    size_t posSection = line->find("- " + section + " -");

    size_t posEnd;
    string sectionValue = "";
    bool endReached = false;

    while (!endReached && continueReading){
        if (posSection == 0){
            continueReading = getline (*myfile, *line);
            posSection = line->find("- ");
            posEnd = line->find("$end");
            while ( (posEnd == string::npos || posEnd > 0) && (posSection == string::npos || posSection > 0) && continueReading){
                if (!line->empty()){
                    sectionValue.append(*line + VBCRLF);
                }
                continueReading = getline (*myfile, *line);
                posSection = line->find("- ");
                posEnd = line->find("$end");
            }
            endReached = true;
        } else {
            posEnd = line->find("$end");
            if (posEnd == string::npos || posEnd > 0){
                continueReading = getline (*myfile, *line);
                posSection = line->find("- " + section + " -");
            } else {
                endReached = true;
            }
        }
    }
    return sectionValue;
}


/**
*
*/
bool test_compress(Byte *compr, uLong *comprLen, const char *src){
    int err;
    uLong len = (uLong)strlen(src)+1;
    err = compress(compr, comprLen, (const Bytef*)src, len);
    return err == Z_OK;
}

/**
*
*/
bool test_uncompress(Byte *uncompr, uLong *uncomprLen, Byte *compr, uLong comprLen){
    int err;
    strcpy((char*)uncompr, "garbage");
    err = uncompress(uncompr, uncomprLen, compr, comprLen);
    return err == Z_OK;
}

void loadHistoryDatMameMap(string ruta, map<string, map<string, string> > *mapHistoryRoms){
    long ant = SDL_GetTicks();
    cout << "Iniciando la carga del fichero y procesando..." << endl;
    ant = SDL_GetTicks();
    int foundRoms = 0;
    string key, value, line;
    vector<string> splittedLine, splittedNameRom;
//    DataList *infoRom;
    string descRom;
    string nameRom;
    bool continueReading = true;

//    Byte *compr;
//    uLong comprLen = 10000*sizeof(int); /* don't overflow on MSDOS */
//    compr    = (Byte*)calloc((uInt)comprLen, 1);

    ifstream myfile (ruta.c_str());
    if (myfile.is_open()){
        while ( getline (myfile, line) ){
            if (line.find("$info") == 0){
                splittedLine = Constant::split(line, "=");
                //Si hemos encontrado un fichero, comprobamos que tenga descripcion, que debe estar en la
                //linea 5 despues de la $info
                for (int i=0; i < 5 && continueReading; i++){
                    continueReading = getline (myfile, line);
                }

//                if (i < file->getSize() - 5) i+=5;
//                line = file->get(i);
                descRom = "";

                size_t posSection = line.find("- ");
                while ( (posSection == string::npos || posSection > 0) && continueReading){
                    if (!line.empty()){
                        descRom.append(line + VBCRLF);
                    }
                    continueReading = getline (myfile, line);
                    posSection = line.find("- ");
                }

                if (splittedLine.size() > 1 && !descRom.empty()){
                    nameRom = splittedLine.at(1);
                    splittedNameRom = Constant::split(nameRom, ",");
                    for (unsigned int j=0; j < splittedNameRom.size(); j++){
                    //for (int j=0; j < 1; j++){
                        if (!splittedNameRom.at(j).empty()){
//                            infoRom = new DataList();
//                            cout << "adding: " << splittedNameRom.at(j) << endl;
                            map<string, string> attrRom;
                            attrRom.insert( make_pair("desc", descRom));
                            attrRom.insert( make_pair("tech", extractSection(&myfile, &line, "TECHNICAL")));
                            attrRom.insert( make_pair("trivia", extractSection(&myfile,&line,  "TRIVIA")));
                            attrRom.insert( make_pair("scoring", extractSection(&myfile, &line, "SCORING")));
                            attrRom.insert( make_pair("tips", extractSection(&myfile, &line, "TIPS AND TRICKS")));

    /**Prueba para comprimir los datos*/
    //                        comprLen = 10000*sizeof(int); /* don't overflow on MSDOS */
    //                        test_compress(compr, &comprLen, descRom.c_str());
    //                        attrRom.insert( make_pair("desc", (char *)compr));
    //                        comprLen = 10000*sizeof(int); /* don't overflow on MSDOS */
    //                        test_compress(compr, &comprLen, extractSection(file, &i, "TECHNICAL").c_str() );
    //                        attrRom.insert( make_pair("tech", (char *)compr));
                            mapHistoryRoms->insert(make_pair(splittedNameRom.at(j), attrRom));
                        }
                        foundRoms++;
                    }
                }
            }
        }
        myfile.close();
    }
    cout << "roms encontradas: " << foundRoms << endl;
    cout << "procesado en: " << (SDL_GetTicks() - ant) << " ms" << endl;
}

/**
*
*/
double getSongTime(string filepath){
    Launcher lanzador;
    FileLaunch emulInfo;
    Dirutil dir;
    //Se especifica el fichero de musica a reproducir
    emulInfo.rutaroms = dir.getFolder(filepath);
    emulInfo.nombrerom = dir.getFileName(filepath);
    //Se especifica el ejecutable
    emulInfo.rutaexe = string("C:") + tempFileSep;
    //emulInfo.fileexe = "ffmpeg.exe";
    //emulInfo.parmsexe = "-i";
    emulInfo.fileexe = "ffprobe.exe";
    emulInfo.parmsexe = " -i \"%ROMFULLPATH%\" -show_entries format=duration -v quiet -of csv=\"p=0\" > \""
                        + emulInfo.rutaroms + tempFileSep + "time.dat\"";

    //Lanzamos el programa
    bool resultado = lanzador.lanzarProgramaUNIXFork(&emulInfo);
    Fileio fichero;
    fichero.loadFromFile(emulInfo.rutaroms + tempFileSep + "time.dat");
    cout << "Obtenido: " << fichero.getFile() << endl;
    if (resultado){
        cout << "Lanzamiento OK" << endl;
    } else {
        cout << "Error en el lanzamiento" << endl;
    }
    return Constant::strToTipo<double>(fichero.getFile());
}


void convertir(string ruta){
    Dirutil dir;
    listaSimple<FileProps> *filelist = new listaSimple<FileProps>();

    dir.listarDir(ruta.c_str(), filelist);
    FileProps file;
    Launcher lanzador;
    FileLaunch emulInfo;
    emulInfo.rutaexe = "C:";
    emulInfo.fileexe = "ffmpeg.exe";
    //Conversion OGG
    emulInfo.parmsexe = string("-y -i \"%ROMFULLPATH%\" -map_metadata 0 -acodec libvorbis -id3v2_version 3 -write_id3v1 1 -ac 2 -b:a 128k ") +
                        string("\"%ROMPATH%") + tempFileSep + string("%ROMNAME%.ogg\"");
    //Conversion MP3
//    emulInfo.parmsexe = string("-y -i \"%ROMFULLPATH%\" -map_metadata 0 -acodec libmp3lame -id3v2_version 3 -write_id3v1 1 -ac 2 -b:a 128k ") +
//                        string("\"%ROMPATH%\\%ROMNAME%.mp3\"");
    Fileio fichero;
    string metadata = "";
    string rutaMetadata = ruta + tempFileSep + "metadata.txt";
    dir.borrarArchivo(rutaMetadata);
    Constant::setExecMethod(launch_create_process);

    for (unsigned int i=0; i < filelist->getSize(); i++){
        file = filelist->get(i);
        if (file.filename.find(".mp3") != string::npos || file.filename.find(".wma") != string::npos){
//            cout << file.dir << "," << file.filename << "," << file.filetype << endl;
            //timeSong = getSongTime(file.dir + FILE_SEPARATOR + file.filename);
//            cout << "time: " << timeSong << endl;
            emulInfo.rutaroms = file.dir;
            emulInfo.nombrerom = file.filename;
            lanzador.lanzarProgramaUNIXFork(&emulInfo);
//            metadata = dir.getFileNameNoExt(file.filename) + "=" + Constant::TipoToStr(timeSong) + VBCRLF;
//            fichero.writeToFile(rutaMetadata.c_str(),(char *)metadata.c_str(),metadata.length(),true);
        }
    }
}

int main(int argc, char *argv[]){

    //convertir("C:\\Users\\dmarcobo\\Music\\Johnny Cash\\Greatest");

//    Dirutil dir;
//    listaSimple<FileProps> *filelist = new listaSimple<FileProps>();
//    dir.listarDir("C:\\Greatest", filelist, ".mp3,.mid,.wav,.wma,.cda,.aac,.ac3,.flac,.mp4");
//    cout << filelist->getSize() << endl;

    //
//    Launcher lanzador;
//    Executable execCMD;
//    execCMD.filenameinparms = true;
//    execCMD.ejecutable = "C:\\ffmpeg.exe";
//    //execCMD.param = " -i \"C:\\Users\\dmarcobo\\Music\\Johnny Cash\\Greatest\\01 Goodbye Little Darlin' Goodbye.wma\" -show_entries format=duration -v quiet -of csv=\"p=0\" > \"C:\\Users\\dmarcobo\\Music\\Johnny Cash\\Greatest\\time.dat\"";
//    //execCMD.param = " -i \"C:\\Users\\dmarcobo\\Music\\Johnny Cash\\Greatest\\01 Goodbye Little Darlin' Goodbye.wma\" -show_entries format=duration -v quiet -of csv=\"p=0\"";
//    execCMD.param = "-y -i \"C:\\Users\\dmarcobo\\Music\\Johnny Cash\\Greatest\\01 Goodbye Little Darlin' Goodbye.wma\" -map_metadata 0 -acodec libvorbis -id3v2_version 3 -write_id3v1 1 -ac 2 -b:a 128k \"C:\\Users\\dmarcobo\\Music\\Johnny Cash\\Greatest\\01 Goodbye Little Darlin' Goodbye.ogg\"";
//
//    lanzador.createProcess(execCMD);
//    cout << "Obteniendo datos del clipboard" << endl;
//    cout << Constant::GetClipboardText() << endl;

/**Pruebas del token
    Traza *traza = new Traza();

    ListaIni<Data> *config = new ListaIni<Data>();
    string rutaIni = "C:\\GitHub\\ExternalLibs\\UIObjects\\bin\\Debug\\token.ini";
    config->loadFromFile(rutaIni);
    config->sort();

    Data dato;
    int pos = config->find("accesstoken");
    if (pos != -1){
        dato.setKeyValue("accesstoken", "1");
        config->set(pos, &dato);
    }
    Traza::print("pos1", pos, W_DEBUG);

    pos = config->find("googleaccesstoken");
    if (pos != -1){
        dato.setKeyValue("googleaccesstoken", "2");
        config->set(pos, &dato);
    }
    Traza::print("pos2", pos, W_DEBUG);

    pos = config->find("googlerefreshtoken");
    if (pos != -1){
        dato.setKeyValue("googlerefreshtoken", "3");
        config->set(pos, &dato);
    }
    Traza::print("pos3", pos, W_DEBUG);

    Traza::print("Size: ", config->getSize(), W_DEBUG);

    config->writeToFile("C:\\GitHub\\ExternalLibs\\UIObjects\\bin\\Debug\\token2.ini");


    cout << "pos: " << pos << endl;
*/

//    if (pos != -1){
//        config->del(pos);
//        Data *dato = new Data();
//        dato->setKeyValue("accesstoken", accessTokenCipherB64);
//        config->add(*dato);
//        config->writeToFile("C:\\asdasdaa.txt");
//        Traza::print("guardando en C:\\asdasdaa.txt", W_DEBUG);
//    }
//    Traza::print("strAccessToken: " + strAccessToken, W_DEBUG);

//    Dirutil dir;
//    vector<FileProps> *filelist = new vector<FileProps>();
//    dir.listarDirRecursivo("D:\\\\JohnnyCash", filelist, ".ogg");
//
//    for (int i=0; i < filelist->size(); i++){
//        cout << filelist->at(i).filename << endl;
//    }

//    Dirutil dir;
//
//    vector<t_drive *> drives;
//    dir.getDrives(&drives);
//    string driveTypeString;
//
//    for (unsigned int i=0; i < drives.size(); i++){
//        cout << drives.at(i)->drive;
//        cout << " " << drives.at(i)->driveTypeString << endl;
//    }

    cout << Constant::timeFormat(4721768) << endl;

    return 0;
}
