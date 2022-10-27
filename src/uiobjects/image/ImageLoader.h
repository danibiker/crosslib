/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ImageLoader.h
 * Author: Ryuk
 *
 * Created on 6 de abril de 2020, 17:24
 */

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <uiobjects/Dirutil.h>
#include "../uiimglist.h"
#include "uiobjects/common/Icogestor.h"
#include "uiobjects/Constant.h"
#include <uiobjects/gmutex.h>

class ImageLoader {
public:
    ImageLoader();
    virtual ~ImageLoader();
    uint32_t loadVisible();
    UIImgList *getListImages(){return listImages;}
    void setListImages(UIImgList *var){
        listImages = var;
    }
    void setScreen(SDL_Surface **var){
        screen = *var;
    }

    void setGestorIconos(IcoGestor **var){
        gestorIconos = *var;
    }
    uint32_t loadImage();
    void setImagetoload(int imagetoload);
    int getImagetoload() const;
    int getEstado() const;
    int estado;

private:
    UIImgList *listImages;
    SDL_Surface *screen;
    int imagetoload;
    bool loadFromFileToCache(int pos);
    static GMutex mutex;
    IcoGestor *gestorIconos;
    void createIconPrev(string ruta, UIPicture *pict);
    void previsualizeContentInDir(string ruta, UIPicture *pict);

};

#endif /* IMAGELOADER_H */

