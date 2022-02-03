#ifndef UNZIPTOOL_H
#define UNZIPTOOL_H

#include <unistd.h>
#include <zlib.h>
#include <minizip\unzip.h>
//#include "..\..\ziputils\unzip\crypt.h"
#include "..\Fileio.h"
#include "..\Dirutil.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "../fileprops.h"

using namespace std;

struct unzippedFileInfo{
    int errorCode;
    string rutaEscritura;
    unsigned long romsize;
    uint32_t nFilesInZip;
    uint32_t nFilesWritten;
    vector<FileProps> files;
};

class UnzipTool
{
    public:
        UnzipTool();
        virtual ~UnzipTool();
        unzippedFileInfo descomprimirZip(const char *);
        unzippedFileInfo descomprimirZip(const char *rutaZip, const char *rutaDest);
        unzippedFileInfo descomprimirZip(const char *rutaZip, const char *rutaDest, bool uncompress);
        unzippedFileInfo listZipContent(const char *rutaZip);
        
    protected:
    private:
        unzippedFileInfo extraerFichero(unzFile *myZip, const char *ruta, bool uncompress);
        unzippedFileInfo extraerFichero(unzFile *myZip, const char *ruta);
};

#endif // UNZIPTOOL_H
