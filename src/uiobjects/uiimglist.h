/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UIImgList.h
 * Author: Ryuk
 *
 * Created on 4 de abril de 2020, 11:46
 */

#ifndef IOIMGLIST_H
#define IOIMGLIST_H

#include "uilistgroup.h"

typedef enum {
    IMGGROUPMODE = 0, IMGTHUMBMODE
} enumMode;

class UIImgList : public UIListGroup {
public:
    UIImgList();
    virtual ~UIImgList();
    unsigned int getElemVisibles();
    void checkPos(tEvento evento);
    void setPrevImgWidth(int prevImgWidth);
    int getPrevImgWidth() const;
    void setPrevImgHeight(int prevImgHeight);
    int getPrevImgHeight() const;
    void setMode(int mode);
    int getMode() const;
    void action(tEvento *evento);
    bool nextRow();
    bool prevRow();
    bool nextElement(int);
    bool prevElement(int);
    bool prevPage(int positions);
    bool nextPage(int positions);
    bool goToEnd();
    bool goToIni();
    void eventoResize(tEvento *evento);
    void setReloadImages(bool var);
    bool isReloadImages();
    void setLastEnd(unsigned int lastEnd);
    unsigned int getLastEnd() const;
    void setLastIni(unsigned int lastIni);
    unsigned int getLastIni() const;
    void clearPrevImageCache(bool force = false);

private:
    int prevImgHeight;
    int prevImgWidth;
    int mode;
    bool reloadImages;
    unsigned int lastIni;
    unsigned int lastEnd;
};

#endif /* IOIMGLIST_H */

