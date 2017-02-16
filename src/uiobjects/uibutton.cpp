#include "uibutton.h"

UIButton::UIButton(){
    setObjectType(GUIBUTTON);
}

UIButton::~UIButton(){

}

void UIButton::checkPos(tEvento evento){
    this->setChecked(true);
    Traza::print("checkPos UIButton", W_DEBUG);
}

/**
*
*/
void UIButton::draw(){
    //cout << "drawing UIButton" << endl;
}


/**
*
*/
void UIButton::action(tEvento *evento){
    //Si hay un popup lanzado por este elemento, debe dejar de procesar los eventos
    if (popup == false){
        if (evento->isMouseMove){
            if (cursorHover(evento)){
                this->setCursor(cursor_hand);
            } else {
                this->setCursor(-1);
            }
        } else {
            //Traza::print("UIListGroup::action: Pasando al padre: " + this->getName(), W_DEBUG);
            Object::action(evento);
        }
    }
}



