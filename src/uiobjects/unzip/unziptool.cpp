#include "unziptool.h"

UnzipTool::UnzipTool()
{
    //ctor
}

UnzipTool::~UnzipTool()
{
    //dtor
}

unzippedFileInfo UnzipTool::descomprimirZip(const char *ruta){
    return descomprimirZip(ruta, ruta);
}

unzippedFileInfo UnzipTool::descomprimirZip(const char *rutaZip, const char *rutaDest){
    return descomprimirZip(rutaZip, rutaDest, true);
}

unzippedFileInfo UnzipTool::listZipContent(const char *rutaZip){
    return descomprimirZip(rutaZip, rutaZip, false);
}

unzippedFileInfo UnzipTool::descomprimirZip(const char *rutaZip, const char *rutaDest, bool uncompress){
    unzFile myZip = NULL;
    myZip = unzOpen(rutaZip);
    unz_global_info zip_global_info;
    int ret;
    Dirutil dir;

    unsigned long nFilesInZip = 0;
    unzippedFileInfo retorno;
    unzippedFileInfo tempRetorno;

    tempRetorno.errorCode = -1;
    tempRetorno.rutaEscritura = rutaZip;
    tempRetorno.romsize = 0;
    tempRetorno.nFilesInZip = 0;
    tempRetorno.nFilesWritten = 0;
    
    retorno.errorCode = -1;
    retorno.rutaEscritura = rutaZip;
    retorno.romsize = 0;
    retorno.nFilesInZip = 0;
    retorno.nFilesWritten = 0;


    if (myZip == NULL){
        Traza::print("UnzipTool::descomprimirZip. Unable to open zip file: " + string(rutaZip), W_ERROR);
        return(retorno);
    }
    else{
        Traza::print("UnzipTool::descomprimirZip. File found: " + string(rutaZip), W_PARANOIC);
    }

    ret = unzGetGlobalInfo(myZip, &zip_global_info);
    if (ret != UNZ_OK){
        Traza::print("unzGetGlobalInfo() call failed.", nFilesInZip, W_ERROR);
        return(retorno);
    } else {
        nFilesInZip = zip_global_info.number_entry;
        tempRetorno.nFilesInZip = nFilesInZip;
        Traza::print("Found files in archive.", nFilesInZip, W_PARANOIC);
    }

    ret = unzGoToFirstFile(myZip);

    if (ret != UNZ_OK)
    {
        Traza::print("unzGoToFirstFile() call failed.", W_ERROR);
        return(retorno);
    }

    while (ret == UNZ_OK){
        retorno = extraerFichero(&myZip, rutaDest, uncompress);
        if (retorno.romsize > tempRetorno.romsize){
            tempRetorno.errorCode = retorno.errorCode;
            tempRetorno.romsize = retorno.romsize;
            tempRetorno.rutaEscritura = retorno.rutaEscritura;
        }
        
        if (retorno.errorCode == 0){
            tempRetorno.nFilesWritten++;
        }
        
        if (!uncompress){
            FileProps fileprop;
            fileprop.filename = dir.getFileName(retorno.rutaEscritura);
            fileprop.dir = dir.getFolder(retorno.rutaEscritura);
            fileprop.fileSize = retorno.romsize;
            tempRetorno.files.push_back(fileprop);
        }
        
        
        ret = unzGoToNextFile(myZip);
    }

    unzClose(myZip);
    //unzCloseCurrentFile(myZip);
    
    
    return tempRetorno;
}



#define MAX_FILENAME 512

/**
*
*/
unzippedFileInfo UnzipTool::extraerFichero(unzFile *myZip, const char *ruta){
    return extraerFichero(myZip, ruta, true);
}

unzippedFileInfo UnzipTool::extraerFichero(unzFile *myZip, const char *ruta, bool uncompress){
    int ret;
    unsigned long rom_size = 0;
    char filename[MAX_FILENAME];
    char *cartridge;
    unzippedFileInfo retorno;
    retorno.errorCode = -1;
    retorno.rutaEscritura = ruta;
    retorno.romsize = 0;
    unz_file_info zip_file_info;

    ret = unzGetCurrentFileInfo(*myZip, &zip_file_info, filename, MAX_FILENAME, NULL, 0, NULL, 0);

    if (ret == UNZ_OK){
        Traza::print("Uncompressed size is in bytes", zip_file_info.uncompressed_size, W_PARANOIC);
        rom_size = zip_file_info.uncompressed_size;
        retorno.romsize = rom_size;
    }

    ret = unzOpenCurrentFile(*myZip);
    if (ret != UNZ_OK){
        Traza::print("Unable to open file from zip archive.", W_ERROR);
        return(retorno);
    }
    
    if (uncompress){
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
    } else {
        retorno.rutaEscritura = filename;
    }

    return retorno;
}




