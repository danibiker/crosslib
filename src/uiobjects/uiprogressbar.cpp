#include "uiprogressbar.h"

UIProgressBar::UIProgressBar(){
    setObjectType(GUIPROGRESSBAR);
    min = 0;
    max = 0;
    pos = 0;
    lastTimeTick = 0;
    posXNow = 0;
    typeHint = HINT_PERCENT;
    mouseOverBar = false;
    showHint = true;
}

UIProgressBar::~UIProgressBar(){

}

void UIProgressBar::checkPos(tEvento evento){
    int tempX = evento.mouse_x;

    if (tempX > this->getX() && tempX < (this->getX() + this->getW()) ){
        int dif = tempX - this->getX();
        float percent = this->getW() >= 1 ? dif/(float)this->getW() : 0;

        Traza::print("tempX: " + Constant::TipoToStr(tempX)
                     + " ,this->getX(): " + Constant::TipoToStr(this->getX())
                     + " ,dif: " + Constant::TipoToStr(dif)
                     + " ,this->getW(): " + Constant::TipoToStr(this->getW())
                     + " ,percent: " + Constant::TipoToStr(percent), W_DEBUG);

        //Miramos cual es el porcentaje seleccionado dependiendo del tamanyo del progressbar
        if (tempX == this->getX() + this->getW() - INPUTBORDER){
            this->setProgressPos(this->getProgressMax());
        } else if(tempX == this->getX() + INPUTBORDER){
            this->setProgressPos(this->getProgressMin());
        } else {
            this->setProgressPos(ceil(this->getProgressMax() * percent));
        }
        this->setChecked(true);
    }
}

/**
*
*/
void UIProgressBar::action(tEvento *evento){
    if (evento->isMouseMove && evento->mouse == MOUSE_BUTTON_LEFT){
       checkPos(*evento);
       Traza::print("UISlider::action: Mouse Drag: " + this->getName(), W_DEBUG);
       this->setImgDrawed(false);
    } else {
        Object::action(evento);
    }
}
