#include "unziptool.h"

UnzipTool::UnzipTool()
{
    //ctor
}

UnzipTool::~UnzipTool()
{
    //dtor
}

unzippedFileInfo UnzipTool::descomprimirZip(const char *ruta)
{

    unzFile myZip = NULL;
    myZip = unzOpen(ruta);
    unz_global_info zip_global_info;
    int ret;

    unsigned long nFilesInZip = 0;
    unzippedFileInfo retorno;
    unzippedFileInfo tempRetorno;

    tempRetorno.errorCode = -1;
    tempRetorno.rutaEscritura = ruta;
    tempRetorno.romsize = 0;

    retorno.errorCode = -1;
    retorno.rutaEscritura = ruta;
    retorno.romsize = 0;


    if (myZip == NULL){
        Traza::print("Unable to open zip file", W_ERROR);
        return(retorno);
    }
    else{
        Traza::print("File found", W_DEBUG);
    }

    ret = unzGetGlobalInfo(myZip, &zip_global_info);
    if (ret != UNZ_OK){
        Traza::print("unzGetGlobalInfo() call failed.", nFilesInZip, W_ERROR);
        return(retorno);
    } else {
        nFilesInZip = zip_global_info.number_entry;
        Traza::print("Found files in archive.", nFilesInZip, W_DEBUG);
    }

    ret = unzGoToFirstFile(myZip);

    if (ret != UNZ_OK)
    {
        Traza::print("unzGoToFirstFile() call failed.", W_ERROR);
        return(retorno);
    }


    while (ret == UNZ_OK){
        retorno = extraerFichero(&myZip, ruta);
        if (retorno.romsize > tempRetorno.romsize){
            tempRetorno.errorCode = retorno.errorCode;
            tempRetorno.romsize = retorno.romsize;
            tempRetorno.rutaEscritura = retorno.rutaEscritura;
        }
        ret = unzGoToNextFile(myZip);
    }

    unzClose(myZip);
    //unzCloseCurrentFile(myZip);

    return tempRetorno;
}

/**
*
*/
unzippedFileInfo UnzipTool::extraerFichero(unzFile *myZip, const char *ruta){

    int ret;
    unsigned long rom_size = 0;
     char filename[256];
    char *cartridge;
    unzippedFileInfo retorno;
    retorno.errorCode = -1;
    retorno.rutaEscritura = ruta;
    retorno.romsize = 0;
    unz_file_info zip_file_info;

    ret = unzGetCurrentFileInfo(*myZip, &zip_file_info, filename, 256, NULL, 0, NULL, 0);

    if (ret == UNZ_OK){
        Traza::print("Uncompressed size is in bytes", zip_file_info.uncompressed_size, W_DEBUG);
        rom_size = zip_file_info.uncompressed_size;
        retorno.romsize = rom_size;
    }

    ret = unzOpenCurrentFile(*myZip);
    if (ret != UNZ_OK){
        Traza::print("Unable to open file from zip archive.", W_ERROR);
        return(retorno);
    }

    cartridge = new char[rom_size];

    if (cartridge == NULL){
        Traza::print("Unable allocate memory for cartridge.", W_ERROR);
        return(retorno);
    }

    // Load rom file.
    ret = unzReadCurrentFile(*myZip, cartridge, rom_size);

    Fileio *fichero = new Fileio();
    Dirutil *dirUtil = new Dirutil();

    string filenameOut = dirUtil->getFolder(ruta) + FILE_SEPARATOR + filename;

    try{
        fichero->writeToFile(filenameOut.c_str(), cartridge, rom_size, false);
        retorno.rutaEscritura = filenameOut;
        retorno.errorCode = 0;
    } catch (Excepcion &e){
        Traza::print("Error UnzipTool::extraerFichero" + string(e.getMessage()), W_ERROR);
    }

    delete fichero;
    delete dirUtil;
    delete [] cartridge;
    return retorno;
}


