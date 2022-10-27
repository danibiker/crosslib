/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ImageLoader.cpp
 * Author: Ryuk
 *
 * Created on 6 de abril de 2020, 17:24
 */

#include "ImageLoader.h"
#include "common/Icogestor.h"

GMutex ImageLoader::mutex;

ImageLoader::ImageLoader() {
    listImages = NULL;
    estado = -1;
}

ImageLoader::~ImageLoader() {
}

uint32_t ImageLoader::loadImage(){
    mutex.Lock();
    estado = 1;
    Traza::print("Image Loaded", W_PARANOIC);
    if (listImages != NULL){
        if (getImagetoload() < listImages->getSize() && getImagetoload() >= 0){
            loadFromFileToCache(getImagetoload());
        }
        Traza::print("Image Loaded", W_PARANOIC);
    }
    estado = 0;
    mutex.Unlock();
    return 0;
}

void ImageLoader::setImagetoload(int imagetoload) {
    this->imagetoload = imagetoload;
}

int ImageLoader::getImagetoload() const {
    return imagetoload;
}

int ImageLoader::getEstado() const {
    return estado;
}
/**
 *
 * @return
 */
uint32_t ImageLoader::loadVisible(){
//    mutex.Lock();
    if (listImages != NULL){
        const unsigned int finLista = listImages->getPosFinLista();
        const unsigned int iniLista = listImages->getPosIniLista();
//        Traza::print("ThreadImgLoader. size: ",(listImages)->getSize(), W_PARANOIC);
        for (int i=iniLista; i <= finLista; i++ ){
            loadFromFileToCache(i);
        }
//        Traza::print("Images Loaded", W_PARANOIC);
    }
//    mutex.Unlock();
    return 0;
}

/**
 *
 * @param pos
 * @return
 */
bool ImageLoader::loadFromFileToCache(int pos){
    int ret = 0;
    Dirutil dir;

    if (listImages == NULL)
        return false;
    if (listImages->getRow(pos) == NULL){
        return false;
    }
    if (listImages->getRow(pos)->GetListGroupCol().size() <= 0){
        return false;
    }

    ListGroupCol* col = listImages->getRow(pos)->GetListGroupCol().at(0);
    UIPicture* pict = listImages->getRow(pos)->GetUipicture();
    string ruta = listImages->getValue(pos);
    bool loaded = false;
//    Traza::print("ThreadImgLoader: " + col->getValor(), W_PARANOIC);

    if (pict == NULL && !ruta.empty()){
        pict = new UIPicture();
        pict->getImgGestor()->setResize(true);
        pict->getImgGestor()->setEnabledMoveImg(false);
        pict->getImgGestor()->setFillBackgroundColour(false);
        pict->getImgGestor()->setBestfit(false);
        pict->getImgGestor()->setSmooth(true);
        pict->getImgGestor()->setBordeBottom(20);
        pict->getImgGestor()->setBordeLeft(3);
        pict->getImgGestor()->setBordeRight(3);
        pict->getImgGestor()->setBordeTop(5);
        pict->getImgGestor()->setCentradoX(true);
        pict->setW(listImages->getPrevImgWidth());
        pict->setH(listImages->getPrevImgHeight());
        Traza::print("Cargando imagen: " + ruta, W_PARANOIC);

        if (pict->isValidImage(ruta)){
            if (dir.existe(DIR_PREV + tempFileSep + ruta)){
                loaded = pict->loadImgFromFile(DIR_PREV + tempFileSep + ruta);
            } else {
                loaded = pict->loadImgFromFile(ruta);
                pict->getImgGestor()->setZoom(-3);
            }
        } else {
            createIconPrev(ruta, pict);
        }

        if (loaded){
            Traza::print("Cargada ok", W_PARANOIC);
            SDL_Surface *tmp = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, pict->getW(), pict->getH(), screen->format->BitsPerPixel,
                                                                                                             screen->format->Rmask,
                                                                                                             screen->format->Gmask,
                                                                                                             screen->format->Bmask,
                                                                                                             screen->format->Amask);
            t_region regionPantalla = {pict->getX(), pict->getY(), pict->getW(), pict->getH()};
            SDL_Rect *imgLocation = new SDL_Rect();
            bool surfaceOk = pict->getImgGestor()->drawImgMem(-1,pict->getW(), pict->getH(), regionPantalla, tmp, imgLocation);

            if (surfaceOk){
                pict->surfaceCache = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, imgLocation->w, imgLocation->h, screen->format->BitsPerPixel,
                                                                                                             screen->format->Rmask,
                                                                                                             screen->format->Gmask,
                                                                                                             screen->format->Bmask,
                                                                                                             screen->format->Amask);
                SDL_BlitSurface(tmp, imgLocation, pict->surfaceCache, NULL);
                //Guardamos la miniatura en el disco duro para cacheo mas rapido y menor memoria ram utilizada
                if (!dir.existe(DIR_PREV + tempFileSep + ruta)){
                    if (!dir.existe(DIR_PREV + tempFileSep)){
                        dir.createDir(DIR_PREV + tempFileSep);
                    }
                    pict->getImgGestor()->writeImg(DIR_PREV + tempFileSep + ruta, pict->surfaceCache);
                }
                listImages->setImgDrawed(false);
            }
            pict->setImgDrawed(true);
            pict->setSafeToDraw(true);
            pict->getImgGestor()->clearFile();
            SDL_FreeSurface(tmp);
        }
        listImages->getRow(pos)->SetUipicture(pict);
    }
    return loaded;
}

/**
 *
 * @param ruta
 * @param pict
 */
void ImageLoader::createIconPrev(string ruta, UIPicture *pict){

    Traza::print(ruta + " --> no es una imagen", W_PARANOIC);
    t_color fondo = listImages->getColor();
    pict->surfaceCache = SDL_CreateRGBSurface(SDL_SWSURFACE, pict->getW(), pict->getH(), screen->format->BitsPerPixel, rmask,
                gmask,
                bmask,
                amask);

    SDL_FillRect(pict->surfaceCache, NULL, SDL_MapRGB(pict->surfaceCache->format, fondo.r,fondo.g,fondo.b));

    int tipoPic = directory;
    Dirutil dir;
    if (!dir.isDir(ruta)){
        tipoPic = dir.findIcon(ruta.c_str());
    }
    gestorIconos->drawIcono(tipoPic, pict->surfaceCache, 0, 5, 5, pict->getW() - 10, pict->getH() - 30);

    if (tipoPic == directory) {
        previsualizeContentInDir(ruta, pict);
    }

    pict->setImgDrawed(true);
    pict->setSafeToDraw(true);
    listImages->setImgDrawed(false);
}

/**
 *
 * @param ruta
 * @param pict
 */
void ImageLoader::previsualizeContentInDir(string ruta, UIPicture *pict){
    Dirutil dirutil;
    vector <FileProps> *filelist = new vector<FileProps>();
    bool found = false;
    bool thumbFound = false;
    string imgToPrev;
    const string thumbFilename = "thumbs.jpg";

    if (ruta.compare(".") == 0 || ruta.compare("..") == 0)
         return;

    if (!dirutil.existe(ruta + tempFileSep + thumbFilename)){
        Traza::print("listando: " + ruta, W_PARANOIC);
        dirutil.listarDirSinOrdenMaxFiles(ruta.c_str(), NULL, filelist, ".jpg,.jpeg,.gif,.bmp", 100, 1);
        unsigned int numFiles = filelist->size();
        if (filelist != NULL && numFiles > 0){
            int i=0;
            //Recorremos la lista de ficheros y lo incluimos en el objeto de lista para mostrar los datos
            while (i < numFiles && !found){
                if (pict->isValidImage(filelist->at(i).filename)){
                    imgToPrev = filelist->at(i).filename;
                    found = true;
                }
                i++;
            }
        }
    } else {
        imgToPrev = thumbFilename;
        found = true;
        thumbFound = true;
    }

    //Traza::print("la imagen para previsualizar es: " + imgToPrev, W_PARANOIC);
    if (found){
        UIPicture *pictPrev = new UIPicture();
        pictPrev->getImgGestor()->setResize(thumbFound);
        pictPrev->getImgGestor()->setEnabledMoveImg(false);
        pictPrev->getImgGestor()->setFillBackgroundColour(false);
        pictPrev->getImgGestor()->setBestfit(false);
        pictPrev->getImgGestor()->setSmooth(false);
        pictPrev->getImgGestor()->setBordeBottom(20);
        pictPrev->getImgGestor()->setBordeLeft(2);
        pictPrev->getImgGestor()->setBordeRight(2);
        pictPrev->getImgGestor()->setBordeTop(2);
        pictPrev->getImgGestor()->setCentradoX(true);
        pictPrev->setW(listImages->getPrevImgWidth() * 0.9 );
        pictPrev->setH(listImages->getPrevImgHeight() * 0.9);
        //Traza::print("Cargando imagen: " + ruta, W_PARANOIC);
        bool loaded = pictPrev->loadImgFromFile(ruta + tempFileSep + imgToPrev);
        if (loaded){
            t_region regionPantalla = {pict->getX() + pict->getW() - pictPrev->getW(),
                                       pict->getY() + pict->getH() - pictPrev->getH(),
                                       pict->getW(), pict->getH()};
            SDL_Rect *imgLocation = new SDL_Rect();

            bool surfaceOk = pictPrev->getImgGestor()->drawImgMem(-1, pictPrev->getW(), pictPrev->getH(),
                    regionPantalla, pict->surfaceCache, imgLocation);

            if (imgToPrev.compare(thumbFilename) != 0){
                t_region regionPantalla2 = {0,0, imgLocation->w, imgLocation->h};
                pictPrev->getImgGestor()->writeImg(ruta + tempFileSep + thumbFilename, regionPantalla2, pict->surfaceCache->format);
            }
            delete imgLocation;
        }
        delete pictPrev;
    }
}
