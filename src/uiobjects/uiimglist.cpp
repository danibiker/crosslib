/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UIImgList.cpp
 * Author: Ryuk
 * 
 * Created on 4 de abril de 2020, 11:46
 */

#include "uiimgList.h"

UIImgList::UIImgList() {
    inicializarObjeto(GUILISTIMG);
    prevImgWidth = 105;
    prevImgHeight = 116;
    mode = IMGTHUMBMODE;
    reloadImages = false;
    lastIni = 0;
    lastEnd = 0;
}

UIImgList::~UIImgList() {
    
}

/**
 * Overriding the parent method
 * @return 
 */
unsigned int UIImgList::getElemVisibles(){
    unsigned int ret = 0;
    
    if (mode == IMGTHUMBMODE && prevImgWidth > 0 && prevImgHeight > 0){
//        int horizontalElems = this->getW() / prevImgWidth;
//        int verticalElems = this->getH() / prevImgHeight;
        
        int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
        int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
        
        ret = horizontalElems * verticalElems;
    } else if (mode == IMGGROUPMODE){
        ret = UIListGroup::getElemVisibles();
    }
    return ret;
}

/**
 * 
 * @param evento
 */
void UIImgList::checkPos(tEvento evento){
    if (mode == IMGGROUPMODE){
        UIListGroup::checkPos(evento);
    } else if (mode == IMGTHUMBMODE && prevImgWidth > 0 && prevImgHeight > 0){
        int my = evento.mouse_y;
        int mx = evento.mouse_x;
        t_region regionPantalla = {this->getX(), this->getY(), this->getW(), this->getH()};
        int posPulsada = getImgGestor()->getPosThumb(mx - this->getX(), my - this->getY(), prevImgWidth, prevImgHeight, regionPantalla);
        this->setPosActualLista(posPulsada + this->getPosIniLista());
        
        //Se setea el checked cuando se hace doble click
        if (evento.isMousedblClick){
            if (lastSelectedPos == this->posActualLista){
                this->setChecked(true);
            }
            lastSelectedPos = this->posActualLista;
            lastClick = SDL_GetTicks() - DBLCLICKSPEED;  //reseteo del dobleclick
        } else if (evento.isMouse && evento.mouse == MOUSE_BUTTON_LEFT && evento.mouse_state == SDL_PRESSED){
            lastSelectedPos = this->posActualLista;
            lastClick = SDL_GetTicks();
        }
    }
}

/**
 * 
 * @param evento
 */
void UIImgList::action(tEvento *evento){
    if (popup == false && mode == IMGTHUMBMODE && prevImgWidth > 0 && prevImgHeight > 0){
        if (evento->resize || evento->isKey || evento->isJoy || (evento->isMouse &&
                                               (evento->mouse == MOUSE_BUTTON_WHEELDOWN || evento->mouse == MOUSE_BUTTON_WHEELUP) 
                                                && evento->mouse_state == 1
                ) ){
                                         
            Traza::print("UIImgList::action: " + this->getName(), W_DEBUG);
            
            if ((evento->key == SDLK_LEFT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_LEFT))){
               this->setImgDrawed(prevElement(1));
               reloadImages = !this->getImgDrawed();
            } else if ((evento->key == SDLK_RIGHT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_RIGHT))){
                this->setImgDrawed(nextElement(1));
                reloadImages = !this->getImgDrawed();
            } else if ((evento->key == SDLK_DOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN))){
                this->setImgDrawed(nextRow());
                reloadImages = !this->getImgDrawed();
            } else if ((evento->key == SDLK_UP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_UP))){
                this->setImgDrawed(prevRow());
                reloadImages = !this->getImgDrawed();
            } else if (evento->mouse == MOUSE_BUTTON_WHEELUP || evento->key == SDLK_PAGEUP){
                this->setImgDrawed(prevPage(0));
                reloadImages = !this->getImgDrawed();
            } else if (evento->mouse == MOUSE_BUTTON_WHEELDOWN || evento->key == SDLK_PAGEDOWN){
                this->setImgDrawed(nextPage(0));
                reloadImages = !this->getImgDrawed();
            } else if (evento->key == SDLK_HOME){
                this->setImgDrawed(goToIni());
                reloadImages = !this->getImgDrawed();
            } else if (evento->key == SDLK_END){
                this->setImgDrawed(goToEnd());
                reloadImages = !this->getImgDrawed();
            } else if (evento->resize){
                eventoResize(evento);
                this->setImgDrawed(false);
                reloadImages = !this->getImgDrawed();
            } else {
                UIListCommon::action(evento);
            }
            
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
            Traza::print("UIImgList::action: Mouse Pressed: " + this->getName(), W_DEBUG);
            checkPos(*evento);
            setImgDrawed(true);
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_RELEASED){
            Traza::print("UIImgList::action: Mouse Released: " + this->getName(), W_DEBUG);
        } else {
            UIListCommon::action(evento);
        }
    } else if (mode == IMGGROUPMODE){
        UIListGroup::action(evento);
    } 
}

/**
 * 
 * @param evento
 */
void UIImgList::eventoResize(tEvento *evento){
    int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
    int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    int total = horizontalElems * verticalElems;
    
    Traza::print(string("Redimensionando a w: ") + Constant::TipoToStr(this->getW()) 
        + string(", h: ") + Constant::TipoToStr(this->getH()), W_DEBUG);
    
    int ini = 0;
    int fin = 0;
    
    if (horizontalElems > 0){
        
        ini = getPosActualLista();
        fin = getPosActualLista();
        
        do{
            if (ini > 0){
                ini--;
            }
            if (fin - ini < total - 1 && fin < this->getSize() - 1){
                fin++;
            }
        } while (fin - ini < total - 1 && fin - ini < this->getSize() - 1);
          
        int resto = 0;
        
//        if (fin >= (this->getSize() / horizontalElems) * horizontalElems &&
//            fin < (this->getSize() / horizontalElems) * horizontalElems + horizontalElems - 1
//            && ini > 0){
//            resto = fin % horizontalElems;
//        }
        
        while(ini % horizontalElems != 0 && ini < getPosActualLista()){
            ini++;
            if (fin < getSize() - 1) 
                fin++;
        }
        

        setPosIniLista(ini);
        setPosFinLista(fin);
        
        Traza::print("resto: ", resto, W_DEBUG);
    
//    double percentTotal = getPosFinLista() - getPosIniLista();
//    double percentLeft = 0;
//    double percentRight = 0;
//    
//    if (percentTotal > 1){
//        percentLeft = (getPosActualLista() - getPosIniLista()) / percentTotal;
//        percentRight = (getPosFinLista() - getPosActualLista()) / percentTotal;
//        int elemsLeft = percentLeft * total - 1;
//        int elemsRight = percentRight * total;
//        
//        int ini = getPosActualLista() > elemsLeft ? getPosActualLista() - elemsLeft : 0;
//        int fin = ini + total - 1 < this->getSize() ? ini + total - 1 : this->getSize() - 1;
//        int resto = fin % horizontalElems;
//        
//        
////        if (fin == this->getSize() - 1 && ini > 0 && getPosActualLista() > this->getSize() - horizontalElems + resto -1){
////            setPosIniLista(ini + resto);
////            setPosFinLista(fin);
////        } else {
//            setPosIniLista(ini);
//            setPosFinLista(fin);
////        }
        
        
    } else {
        setPosActualLista(0);
        setPosIniLista(0);
        setPosFinLista(total > this->getSize() ? this->getSize() - 1 : total -1);
    }
}

/**
 * 
 * @return 
 */
bool UIImgList::prevPage(int positions){
    int verticalElems = positions > 0 ? positions : getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    unsigned int posIni = getPosIniLista();
    
    if (verticalElems > 0){
        for (int i=0; i < verticalElems; i++){
            prevRow();
        }
    }
    return posIni == getPosIniLista();
}

/**
 * 
 * @return 
 */
bool UIImgList::nextPage(int positions){
    int verticalElems = positions > 0 ? positions : getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    unsigned int posIni = getPosIniLista();
    
    if (verticalElems > 0){
        for (int i=0; i < verticalElems; i++){
            nextRow();
        }
    }
    return posIni == getPosIniLista();
}

/**
 * 
 * @param positions
 */
bool UIImgList::prevElement(int positions){
    int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
    int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    int total = horizontalElems * verticalElems;
    int allowedJump = getPosActualLista() - positions >= 0 ? positions : 1;
    bool ret = false;
    
    if (horizontalElems > 0){
        if (getPosActualLista() - allowedJump >= 0 && getPosIniLista() > getPosActualLista() - allowedJump){
            setPosActualLista(getPosActualLista() - allowedJump);
            setPosFinLista((getPosActualLista() / horizontalElems) * horizontalElems + total - 1);
            //setPosFinLista(getPosActualLista() - allowedJump);
            setPosIniLista(getPosFinLista() - total + 1 >= 0 ? getPosFinLista() - total + 1 : 0);
            total = total;
        } else if (getPosActualLista() >= allowedJump){
            setPosActualLista(getPosActualLista() - allowedJump);
            ret = true;
        } else {
            setPosActualLista(0);
            ret = true;
        }
    }
    
    return ret;
}

/**
 * 
 * @param positions
 */
bool UIImgList::nextElement(int positions){
    int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
    int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    int total = horizontalElems * verticalElems;
    int allowedJump = getPosActualLista() + positions < this->getSize() ? positions : 1;
    bool ret = false;
    
    if (horizontalElems == 1 && getPosActualLista() + allowedJump < this->getSize()){
        setPosActualLista(getPosActualLista() + allowedJump);
        setPosIniLista(getPosActualLista());
        setPosFinLista(getPosActualLista());
    } else if (horizontalElems > 0){
        if (getPosActualLista() + allowedJump > getPosFinLista() && getPosFinLista() < this->getSize() - 1 
                && getPosActualLista() + allowedJump < this->getSize()){
            setPosActualLista(getPosActualLista() + allowedJump);
            //setPosIniLista( ((getPosActualLista() / horizontalElems) - 1) *  horizontalElems);
            //setPosIniLista( ((getPosActualLista() / horizontalElems)) *  horizontalElems);
            setPosIniLista(getPosIniLista() + horizontalElems);
            setPosFinLista(getPosIniLista() + total - 1 < this->getSize() ? getPosIniLista() + total - 1 : this->getSize() - 1);
            
//            if (getPosFinLista() == this->getSize() - 1){
//                setPosIniLista(getPosActualLista() - horizontalElems - allowedJump + 1);
//            }
        }
        else if (getPosActualLista() < this->getSize() - 1){
            setPosActualLista(getPosActualLista() + allowedJump);
            ret = true;
        }
    }
    return ret;
}

/**
 * 
 */
bool UIImgList::nextRow(){
    int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
    int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    bool ret = false;
    
    if (horizontalElems > 0){
        int posPantalla = getPosActualLista() - getPosIniLista();
        if (this->getPosActualLista() + horizontalElems < this->getSize()){
            if (this->getPosActualLista() + horizontalElems > this->getPosFinLista()){
                setPosIniLista(getPosIniLista() + horizontalElems);
                setPosFinLista(getPosFinLista() + horizontalElems >= getSize() ? getSize() - 1 : getPosFinLista() + horizontalElems);
                setPosActualLista(this->getPosIniLista() + posPantalla);
            } else {
                setPosActualLista(this->getPosActualLista() + horizontalElems);
                ret = true;
            }
        } else if (this->getSize() % horizontalElems > 0 && this->getSize() > horizontalElems * verticalElems){
            setPosFinLista(getSize() - 1);
            setPosActualLista(getPosFinLista());
            int ini = getPosFinLista() - verticalElems * horizontalElems + this->getSize() % horizontalElems + 1;
            setPosIniLista(ini < 0 ? 0 : ini);
        }
    }
    return ret;
}

/**
 * 
 * @return 
 */
bool UIImgList::goToEnd(){
    int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
    int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    unsigned int posIni = getPosIniLista();
    
    setPosFinLista(getSize() - 1);
    setPosActualLista(getPosFinLista());
    int ini = getPosFinLista() - verticalElems * horizontalElems + this->getSize() % horizontalElems + 1;
    setPosIniLista(ini < 0 ? 0 : ini);
    return posIni == getPosIniLista();
}

/**
 * 
 * @return 
 */
bool UIImgList::goToIni(){
    int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
    int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    unsigned int posIni = getPosIniLista();
    
    setPosActualLista(0);
    setPosIniLista(0);
    setPosFinLista(this->getSize() > horizontalElems * verticalElems ? horizontalElems * verticalElems - 1 : this->getSize() - 1);
    return posIni == getPosIniLista();
}

/**
 * 
 * @return 
 */
bool UIImgList::prevRow(){
    int horizontalElems = getImgGestor()->calcMaxX(prevImgWidth, this->getW());
    int verticalElems = getImgGestor()->calcMaxY(prevImgHeight, this->getH());
    bool ret = false;
    if (horizontalElems > 0){
        if(getPosActualLista() - horizontalElems < getPosIniLista() && getPosActualLista() > horizontalElems){
           setPosActualLista(getPosActualLista() - horizontalElems);
           setPosIniLista(getPosIniLista() > horizontalElems ? getPosIniLista() - horizontalElems : 0);
           setPosFinLista(getPosIniLista() + horizontalElems * verticalElems - 1);
        } else if (getPosActualLista() <= horizontalElems){
            setPosActualLista(0);
            setPosIniLista(0);
            setPosFinLista(this->getSize() > horizontalElems * verticalElems ? horizontalElems * verticalElems - 1 : this->getSize() - 1);
        } else {
            setPosActualLista(getPosActualLista() - horizontalElems); 
            ret = true;
        }
    }
    return ret;
}

/**
 * Evita tener en memoria todas las imagenes de la lista cacheada. 
 * Asi se fuerza a recargar las imagenes que se muestran en pantalla en cache
 * 
 * Esto solo funciona correctamente si se ha podido guardar una miniatura en disco
 * duro. Sino tardaria enormemente luego en volver a recargar las imagenes
 * @param listImages
 */
void UIImgList::clearPrevImageCache(bool force){
    Dirutil dir;
    if (getLastIni() != getLastEnd() && getLastEnd() > 0){
        for (unsigned int i = getLastIni(); i <= getLastEnd() && i < getSize(); i++){
            UIPicture* pict = getRow(i)->GetUipicture();
            if (pict != NULL && pict->surfaceCache != NULL && (dir.existe(DIR_PREV + tempFileSep + getValue(i)) || force) ){
                SDL_FreeSurface(pict->surfaceCache);
                pict->surfaceCache = NULL;
                delete pict;
                pict = NULL;
                getRow(i)->SetUipicture(NULL);
            }
        }
    }
}

void UIImgList::setPrevImgWidth(int prevImgWidth) {
    this->prevImgWidth = prevImgWidth;
}

int UIImgList::getPrevImgWidth() const {
    return prevImgWidth;
}

void UIImgList::setPrevImgHeight(int prevImgHeight) {
    this->prevImgHeight = prevImgHeight;
}

int UIImgList::getPrevImgHeight() const {
    return prevImgHeight;
}

void UIImgList::setMode(int mode) {
    this->mode = mode;
}

int UIImgList::getMode() const {
    return mode;
}

void UIImgList::setReloadImages(bool var) {
    this->reloadImages = var;
}

bool UIImgList::isReloadImages() {
    return reloadImages;
}

void UIImgList::setLastEnd(unsigned int lastEnd) {
    this->lastEnd = lastEnd;
}

unsigned int UIImgList::getLastEnd() const {
    return lastEnd;
}

void UIImgList::setLastIni(unsigned int lastIni) {
    this->lastIni = lastIni;
}

unsigned int UIImgList::getLastIni() const {
    return lastIni;
}
