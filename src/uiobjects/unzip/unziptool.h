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

using namespace std;

struct unzippedFileInfo{
    int errorCode;
    string rutaEscritura;
    unsigned long romsize;
};

class UnzipTool
{
    public:
        UnzipTool();
        virtual ~UnzipTool();
        unzippedFileInfo descomprimirZip(const char *);
    protected:
    private:
        unzippedFileInfo extraerFichero(unzFile *myZip, const char *ruta);
};

#endif // UNZIPTOOL_H
