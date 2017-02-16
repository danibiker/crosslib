#include "uipopupmenu.h"

/**
*
*/
UIPopupMenu::UIPopupMenu(){
   setObjectType(GUIPOPUPMENU);
   callerPopup = NULL;
   font = NULL;
   autosize = false;
   maxElemW = 0;
}

/**
*
*/
UIPopupMenu::~UIPopupMenu(){

}

/**
*
*/
void UIPopupMenu::action(tEvento *evento){

    if (evento->isKey || evento->isJoy || (evento->isMouse && evento->mouse_state == SDL_PRESSED &&
                                           (evento->mouse == MOUSE_BUTTON_WHEELDOWN || evento->mouse == MOUSE_BUTTON_WHEELUP)) ){

        Traza::print("UIPopupMenu::action: " + this->getName(), W_DEBUG);

        this->setImgDrawed(false);
        if (evento->key == SDLK_UP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_UP) || (evento->mouse == MOUSE_BUTTON_WHEELUP) ){
           this->prevElemLista();
        //Boton abajo
        } else if (evento->key == SDLK_DOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN) || evento->mouse == MOUSE_BUTTON_WHEELDOWN){
            this->nextElemLista();
        //Boton intro
        } else if ((evento->key == SDLK_RETURN && !(evento->keyMod & KMOD_LALT)) || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_A)){
            Traza::print("RETURN: Checkeando el elemento: " + this->getName(), W_DEBUG);
            this->setChecked(true);
            closePopup();
        } else if (evento->key == SDLK_PAGEDOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_R)){
            this->nextPageLista();
        } else if (evento->key == SDLK_PAGEUP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_L)){
            this->prevPageLista();
        } else if (evento->key == SDLK_END){
            this->FinLista();
        } else if (evento->key == SDLK_HOME){
            this->inicioLista();
        } else {
            //Con cualquier otro evento, buscamos el texto que ha introducido el usuario y navegamos hasta la
            //primera ocurrencia que encontremos en la lista
            if (searchTextInList(evento->key) == false){
                //Buscamos una segunda vez pero desde el principio
                //this->setPosActualLista(0);
                //searchTextInList('\0');
            }
        }
    }
    else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
       Traza::print("UIPopupMenu::action: Mouse Left: " + this->getName(), W_DEBUG);
       checkPosAndExit(*evento);
       this->setImgDrawed(false);
    } else if (evento->isMouseMove){
       checkPos(*evento);
    }
}

/**
*
*/
void UIPopupMenu::clearLista(){
    UIList::clearLista();
    maxElemW = 0;
}

/**
*
*/
void UIPopupMenu::addElemLista(string name, string value, int ico, int destino){

    int elemW = fontStrLen(name);
    if (autosize && elemW > 0){
        if (getSize() > 0){
            //Obtenemos el tamanyo de la lista y le sumamos 1 para que calcule correctamente
            //la posicion final (posFinLista) al llamar a addElemLista
            this->setH((Constant::getMENUSPACE()) * (getSize() + 1) + INPUTCONTENT);
        } else {
            this->setH(Constant::getMENUSPACE() + INPUTCONTENT);
        }

        if (elemW > maxElemW){
            this->setW(INPUTCONTENT + ICOSPACE + elemW + INPUTCONTENT * 2);
            maxElemW = elemW;
        } else if (maxElemW <= 0){
            maxElemW = Constant::getMENUSPACE();
        }
    }

    UIList::addElemLista(name, value, ico, "", destino);

}

/**
*
*/
void UIPopupMenu::addElemLista(string name, string value){
    addElemLista(name, value, -1);
}

/**
*
*/
void UIPopupMenu::addElemLista(string name, string value, int ico){
    addElemLista(name, value, ico, -1);
}

/**
*
*/
void UIPopupMenu::draw(){
    cout << "drawing UIPopupMenu" << endl;
}

/**
* Solo checkea la posicion del raton y selecciona el elemento de la lista
*/
void UIPopupMenu::checkPos(tEvento evento){
    int tempX = evento.mouse_x;
    int tempY = evento.mouse_y;

    if (tempX >= this->getX() && tempX <= this->getX() + this->getW() && tempY >= this->getY() && tempY <= this->getY() + this->getH()){
        //Se setea el checked cuando se hace un unico click
        if (evento.isMouse && evento.mouse == MOUSE_BUTTON_LEFT && evento.mouse_state == SDL_PRESSED){
            this->setChecked(true);
            closePopup();
        }
        int posClick = (tempY - this->getY() - INPUTCONTENT) / Constant::getMENUSPACE();
        //Si la posicion es distinta a la actual, refrescamos la lista
        if (this->getPosActualLista() != this->getPosIniLista() + posClick){
            this->setImgDrawed(false);
            this->setPosActualLista(this->getPosIniLista() + posClick);
        }
    }
}

/**
* Checkea la posicion del raton, selecciona el elemento de la lista. Si se pulsa en alguna parte fuera del menu
* se
*/
void UIPopupMenu::checkPosAndExit(tEvento evento){
    int tempX = evento.mouse_x;
    int tempY = evento.mouse_y;

    if (tempX >= this->getX() && tempX <= this->getX() + this->getW() && tempY >= this->getY() && tempY <= this->getY() + this->getH()){
        checkPos(evento);
    } else {
        closePopup();
    }
}

/**
*
*/
void UIPopupMenu::closePopup(){
    this->setVisible(false);
    this->setFocus(false);

    if (this->getCallerPopup() != NULL){
        this->getCallerPopup()->setEnabled(true);
        this->getCallerPopup()->setImgDrawed(false);
        this->getCallerPopup()->setPopup(false);
        this->getCallerPopup()->setFocus(true);
    }
}

/**
*
*/
int UIPopupMenu::fontStrLen(string str){
    int pixelSize = 0;
    if (font != NULL){
        TTF_SizeText(font, str.c_str(),&pixelSize,NULL );
    }
    return pixelSize;
}
