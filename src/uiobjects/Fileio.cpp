#include "Fileio.h"


/**
*/
Fileio::Fileio(){
    memblock = NULL;
    size = 0;
}

/**
*/
Fileio::~Fileio(){
    clearFile();
}

 bool Fileio::loadFromMem(const char *buf, size_t tam){
    memblock = new char [tam];//Reservamos memoria
    memcpy ( memblock, buf, tam );
    size = tam;
    return true;
 }

/**
* Retorna en bytes el tamanyo de un fichero
*/
ifstream::pos_type Fileio::getFileSize(const char *uri){

    ifstream::pos_type begin,end;
    ifstream::pos_type total = 0;
    ifstream myfile (uri);
    if (myfile.is_open()){
        begin = myfile.tellg();
        myfile.seekg (0, ios::end);
        end = myfile.tellg();
        myfile.close();
        total = end-begin;
        return total;
    } else {
        return 0;
    }
}

/**
*/
bool Fileio::clearFile(){
    if (memblock != NULL){
        size = 0;
        delete [] memblock;
        memblock = NULL;
        return true;
    }
    return false;
}

unsigned int Fileio::contarLineas(const char *filename){

    ifstream archivo(filename, ios::in);
    string str;
    unsigned int cont=0;

    if (archivo.is_open()){
        for(cont=0;!archivo.eof() && getline(archivo,str);cont++);
        archivo.close();
    }
    Traza::print("contarLineas:",cont, W_DEBUG);
    return cont;
}

/**
* Lee un fichero en formato binario y lo almacena en memoria
*/
bool Fileio::loadFromFile(string uri){
    return loadFromFile(uri.c_str(), 0, getFileSize(uri.c_str()));
}

/**
* Lee un fichero en formato binario y lo almacena en memoria
*/
bool Fileio::loadFromFile(const char *uri, ifstream::pos_type ini, ifstream::pos_type tam){
    bool ret = false;
    clearFile();
    //Usamos el flag ios::ate flag para poder obtener
    //directamente el tam del fichero una vez abierto
    ifstream file (uri, ios::in | ios::binary | ios::ate);
    Traza::print("loadfromfile: " + string(uri), W_PARANOIC);

    if (file.is_open()){
        try{
            size = file.tellg();//Obtenemos el tamanyo del fichero
            if (tam + ini <= size){
                memblock = new char [tam];//Reservamos memoria
                file.seekg (ini, ios::beg);//Volvemos al principio del fichero
                file.read (memblock, tam);//Leemos el fichero
                file.close();//Cerramos el fichero
                size = tam;
                Traza::print("the complete file content is in memory: size:",size, W_PARANOIC);
                ret = true;
            } else {
                Traza::print("loadFromFile: El tam. del fichero es menor que el tam. definido", W_ERROR);
                Traza::print("uri:" + string(uri), W_PARANOIC);
                Traza::print("size:",size, W_PARANOIC);
                Traza::print("tam + ini:",tam + ini, W_PARANOIC);
                file.close();//Cerramos el fichero
                ret = false;
                throw(Excepcion(ERANGO));
            }

        } catch(bad_alloc&) {
            Traza::print("loadFromFile: Memoria insuficiente al reservar: " + string(uri), W_ERROR);
            file.close();//Cerramos el fichero
            ret = false;
            throw(Excepcion(EESPACIO));
        }
    } else {
        Traza::print("loadFromFile: Unable to open file: " + string(uri), W_ERROR);
        file.close();//Cerramos el fichero
        ret = false;
        throw(Excepcion(EFIO));
    }
    return ret;
}


/**
* operaciones de escritura
*
* ios::in	Open for input operations.
* ios::out	Open for output operations.
* ios::binary	Open in binary mode.
* ios::ate	Set the initial position at the end of the file. If this flag is not set to any value, the initial position is the beginning of the file.
* ios::app	All output operations are performed at the end of the file, appending the content to the current content of the file. This flag can only be used in streams open for output-only operations.
* ios::trunc	If the file opened for output operations already existed before, its previous content is deleted and replaced by the new one.
*/
void Fileio::writeToFile(const char *uri, char * memblocktowrite, ifstream::pos_type tam, bool append){

    Traza::print("writeToFile:" + string(uri), W_DEBUG);
    ofstream file;

    if (append){
        file.open(uri,ios::out | ios::binary | ios::app);
    } else {
        file.open(uri,ios::out | ios::binary | ios::trunc);
    }

    if (file.is_open()){
        file.write(memblocktowrite, tam);
        file.close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file.close();//Cerramos el fichero
        throw(Excepcion(EFIO));
    }
    #ifdef GP2X
        sync();
    #endif
}

