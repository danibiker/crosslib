#include "uicheck.h"

UICheck::UICheck(){
    setObjectType(GUICHECK);
}

UICheck::~UICheck(){
}

void UICheck::checkPos(tEvento evento){

//    int tempX = evento.mouse_x;
//    int tempY = evento.mouse_y;
    this->setChecked(!this->isChecked());

}

