#ifndef FILEIO_H_INCLUDED
#define FILEIO_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>    // std::count
#include <vector>       // std::vector
#include <iterator>     // istream_iterator
#include "Excepcion.h"
#include "Traza.h"


// template<typename T, size_t SIZE>
//        static size_t getSize(T (&)[SIZE]) {
//            return SIZE;
//        }

class Fileio{
    public :
        Fileio();
        ~Fileio();
        bool loadFromFile(string);
        bool loadFromFile(const char *, ifstream::pos_type , ifstream::pos_type);
        bool loadFromMem(const unsigned char *, size_t tam);
        int writeToFile(const char *, char * ,ifstream::pos_type, bool append);
        bool clearFile();
        char * getFile() {return memblock;}
        ifstream::pos_type getFileSize(const char *);
        ifstream::pos_type getFileSize() {return size;}
        unsigned int contarLineas(const char *);

    private:
        ifstream::pos_type size;
        char * memblock;
};

#endif // FILEIO_H_INCLUDED
