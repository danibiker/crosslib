#include "uitextelementsarea.h"

UITextElementsArea::UITextElementsArea(){
    setObjectType(GUITEXTELEMENTSAREA);
    offsetDesplazamiento = 0;
    intervalDespl = Constant::getMENUSPACE();
    maxOffsetY = 0;
    selectedPos = -1;
}

UITextElementsArea::~UITextElementsArea(){
    clear();
}

void UITextElementsArea::addField(TextElement *textElem){
    textVector.push_back(new TextElement(textElem));
}

void UITextElementsArea::clear(){
    textVector.clear();
    offsetDesplazamiento = 0;
}

void UITextElementsArea::setFieldText(string name, string text){
    for (unsigned int i=0; i < textVector.size(); i++){
        if (textVector.at(i)->getName().compare(name) == 0){
            textVector.at(i)->setText(text);
        }
    }
}
void UITextElementsArea::setPosition(string name, t_posicion pos){
    for (unsigned int i=0; i < textVector.size(); i++){
        if (textVector.at(i)->getName().compare(name) == 0){
            textVector.at(i)->setPos(pos);
        }
    }
}

void UITextElementsArea::action(tEvento *evento){
    //Si hay un popup lanzado por este elemento, debe dejar de procesar los eventos
    if (popup == false){
        if (evento->isKey || evento->isJoy || (evento->isMouse &&
                                               (evento->mouse == MOUSE_BUTTON_WHEELDOWN || evento->mouse == MOUSE_BUTTON_WHEELUP)) ){

            Traza::print("UITextElementsArea::action: " + this->getName(), W_PARANOIC);
            this->setImgDrawed(false);

            if (evento->key == SDLK_UP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_UP) || (evento->mouse == MOUSE_BUTTON_WHEELUP) ){
               if (offsetDesplazamiento < 0){
                    offsetDesplazamiento += intervalDespl;
               }
            } else if (evento->key == SDLK_DOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN) || evento->mouse == MOUSE_BUTTON_WHEELDOWN){
               if (abs(offsetDesplazamiento) < abs(maxOffsetY) || maxOffsetY == 0){
                    offsetDesplazamiento -= intervalDespl;
               }
            }
        } else if ( (evento->isMouse && evento->mouse_state == SDL_PRESSED && evento->mouse == MOUSE_BUTTON_LEFT)
                   || evento->isMouseMove){
            int my = evento->mouse_y;
            int mx = evento->mouse_x;

            my -= offsetDesplazamiento;

            for (unsigned int i=0; i < textVector.size(); i++){
                if (my >= textVector.at(i)->getPos()->y + this->getY() &&
                    my <= textVector.at(i)->getPos()->y + this->getY() + textVector.at(i)->getStyle()->fontSize &&
                    mx >= textVector.at(i)->getPos()->x + this->getX() &&
                    mx <= this->getX() + this->getW()
                ){
                    if (evento->isMouse && evento->mouse_state == SDL_PRESSED && evento->mouse == MOUSE_BUTTON_LEFT){
                        this->setChecked(true);
                    } else {
                        //Si nos movemos por el texto y estamos fuera del enlace, refrescamos la pantalla
                        if ((int)i != this->selectedPos){
                            //Para que se actualice la linea del enlace
                            setImgDrawed(false);
                        }
                    }
                    this->selectedPos = i;
                }
            }

            if (selectedPos >= 0 && cursorHover(evento) && !textVector.at(this->selectedPos)->getUrl().empty()){
                this->setCursor(cursor_hand);
            } else {
                this->setCursor(-1);
            }
            Traza::print("UITextElementsArea::Pos: ", this->selectedPos, W_PARANOIC);
        } else {
            Object::action(evento);
        }
    }
}


