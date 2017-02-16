#include "uiimgdownloader.h"

UIImgDownloader::UIImgDownloader(){
    quality = 85;
    bgColor = cBlanco;
}

UIImgDownloader::~UIImgDownloader(){
    //dtor
}

bool UIImgDownloader::download(string url, string strFileName, int w, int h){
    HttpUtil utilHttp;
    bool ret = utilHttp.download(url);

    if (!ret){
        Traza::print("Imposible conectar a: " + url, W_ERROR);
    } else if (w > 0 && h > 0){
        //Redimensionamos la imagen al tamanyo indicado
        SDL_Surface *mySurface = NULL;
        ImagenGestor imgGestor;
        imgGestor.loadImgFromMem(utilHttp.getRawData(), utilHttp.getDataLength(), &mySurface);
        if (mySurface != NULL){
            //Calculamos el tamanyo que tendra la imagen
            float relacion = imgGestor.relacion(mySurface, w, h);

            if (relacion != 0.){
                int imW = mySurface->w / relacion;
                int imH = mySurface->h / relacion;

                SDL_Surface *resizedSurface = SDL_CreateRGBSurface( SDL_SWSURFACE, imW, imH, mySurface->format->BitsPerPixel,mySurface->format->Rmask,mySurface->format->Gmask,mySurface->format->Bmask,mySurface->format->Amask);
                //SDL_FillRect(resizedSurface, NULL, SDL_MapRGB(mySurface->format, bgColor.r,bgColor.g,bgColor.b));
                imgGestor.updateImgScr(mySurface, resizedSurface);
                UIImageEncoder imgEncoder;
                imgEncoder.IMG_SaveJPG(strFileName.c_str(), resizedSurface, quality);
                SDL_FreeSurface(resizedSurface);

            } else {
                Traza::print("No se ha podido calcular la relacion de la imagen: " + url, W_ERROR);
            }

        } else {
            ret = false;
            Traza::print("El recurso especificado no es una imagen: " + url, W_ERROR);
        }
    } else {
        ret = utilHttp.writeToFile(strFileName);
    }
    return ret;
}

/**
*
*/
bool UIImgDownloader::downloadToJpg(string url, string strFileName){
    HttpUtil utilHttp;
    bool ret = utilHttp.download(url);

    if (!ret){
        Traza::print("Imposible conectar a: " + url, W_ERROR);
    } else {
        //Redimensionamos la imagen al tamanyo indicado
        SDL_Surface *mySurface = NULL;
        ImagenGestor imgGestor;
        imgGestor.loadImgFromMem(utilHttp.getRawData(), utilHttp.getDataLength(), &mySurface);
        if (mySurface != NULL){
            UIImageEncoder imgEncoder;
            imgEncoder.IMG_SaveJPG(strFileName.c_str(), mySurface, quality);
        } else {
            ret = false;
            Traza::print("El recurso especificado no es una imagen: " + url, W_ERROR);
        }
    }
    return ret;
}

bool UIImgDownloader::downloadToJpgAndThumbnail(string url, string strFileName, int w, int h){
    HttpUtil utilHttp;
    bool ret = utilHttp.download(url);

    if (!ret){
        Traza::print("Imposible conectar a: " + url, W_ERROR);
    } else if (w > 0 && h > 0){
        //Redimensionamos la imagen al tamanyo indicado
        SDL_Surface *mySurface = NULL;
        ImagenGestor imgGestor;
        imgGestor.loadImgFromMem(utilHttp.getRawData(), utilHttp.getDataLength(), &mySurface);
        if (mySurface != NULL){

            UIImageEncoder imgEncoder;
            Dirutil dir;

            imgEncoder.setDontFreeOnSave(true);

            string origFileName = dir.getFolder(strFileName) + Constant::getFileSep() +
                dir.getFileNameNoExt(strFileName) + "_full" + dir.getExtension(strFileName);

            imgEncoder.IMG_SaveJPG(origFileName.c_str(), mySurface, quality);

            //Calculamos el tamanyo que tendra la imagen
            float relacion = imgGestor.relacion(mySurface, w, h);

            if (relacion != 0.){
                int imW = mySurface->w / relacion;
                int imH = mySurface->h / relacion;

                SDL_Surface *resizedSurface = SDL_CreateRGBSurface( SDL_SWSURFACE, imW, imH, mySurface->format->BitsPerPixel,mySurface->format->Rmask,mySurface->format->Gmask,mySurface->format->Bmask,mySurface->format->Amask);
                //SDL_FillRect(resizedSurface, NULL, SDL_MapRGB(mySurface->format, bgColor.r,bgColor.g,bgColor.b));
                imgGestor.updateImgScr(mySurface, resizedSurface);
                imgEncoder.setDontFreeOnSave(false);
                imgEncoder.IMG_SaveJPG(strFileName.c_str(), resizedSurface, quality);
                SDL_FreeSurface(resizedSurface);

            } else {
                Traza::print("No se ha podido calcular la relacion de la imagen: " + url, W_ERROR);
            }

        } else {
            ret = false;
            Traza::print("El recurso especificado no es una imagen: " + url, W_ERROR);
        }
    } else {
        ret = utilHttp.writeToFile(strFileName);
    }
    return ret;
}
