#include "uislider.h"

/**
*
*/
UISlider::UISlider(){
    setObjectType(GUISLIDER);
}

/**
*
*/
UISlider::~UISlider(){

}

/**
*
*/
void UISlider::checkPos(tEvento evento){
    int tempY = evento.mouse_y;

    if (//tempX > this->getX() && tempX < (this->getX() + this->getW()) &&
        tempY > this->getY() && tempY < (this->getY() + this->getH())){
        int dif = tempY - this->getY();

        float percent = this->getH() >= 1 ? dif/(float)this->getH() : 0;
        percent = 1.0 - percent;

        Traza::print("tempY: " + Constant::TipoToStr(tempY)
                     + " ,this->getY(): " + Constant::TipoToStr(this->getY())
                     + " ,dif: " + Constant::TipoToStr(dif)
                     + " ,this->getH(): " + Constant::TipoToStr(this->getH())
                     + " ,percent: " + Constant::TipoToStr(percent), W_DEBUG);

        //Miramos cual es el porcentaje seleccionado dependiendo del tamanyo del progressbar
        //int percent = this->getProgressMax() * (dif / (float)this->getW());
        this->setProgressPos(ceil(this->getProgressMax() * percent));
        this->setChecked(true);
    }
}
