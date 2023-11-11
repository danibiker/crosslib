#include "Menuobject.h"

Object tmenu_gestor_objects::foo;

/**
* Constructor
*/
tmenu_gestor_objects::tmenu_gestor_objects(int w, int h){
    clear();

    //Iniciamos a null todos los objetos
    for (int i=0; i<MAXOBJECTS; i++){
        arrayNewObj[i] = NULL;
    }

    setAreaObjMenu(w, h);
}

/**
* Destructor
*/
tmenu_gestor_objects::~tmenu_gestor_objects(){
    //TODO
    //Liberar el array de objetos
}

/**
*
*/
void tmenu_gestor_objects::clear(){
    size = 0;
    focusPos = -1;
}

/**
*
*/
void tmenu_gestor_objects::procAllEvent(tEvento evento){
    //Si hemos pulsado con el raton, marcamos el elemento como seleccionado
    if (evento.isMouse && (evento.mouse == MOUSE_BUTTON_LEFT || evento.mouse == MOUSE_BUTTON_RIGHT) && evento.mouse_state == SDL_PRESSED ){
        Object *object = getObjByXY(evento.mouse_x, evento.mouse_y);
        if (object != NULL){
            Traza::print("procEvent. Dando foco a: " + object->getName(), W_DEBUG);
            this->setFocus(object->getName());
        }
    }

    if ((evento.isKey && evento.key == SDLK_TAB) || ( evento.isJoy && evento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_SELECT)) ){
            Traza::print("procEvent. Buscamos siguiente foco", W_PARANOIC);
            findNextFocus();
    }

    for (int i = 0; i < size; i++){
        arrayNewObj[i]->action(&evento);
    }
}

/**
*
*/
void tmenu_gestor_objects::procEvent(tEvento evento){
    static string lastElement;
    //Si hemos pulsado con el raton, marcamos el elemento como seleccionado
    if (evento.isMouse && (evento.mouse == MOUSE_BUTTON_LEFT || evento.mouse == MOUSE_BUTTON_RIGHT) && evento.mouse_state == SDL_PRESSED ){
        Object *object = getObjByXY(evento.mouse_x, evento.mouse_y);
        if (object != NULL){
            Traza::print("procEvent. Dando foco a: " + object->getName(), W_PARANOIC);
            this->setFocus(object->getName());
        }
    } else if (evento.isMouseMove){
        Traza::print("procEvent. getObjByXY", W_PARANOIC);
        Object *object = getObjByXY(evento.mouse_x, evento.mouse_y);
        if (object != NULL
//            && (object->getObjectType() == GUIBUTTON
//            || object->getObjectType() == GUITEXTELEMENTSAREA)
        ){
            Traza::print("procEvent. setFocus: " + object->getName(), W_PARANOIC);
            //Damos el foco al elemento y lo marcamos para repintar
            this->setFocus(object->getName());
            Traza::print("procEvent. compare", W_PARANOIC);
            //Si despues de cambiar el foco, el elemento anterior era otro,
            //forzamos a que se repinte y actualice la imagen de los dos
            if (!lastElement.empty() && lastElement.compare(object->getName()) != 0){
                 Traza::print("procEvent. repaintObject", W_PARANOIC);
                 repaintObject(object->getName());
                 repaintObject(lastElement);
            }
            Traza::print("procEvent. lastElement", W_PARANOIC);
            //Guardamos el ultimo elemento que se selecciono
            lastElement = object->getName();
        } else if (object != NULL && !lastElement.empty()){
            Traza::print("tmenu_gestor_objects::procEvent. Reseteando objetos", W_PARANOIC);
            //Ponemos a todos los objetos con el foco a false
            clearFocus();
            //Volvemos a pintar el ultimo objeto
            repaintObject(lastElement);
            //Reseteamos el ultimo elemento seleccionado a vacio
            lastElement = "";
        }
    }

    if ((evento.isKey && evento.key == SDLK_TAB) || ( evento.isJoy && evento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_SELECT)) ){
        Traza::print("procEvent. Buscamos siguiente foco", W_PARANOIC);
        resetElements();
        findNextFocus();
    } else if (focusPos > -1 && arrayNewObj[focusPos] != NULL){
        Traza::print("Lanzando accion. focusPos: ", focusPos, W_PARANOIC);
        arrayNewObj[focusPos]->action(&evento);
    }
}

/**
* Restablece el elemento indicado para que se repinte en pantalla
*/
void tmenu_gestor_objects::repaintObject(string tname){
//    try{
        Object *lastObject = getObjByName(tname);
//        if (lastObject != NULL){
            lastObject->setImgDrawed(false);
            lastObject->setCursor(-1);
//        }
//     } catch (Excepcion &e){
//        Traza::print("No existe el elemento anterior en este menu: " + tname, W_DEBUG);
//     }
}

/**
*
*/
void tmenu_gestor_objects::resetElements(){
    Traza::print("tmenu_gestor_objects::resetElements", W_DEBUG);
    //Ponemos a todos los objetos a false
    for (int i=0;i<MAXOBJECTS;i++){
        if (arrayNewObj[i] != NULL){
            arrayNewObj[i]->setFocus(false);
            arrayNewObj[i]->setImgDrawed(false);
            arrayNewObj[i]->setCursor(-1);
        }
    }
}

/**
*
*/
void tmenu_gestor_objects::clearFocus(){
    Traza::print("tmenu_gestor_objects::clearFocus", W_DEBUG);
    //Ponemos a todos los objetos a false
    for (int i=0;i<MAXOBJECTS;i++){
        if (arrayNewObj[i] != NULL){
            arrayNewObj[i]->setFocus(false);
        }
    }
}

/**
* Obtiene un objeto por su nombre
*/
Object * tmenu_gestor_objects::getObjByName(string tname){
    int pos = 0;

    while (pos < size){
        if (tname.compare(arrayNewObj[pos]->getName()) == 0)
            return arrayNewObj[pos];
        else
            pos++;
    }
    Traza::print("tmenu_gestor_objects::getObjByName. No se ha encontrado el elemento: " + tname, W_PARANOIC);
    return &foo;
    //throw(Excepcion(ENULL, "tmenu_gestor_objects::getObjByName. No se ha encontrado el elemento: " + tname));
}

/**
* Obtiene un objeto por su posicion en el array de la pantalla
*/
Object * tmenu_gestor_objects::getObjByPos(int pos){

    if (pos < size && pos >= 0){
            return arrayNewObj[pos];
    } else {
        Traza::print("tmenu_gestor_objects::getObjByPos. No se ha encontrado el elemento en la pos: ",pos, W_PARANOIC);
        return &foo;
    }
    //else {
        //Traza::print("tmenu_gestor_objects::getObjByPos. No se ha encontrado el elemento en la pos: ",pos, W_ERROR);
        //throw(Excepcion(ENULL));
    //}
}


/**
* Obtiene un objeto por su posicion X, Y en la pantalla
*/
Object * tmenu_gestor_objects::getObjByXY(int tempX, int tempY){
    int i = size - 1;
    bool found = false;
    bool pendientePopup = false;
    Object *tempObj = NULL;

    //Comprobamos primero si hay algun objeto que haya lanzado un popup y este pendiente
    //de recoger algun valor
    while (i >= 0 && !pendientePopup){
        if (arrayNewObj[i] != NULL){
            if (arrayNewObj[i]->isVisible() && arrayNewObj[i]->isPopup()){
                pendientePopup = true;
            } else {
                i--;
            }
        } else {
            i--;
        }
    }
    //Reseteamos la cuenta
    i = size - 1;


    Traza::print("pendientePopup", pendientePopup, W_PARANOIC);
    if (!pendientePopup){
        //Primero comprobamos si el elemento que tiene el foco cae dentro de la posicion seleccionada
        //Esto es util por ejemplo en el caso de los combos que tengan una lista desplegable que
        //este por encima de un elemento.
        if (focusPos >= 0 && tempX >= arrayNewObj[focusPos]->getX() && tempX <= arrayNewObj[focusPos]->getX() + arrayNewObj[focusPos]->getW() && //Entra en lo ancho
                    tempY >= arrayNewObj[focusPos]->getY() && tempY <= (arrayNewObj[focusPos]->getY() + arrayNewObj[focusPos]->getH() //Entra en lo alto
                    + ((arrayNewObj[focusPos]->getObjectType() == GUICOMBOBOX && arrayNewObj[focusPos]->isChecked()) ?  Constant::getCOMBOLISTHEIGHT() : 0)) && //Entra en lo alto anyadiendo la lista del combo
                    arrayNewObj[focusPos]->isVisible() && arrayNewObj[focusPos]->isEnabled()){
            tempObj = arrayNewObj[focusPos];
        } else {

            //Buscamos en el resto de elementos
            while (i >= 0 && !found){
                if (arrayNewObj[i] != NULL){
//                    Traza::print(arrayNewObj[i]->getName()
//                    + " ," + Constant::TipoToStr(arrayNewObj[i]->getX())
//                    + "," + Constant::TipoToStr(arrayNewObj[i]->getY())
//                    + "," + Constant::TipoToStr(arrayNewObj[i]->getW())
//                    + "," + Constant::TipoToStr(arrayNewObj[i]->getH())
//                    + " - " + Constant::TipoToStr(tempX)
//                    + "," + Constant::TipoToStr(tempY), W_DEBUG);

                    if (tempX >= arrayNewObj[i]->getX() && tempX <= arrayNewObj[i]->getX() + arrayNewObj[i]->getW() &&
                        tempY >= arrayNewObj[i]->getY() && tempY <= arrayNewObj[i]->getY() + arrayNewObj[i]->getH() &&
                        arrayNewObj[i]->isVisible() && arrayNewObj[i]->isEnabled()){
                        tempObj = arrayNewObj[i];
                        found = true;
                        i--;
                    } else {
                        i--;
                    }
                } else {
                    i--;
                }
            }
        }
    }


    return tempObj;
}


/**
*
*/
Object * tmenu_gestor_objects::add(string tname, int ttype, int tx, int ty, int tw, int th, string tlabel, bool centered){

    if (size < MAXOBJECTS){
        //Nueva clase para almacenar objetos
        if (ttype == GUILABEL){
            arrayNewObj[size] = new UILabel();
        } else if (ttype == GUIINPUTWIDE){
            arrayNewObj[size] = new UIInput();
        } else if (ttype == GUIBUTTON){
            arrayNewObj[size] = new UIButton();
        } else if (ttype == GUILISTBOX){
            arrayNewObj[size] = new UIList();
        } else if (ttype == GUIARTSURFACE){
            arrayNewObj[size] = new UIArt();
        } else if (ttype == GUICHECK){
            arrayNewObj[size] = new UICheck();
        } else if (ttype == GUIPANEL){
            arrayNewObj[size] = new UIPanel();
        } else if (ttype == GUIPANELBORDER){
            arrayNewObj[size] = new UIPanelBorder();
        } else if (ttype == GUIPICTURE){
            arrayNewObj[size] = new UIPicture();
        } else if (ttype == GUIPROGRESSBAR){
            arrayNewObj[size] = new UIProgressBar();
        } else if (ttype == GUIPOPUPMENU){
            arrayNewObj[size] = new UIPopupMenu();
        } else if (ttype == GUICOMBOBOX){
            arrayNewObj[size] = new UIComboBox();
        } else if (ttype == GUILISTGROUPBOX){
            arrayNewObj[size] = new UIListGroup();
        } else if (ttype == GUITEXTELEMENTSAREA){
            arrayNewObj[size] = new UITextElementsArea();
        } else if (ttype == GUISPECTRUM){
            arrayNewObj[size] = new UISpectrum();
        } else if (ttype == GUISLIDER){
            arrayNewObj[size] = new UISlider();
        } else if (ttype == GUITREELISTBOX){
            arrayNewObj[size] = new UITreeListBox();
        } else if (ttype == GUILISTIMG){
            arrayNewObj[size] = new UIImgList();
        } else {
            arrayNewObj[size] = new Object();
        }

        if (arrayNewObj[size] != NULL){
            arrayNewObj[size]->setName(tname);
            arrayNewObj[size]->setX(tx);
            arrayNewObj[size]->setY(ty);
            arrayNewObj[size]->setOrigx(tx);
            arrayNewObj[size]->setOrigy(ty);
            arrayNewObj[size]->setW(tw);
            arrayNewObj[size]->setH(th);
            arrayNewObj[size]->setLabel(tlabel);
            arrayNewObj[size]->setCentered(centered);
        }

        //Fin Nueva clase para almacenar objetos

        //Centrado del objeto si aplica
        if (centered){
            centrarObjeto(arrayNewObj[size]);
        }

        //Se da el foco al objeto si aplica
        if (size == focusPos){
            arrayNewObj[size]->setFocus(true);
        }

        size++;
        return arrayNewObj[size-1];
    }

    return size > 0 ? arrayNewObj[size-1] : NULL;
}

/**
*
*/
void tmenu_gestor_objects::findNextFocus(){
    Traza::print("tmenu_gestor_objects::findNextFocus", W_DEBUG);
    int tipo;
    bool salir = false;

    if (focusPos < size){
        for (int i=0;i < size && !salir; i++){
            focusPos++;
            if (focusPos >= size) focusPos = 0;

            if (arrayNewObj[i] != NULL){
                tipo = arrayNewObj[focusPos]->getObjectType();
                if (tipo != GUIPANELBORDER && tipo != GUILABEL && arrayNewObj[focusPos]->isEnabled() && arrayNewObj[focusPos]->isVisible()){
                    salir = true;
                }
            }
        }
    }
    setFocus(focusPos);
}

/**
*
*/
void tmenu_gestor_objects::setFocus(int pos){

    Traza::print("tmenu_gestor_objects::setFocus", pos, W_PARANOIC);
    //Ponemos a todos los objetos a false
    clearFocus();

    //Ponemos el foco al elemento que hemos indicado
    if (pos >= 0 && pos < MAXOBJECTS && arrayNewObj[pos] != NULL){
        if (arrayNewObj[pos]->isEnabled()) {
            arrayNewObj[pos]->setFocus(true);
            arrayNewObj[pos]->setImgDrawed(false);
            focusPos = pos;
        } else {
            findNextFocus();
        }
    }
}

/**
*
*/
void tmenu_gestor_objects::setFirstFocus(){
    Traza::print("tmenu_gestor_objects::setFirstFocus", W_PARANOIC);
    if (getSize() > 0 && getObjByPos(0)->isEnabled()){
        this->setFocus(0);
    } else {
        this->findNextFocus();
    }
}


/**
*
*/
void tmenu_gestor_objects::setFocus(string name){
    Traza::print("tmenu_gestor_objects::setFocus. " + name, W_PARANOIC);
    for (int i=0;i<MAXOBJECTS;i++){
        if (arrayNewObj[i] != NULL){
            if (arrayNewObj[i]->getName().compare(name) == 0){
                arrayNewObj[i]->setFocus(true);
                focusPos = i;
            } else {
                arrayNewObj[i]->setFocus(false);
            }
        }
    }
}

/**
*
*/
int tmenu_gestor_objects::getFocus(){
    return focusPos;
}

/**
*
*/
void tmenu_gestor_objects::centrarObjeto(Object *object){
    int x_ = 0;
    int y_ = 0;
    int w_ = object->getW();
    int h_ = object->getH();

    if (object->getObjectType() == GUICHECK){
        h_ = object->getH() - FONTSIZE; //Posicion centrada donde se pinta el inputcheck
    } else if (object->getObjectType() == GUICOMBOBOX){
        h_ = Constant::getINPUTH(); //Posicion centrada donde se pinta el inputcheck
    }

    x_ = (this->w - w_)/2; //Posicion centrada donde se pinta el inputtext
    y_ = (this->h - h_)/2; //Posicion centrada donde se pinta el inputtext

    object->setX(object->getOrigx() + x_);
    object->setY(object->getOrigy() + y_);

    if (object->getX() < 0) object->setX(0);
    if (object->getY() < 0) object->setY(0);
}

/**
*
*/
void tmenu_gestor_objects::setAreaObjMenu(int width, int height){
    this->w = width;
    this->h = height;

    for (int i=0;i<MAXOBJECTS;i++){
        if (arrayNewObj[i] != NULL){
            if (arrayNewObj[i]->isCentered()){
                centrarObjeto(arrayNewObj[i]);
            }
        }
    }
}

/**
*
*/
//void tmenu_gestor_objects::findFirstFocus(int menucarga){
//   //Damos el foco al primer elemento que haya en el menu
//    if (arrayNewObj[0] != NULL and arrayNewObj[0]->isEnabled()){
//        setFocus
//    } else {
//        this->ObjectsMenu[menucarga]->findNextFocus();
//    }
//}

