#include "uicombobox.h"

UIComboBox::UIComboBox(){
    inicializarObjeto(GUICOMBOBOX);
    showScrollbar = false;
    valueChanged = false;
}

UIComboBox::~UIComboBox(){

}

void UIComboBox::action(tEvento *evento){
    UIList::action(evento);
    if ((evento->key == SDLK_RETURN && !(evento->keyMod & KMOD_LALT)) || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_A)){
        checkPos(*evento);
    }
}

/**
*
*/
void UIComboBox::checkPos(tEvento evento){

    int tempY = evento.mouse_y;
    int tempX = evento.mouse_x;
    int anaydido_Y_PorListaCombo = 0;
    valueChanged = false;

    bool objetoClickado = false;

    if (evento.isKey && evento.key == SDLK_RETURN && isFocus()){
        objetoClickado = true;
    } else if (isChecked()){
        //Si el combo esta desplegado, la y es desde todo el combo hasta el tamanyo de la lista
        if (tempY > getY() && tempY < getY() + getH() + Constant::getCOMBOLISTHEIGHT()
            && tempX > getX() && tempX < getX() + getW()) {
            objetoClickado = true;
        }
    } else {
        //Si no esta desplegado, la altura va solo hasta la altura del input del combo
        if (tempY > getY() && tempY < getY() + getH()
            && tempX > getX() && tempX < getX() + getW()) {
            objetoClickado = true;
        }
    }


    if (objetoClickado){
        //Si estamos mostrando la lista "isChecked()", la posicion de "y" queda desplazada tanto como la altura
        //del input. Comprobamos que realmente ha pulsado en la lista y no en el input del combo "tempY > getY() + getH()"
        if (isChecked() && tempY > getY() + Constant::getINPUTH()){
            anaydido_Y_PorListaCombo = Constant::getINPUTH();
            valueChanged = true;
            //Calculamos la posicion del click
            int posClick = (tempY - anaydido_Y_PorListaCombo - this->getY() - INPUTCONTENT) / Constant::getMENUSPACE();
            //Especificamos la posicion de la lista
            if (this->getPosIniLista() + posClick < this->getSize()){
                this->setPosActualLista(this->getPosIniLista() + posClick);
            }
        }

        //Se setea el checked cuando se hace click
        if (evento.isMouse || (evento.isKey && evento.key == SDLK_RETURN && isFocus())){
            this->setChecked(!this->isChecked());
        }
    }

    //Si se pulsa el raton, cuando tenemos la lista desplegada y ha pulsado fuera del objeto,
    //ocultamos la lista
    if (evento.isMouse && isChecked() && !objetoClickado){
        this->setChecked(!this->isChecked());
    }
}

string UIComboBox::getSelectedValue(){
    if (this->getPosActualLista() >= 0 && this->getPosActualLista() < getSize())
        return this->getListValues()->get(this->getPosActualLista());
    else
        return "";
}


