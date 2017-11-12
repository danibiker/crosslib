#include "uilistcommon.h"

UIListCommon::UIListCommon(){
}

UIListCommon::~UIListCommon(){
}


/**
*
*/
void UIListCommon::inicializarObjeto(int objType){
    setObjectType(objType);
    posIniLista = 0;
    posFinLista = 0;
    posActualLista = 0;
    lastSelectedPos = 0;
    showScrollbar = true;
    showScrollbarAlways = false;
    enableLastSelected = false;
    letraPopup = 65;
    showLetraPopup = false;
    bgLetraPopup = false;
    nIconsHoriz = 3;
    listScheme = SCHEMELIST;
    smoothDraw = false;
    lastPosActualLista = 0;
    smoothSfc = NULL;
}


/**
*
*/
void UIListCommon::action(tEvento *evento){
    static unsigned long lastClick = 0;
    //Si hay un popup lanzado por este elemento, debe dejar de procesar los eventos
    if (popup == false){
        if (evento->isKey || evento->isJoy || (evento->isMouse &&
                                               (evento->mouse == MOUSE_BUTTON_WHEELDOWN || evento->mouse == MOUSE_BUTTON_WHEELUP)) ){

            Traza::print("UIList::action: " + this->getName(), W_DEBUG);

            this->setImgDrawed(false);
            //Para mostrar el popup que permite elegir una letra para posicionarnos en la lista
            if (evento->key == SDLK_F1 || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_R3)){
                this->showLetraPopup = true;
            } else if (this->showLetraPopup == true){

                if (evento->key == SDLK_ESCAPE || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)){
                    this->setShowLetraPopup(false);
                } else if (evento->key == SDLK_RIGHT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN)){
                    unsigned int letra = this->getLetraPopup() + 1;
                    if (letra > 90) letra = 65;
                    this->setLetraPopup(letra);
                } else if (evento->key == SDLK_LEFT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_UP)){
                    unsigned int letra = this->getLetraPopup();
                    if (letra == 65) letra = 90;
                    else letra--;
                    this->setLetraPopup(letra);
                } else if ( (evento->key == SDLK_RETURN && !(evento->keyMod & KMOD_LALT)) || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_A)){
                    this->setShowLetraPopup(false);
                    searchTextInList(this->getLetraPopup());
                }
            //Boton arriba
            } else if (getListScheme() == SCHEMELIST && (evento->key == SDLK_UP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_UP) || (evento->mouse == MOUSE_BUTTON_WHEELUP)) ){
               this->prevElemLista();
            //Boton abajo
            } else if (getListScheme() == SCHEMELIST && (evento->key == SDLK_DOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN) || evento->mouse == MOUSE_BUTTON_WHEELDOWN)){
                this->nextElemLista();
            } else if (getListScheme() == SCHEMEICONS && (evento->key == SDLK_LEFT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_LEFT) || (evento->mouse == MOUSE_BUTTON_WHEELUP)) ){
               this->prevElemLista();
            //Boton abajo
            } else if (getListScheme() == SCHEMEICONS && (evento->key == SDLK_RIGHT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_RIGHT) || evento->mouse == MOUSE_BUTTON_WHEELDOWN)){
                this->nextElemLista();
            //Boton intro
            } else if ((evento->key == SDLK_RETURN && !(evento->keyMod & KMOD_LALT)) || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_A)){
                Traza::print("RETURN: Checkeando el elemento: " + this->getName(), W_PARANOIC);
                this->setChecked(true);
                lastSelectedPos = this->posActualLista;
            } else if (evento->key == SDLK_PAGEDOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_R)){
                this->nextPageLista();
            } else if (evento->key == SDLK_PAGEUP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_L)){
                this->prevPageLista();
            } else if (evento->key == SDLK_END){
                this->FinLista();
            } else if (evento->key == SDLK_HOME){
                this->inicioLista();
            } else if (evento->key == SDLK_BACKSPACE || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)){
                unsigned int sizeList = this->getSize();
                this->posActualLista = (sizeList > 0) ? sizeList - 1 : 0;
                this->setChecked(true);
                Traza::print("BACKSPACE: Checkeando el elemento: " + this->getName(), W_PARANOIC);
            } else {
                //Con cualquier otro evento, buscamos el texto que ha introducido el usuario y navegamos hasta la
                //primera ocurrencia que encontremos en la lista
                if (searchTextInList(evento->key) == false){
                    //Buscamos una segunda vez pero desde el principio
                    //this->setPosActualLista(0);
                    //searchTextInList('\0');
                }
            }
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
           Traza::print("UIList::action: Mouse Pressed: " + this->getName(), W_PARANOIC);
           if (SDL_GetTicks() - lastClick < DBLCLICKSPEED){
               lastSelectedPos = this->posActualLista;
               lastClick = SDL_GetTicks() - DBLCLICKSPEED;  //reseteo del dobleclick
            } else {
                lastClick = SDL_GetTicks();
            }
           checkPos(*evento);
           this->setImgDrawed(false);
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_RELEASED){
           Traza::print("UIList::action: Mouse Released: " + this->getName(), W_PARANOIC);
           this->setImgDrawed(false);
        } else {
            Object::action(evento);
        }
    }
}

/**
*
*/
void UIListCommon::draw(){
    cout << "drawing List" << endl;
}

/**
*
*/
void UIListCommon::checkPos(tEvento evento){

    int tempY = evento.mouse_y;
    int tempX = evento.mouse_x;
    if (tempX >= this->getX() && tempX <= this->getX() + this->getW() &&
        tempY >= this->getY() && tempY <= this->getY() + this->getH()){
        //Se setea el checked cuando se hace doble click
        if (evento.isMousedblClick){
            //this->setChecked(true);
            this->setChecked(true);
        }
        int posClick = 0;

        if (getListScheme() == SCHEMEICONS){
            const int elemWidth = this->getW() / getNIconsHoriz();
            //const int elemHeight = this->getH() / getNIconsHoriz();
            posClick = (tempX - this->getX() - INPUTCONTENT) / elemWidth;

        } else {
            posClick = (tempY - this->getY() - INPUTCONTENT) / Constant::getMENUSPACE();
        }

        if (this->getPosIniLista() + posClick < getSize()){
            this->setPosActualLista(this->getPosIniLista() + posClick);
        }
    }
}



/**
* Funcion para buscar un elemento en la lista conforme vamos tecleando
* el texto mientras la lista est� con el foco. De esta forma se puede
* buscar el elemento que sea.
*/
bool UIListCommon::searchTextInList(int key){
    return false;
}

/**
*
*/
void UIListCommon::nextElemLista(){
    lastPosActualLista = posActualLista;
    unsigned int sizeList = this->getSize();
    if (sizeList >= 0){
        if ( posActualLista + 1 < sizeList ){
            posActualLista++;
        } else {
            posActualLista = sizeList - 1;
        }
        calcularScrPos();
    }
}

/**
*
*/
void UIListCommon::prevElemLista(){
    lastPosActualLista = posActualLista;
    if ( posActualLista > 0 ){
        posActualLista--;
    } else {
        posActualLista = 0;
    }
    calcularScrPos();
}

/**
*
*/
void UIListCommon::prevPageLista(){
    unsigned int elemVisibles = getElemVisibles();
    for (unsigned int i=0; i < elemVisibles; i++)
        prevElemLista();
}

/**
*
*/
void UIListCommon::nextPageLista(){
    unsigned int elemVisibles = getElemVisibles();
    for (unsigned int i=0; i < elemVisibles; i++)
        nextElemLista();
}

/**
*
*/
void UIListCommon::inicioLista(){
    while (posActualLista > 0)
        prevPageLista();
}

/**
*
*/
void UIListCommon::FinLista(){
    while (posActualLista < this->getSize() - 1)
        nextPageLista();
}

/**
*
*/
unsigned int UIListCommon::getSize(){
    return 0;
}

/**
*
*/
unsigned int UIListCommon::getElemVisibles(){
    if (getListScheme() == SCHEMEICONS){
        return nIconsHoriz;
    } else if (this->getH() >= 0 && Constant::getMENUSPACE() > 0)
        return this->getH() / Constant::getMENUSPACE();
    else
        return 0;
}

/**
*
*/
void UIListCommon::calcularScrPos(){
    unsigned int elemVisibles = getElemVisibles();
    unsigned int sizeList = this->getSize();
    
    if (getListScheme() == SCHEMEICONS){
        if (this->getH() > 0 && this->getW() > 0){
//            Traza::print("elemVisibles",elemVisibles,W_DEBUG);
//            Traza::print("posIniLista1",posIniLista,W_DEBUG);
//            Traza::print("posFinLista1",posFinLista,W_DEBUG);
//            Traza::print("posActualLista1",posActualLista,W_DEBUG);

            //Mas comprobaciones por si la posicion actual no es correcta
            if (posFinLista < posActualLista )
                posFinLista = posActualLista;

            if (posIniLista > posActualLista)
                posIniLista = posActualLista;

            if (elemVisibles % 2 != 0 && elemVisibles > 1){
                //Impares
                unsigned int lados = (elemVisibles - 1) / 2;

                if (posActualLista >= lados){
                    posIniLista = posActualLista - lados;
                    if (posActualLista + lados + 1 > sizeList && sizeList > 0){
                        posFinLista = sizeList > 0 ? sizeList - 1 : 0;
                    } else {
                        posFinLista = posActualLista + lados;
                    }
                } else {
                    posIniLista = 0;
                    if (elemVisibles > sizeList){
                        posFinLista = sizeList > 0 ? sizeList - 1 : 0;
                    } else {
                        posFinLista = elemVisibles - 1;
                    }
                }
//                Traza::print("posIniLista2",posIniLista,W_DEBUG);
//                Traza::print("posFinLista2",posFinLista,W_DEBUG);
//                Traza::print("posActualLista2",posActualLista,W_DEBUG);
            }
        }
    }

    //Si los elementos visibles es par, aunque hayamos seleccionado el esquema de iconos, no centramos
    if ( (this->getH() > 0 || (getListScheme() == SCHEMEICONS && elemVisibles % 2 == 0))
            && !(getListScheme() == SCHEMEICONS && elemVisibles % 2 != 0) )
    {
        unsigned int elemVisibles = getElemVisibles();
//        Traza::print("Redimensionando: " + Constant::TipoToStr(this->getH() / Constant::getMENUSPACE()) + " elementos para un height: " + Constant::TipoToStr(this->getH()), W_PARANOIC);
//        Traza::print("posActualLista: " + Constant::TipoToStr(posActualLista) + " posFinLista: " + Constant::TipoToStr(posFinLista), W_PARANOIC);
        if (sizeList > 0 && posIniLista < posFinLista){
            //Movemos los limites iniciales y finales hasta que la posicion actual quede entre
            //los dos limites
            do{
                if (posActualLista > posFinLista){
                    posIniLista++;
                    posFinLista++;

                } else if (posActualLista < posIniLista){
                    posIniLista--;
                    posFinLista--;
                }
            }while( (posActualLista > posFinLista || posActualLista < posIniLista) );

            if (posFinLista < posActualLista )
                posFinLista = posActualLista;

            if (posIniLista > posActualLista)
                posIniLista = posActualLista;

        } else if (posIniLista == posFinLista && posActualLista < sizeList){
            posIniLista = posActualLista;
            posFinLista = posActualLista;
        } else {
            //En caso de que no cuadren las posiciones, reseteamos todo
            posActualLista = 0;
            posIniLista = 0;
            posFinLista = 0;
        }

        //Comprobamos que la posicion final sea lo que realmente se puede mostrar
        if (sizeList <= elemVisibles && sizeList > 0){
            posFinLista = sizeList - 1;
        } else if (sizeList > elemVisibles && elemVisibles > 0){
            posFinLista = posIniLista + elemVisibles - 1 ;
        }

        //Comprobamos finalmente que la posicion seleccionada en la lista sea correcta. Si hemos
        //redimensionado la ventana y lo seleccionado cae fuera, movemos el campo seleccionado al
        //ultimo registro que se puede mostrar
        if (posActualLista > posIniLista + elemVisibles - 1 && posIniLista + elemVisibles > 0){
            posActualLista = posIniLista + elemVisibles - 1;
        }
    }

//    Traza::print("posIniLista: " + Constant::TipoToStr(posIniLista), W_PARANOIC);
//    Traza::print("posFinLista: " + Constant::TipoToStr(posFinLista), W_PARANOIC);
}



/**
* Por defecto obtiene el valor de la primera columna de la fila que se le
* indica por parametro
*/
string UIListCommon::getValue(int row){
    return "";
}

/**
* Por defecto obtiene el destino de la primera columna de la fila que se le
* indica por parametro
*/
int UIListCommon::getDestino(int row){
    return 0;
}

/**
*
*/
void UIListCommon::resizeLista(unsigned int tam){
        if (posFinLista >= tam){
            posFinLista = (tam > 0) ? tam -1 : 0;
            posIniLista = (posFinLista > getElemVisibles()) ? (posFinLista - getElemVisibles()) : 0;
            if (posActualLista > posFinLista){
                posActualLista = 0;
                lastPosActualLista = 0;
            }

        }
        posIniLista = posIniLista;
}

/**
*
*/
void UIListCommon::clearLista(){
        posFinLista = 0;
        posIniLista = 0;
        posActualLista = 0;
        lastPosActualLista = 0;
}

