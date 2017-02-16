#include "object.h"

/**
*
*/
Object::Object(){
    name = "";
    clear();
}

void Object::clear(){
    classType = COBJECT;
    x=0;
    y=0;
    w=0;
    h=0;
    origx = 0;
    origy = 0;
    icon = -1;
    enabled = true;
    tag = "";
    checked = false;
    label = "";
    focus = false;
    centered = false;
    sizeLabel = 0;
    visible = true;
    anchorBottom = false;
    marginBottom = 0;
    verContenedor = true;
    imgGestor = new ImagenGestor();
    otherCache = new ImagenGestor();
    imgDrawed = false;
    otherDrawed = false;
    popup = false;
    popupName = "";
    shadow = true;
    cursor = -1;
    color = cInputContent;
    textColor = cNegro;
    alpha = -1;

}

/**
*
*/
Object::~Object(){
    delete imgGestor;
    delete otherCache;
}

/**
*
*/
void Object::action(tEvento *evento){
    //Traza::print("Action not implemented for: " + this->getName() + "; " + this->getLabel(), W_DEBUG);
    if (evento != NULL){
       if ((evento->key == SDLK_RETURN && !(evento->keyMod & KMOD_LALT)) ||
                evento->key == SDLK_SPACE ||
                evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_A)){
            checkPos(*evento);
            Traza::print("Object::action: Evento de seleccion", W_PARANOIC);
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && (evento->mouse_state == SDL_RELEASED) && cursorHover(evento) ){
           Traza::print("Object::action: Checkeando: " + this->getName(), W_PARANOIC);
           checkPos(*evento);
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_RIGHT && (evento->mouse_state == SDL_PRESSED) && cursorHover(evento) ){
           Traza::print("Object::action: Mouse Right: " + this->getName(), W_DEBUG);
           if (getPopupName().compare("") != 0){
                setPopup(true);
                Traza::print("Popup true", W_DEBUG);
           }
        }
    }
}

/**
*
*/
void Object::checkPos(tEvento evento){
    Traza::print("Elemento sin evento de seleccion: " + this->getName() + "; " + this->getLabel(), W_PARANOIC);
}

/**
*
*/
void Object::draw(){
    //cout << "drawing object" << endl;
}

bool Object::cursorHover(tEvento *evento){
    int my = evento->mouse_y;
    int mx = evento->mouse_x;
    int posXI = this->getX();
    int posXD = this->getX() + this->getW();
    int posYU = this->getY();
    int posYD = this->getY() + this->getH();

    if (my >= posYU && my <= posYD && mx >= posXI && mx <= posXD){
        return true;
    } else {
        return false;
    }
}
