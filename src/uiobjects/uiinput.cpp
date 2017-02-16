#include "uiinput.h"

/**
*
*/
UIInput::UIInput(){
    setObjectType(GUIINPUTWIDE);
    posChar = 0; //Posicion dentro del input
    offset = 0;
    posArrayLetras = 0; //Posicion del array de letras que vamos ciclando
    textAreaText = "";
    resetSelection();

}

/**
*
*/
UIInput::~UIInput(){

}

void UIInput::resetSelection(){
    selectionIni = -1;
    selectionFin = -1;
    Traza::print("Reset Seleccion UIInput", W_PARANOIC);
}

/**
*
*/
void UIInput::action(tEvento *evento){
    unsigned int tamInput = this->getText().length();
    unsigned int posCursor = this->posChar;

    Traza::print("Action del UIInput", W_PARANOIC);

    if ( (evento->isKey || evento->isJoy) && evento->keyjoydown ){
        Traza::print("Evento Teclado o joystick", evento->key, W_PARANOIC);
        //Boton Arriba y abajo
        if (evento->key == SDLK_UP || evento->joy == JoyMapper::getJoyMapper((JOY_BUTTON_UP)) ||
            evento->key == SDLK_DOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN)){
            resetSelection();
            if (posCursor < this->getText().length()){
                std::vector<std::string> strSplitted = Constant::splitPos(this->getText(), posCursor+1);
                string letraActualCursor = strSplitted.at(0).substr(strSplitted.at(0).length()-1,1);
                this->posArrayLetras = buscaPosicion(letraActualCursor.at(0));
                if (evento->key == SDLK_UP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_UP)){
                    if (this->getPosArrayLetras() < strlen(ARRAYLETRAS)-1){
                        this->posArrayLetras++;
                    } else {
                        this->posArrayLetras = 0;
                    }
                } else if (evento->key == SDLK_DOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN)){
                    if (this->posArrayLetras > 0){
                        this->posArrayLetras--;
                    } else {
                        this->posArrayLetras = strlen(ARRAYLETRAS)-1;
                    }
                }
                //limitarAInt();
                //Traza::print("Letra del cursor: " + letraActualCursor, DEBUG );
                this->setText(strSplitted.at(0).substr(0, strSplitted.at(0).length()-1) + ARRAYLETRAS[this->posArrayLetras] + strSplitted.at(1));
                this->posChar = posCursor ;
            } else if (posCursor == this->getText().length() || this->getText().length() == 0) {
                this->setText(this->getText() + " ");
                (this->posChar > 0) ? this->posChar-- : this->posChar = 0;
            }
        } else if (evento->key == SDLK_RIGHT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_RIGHT)){
        //Boton Derecho
            if (evento->keyMod & KMOD_LSHIFT){
                if (this->posChar >= 0 && selectionIni == -1)
                    selectionIni = this->posChar;

                if (this->posChar < tamInput){
                    this->posChar++;

                    if ((int)this->posChar <= selectionFin && selectionFin > -1)
                        selectionIni = this->posChar;

                    if ((int)this->posChar >= selectionFin)
                        selectionFin = this->posChar;
                }
                Traza::print("Sel Right: " + Constant::TipoToStr(selectionIni) + ":" + Constant::TipoToStr(selectionFin), W_PARANOIC);
            } else if (evento->keyMod & KMOD_LALT){
                Traza::print("Buscando siguiente espacio", W_PARANOIC);
                if (this->posChar < tamInput){
                    do{
                        this->posChar++;
                        Traza::print("this->posChar",this->posChar, W_PARANOIC);
                    }while (this->posChar < tamInput && string(" ").compare(this->getText().substr(this->posChar,1)) != 0);
                }
            } else {
                resetSelection();
                if (this->posChar < tamInput){
                    this->posChar++;
                }
                if (this->posChar > tamInput){
                    this->posChar = tamInput-1;
                }
            }
        } else if (evento->key == SDLK_LEFT || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_LEFT)){
        /**Boton Izquierdo*/
            if (evento->keyMod & KMOD_LSHIFT){
                if (selectionFin == -1){
                    selectionFin = this->posChar;
                }
                if (this->posChar > 0){
                    this->posChar--;

                    if (selectionIni > -1 && (int)this->posChar >= selectionIni){
                        selectionFin = this->posChar;
                    } else {
                        selectionIni = this->posChar;
                    }
                }
                Traza::print("Sel Left: " + Constant::TipoToStr(selectionIni) + ":" + Constant::TipoToStr(selectionFin), W_PARANOIC);
            } else if (evento->keyMod & KMOD_LALT){
                Traza::print("Buscando anterior espacio", W_PARANOIC);
                if (this->posChar > 0){
                    do{
                        this->posChar--;
                        Traza::print("this->posChar",this->posChar, W_PARANOIC);
                    }while (this->posChar > 0 && string(" ").compare(this->getText().substr(this->posChar - 1,1)) != 0);
                }

            } else {
                resetSelection();
                if (this->posChar > tamInput) this->posChar = tamInput;
                if (this->posChar > 0){
                    this->posChar--;
                }
            }
        } else if (evento->key == SDLK_a && evento->keyMod & KMOD_LCTRL){
            selectionIni = 0;
            selectionFin = tamInput;
        } else if (evento->key == SDLK_HOME){
            if (evento->keyMod & KMOD_LSHIFT){
                selectionIni = 0;
                selectionFin = this->posChar;
            } else {
                resetSelection();
            }
            this->posChar = 0;
        } else if (evento->key == SDLK_END){
            if (evento->keyMod & KMOD_LSHIFT){
                selectionIni = this->posChar;
                selectionFin = tamInput;
            } else {
                resetSelection();
            }
            this->posChar = tamInput;
        //Intro para seleccionar el elemento
        } else if (evento->key == SDLK_BACKSPACE ||  evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_X)){
            if (selectionIni == -1 && selectionFin == -1){
                resetSelection();
                std::vector<std::string> strSplitted = Constant::splitPos(this->getText(), posCursor);
                if (posCursor > 0 && posCursor <= this->getText().length()){
                    this->setText(strSplitted.at(0).substr(0,posCursor-1) + strSplitted.at(1));
                }
                if (posCursor > 0){
                    this->posChar = posCursor - 1;
                }
                Traza::print("borrando::poschar: " + Constant::TipoToStr<int>(this->posChar) + "; tam:" + Constant::TipoToStr<int>(this->getText().length()), W_PARANOIC );
            } else {
                borrarSeleccion();
            }
        } else if (evento->key == SDLK_DELETE){
            if (selectionIni == -1 && selectionFin == -1){
                resetSelection();
                if (posCursor >= 0 && posCursor < this->getText().length()){
                    std::vector<std::string> strSplitted = Constant::splitPos(this->getText(), posCursor);
                    this->setText(strSplitted.at(0) + strSplitted.at(1).substr(1));
                    this->posChar = posCursor;
                }
                Traza::print("borrando::poschar: " + Constant::TipoToStr<int>(this->posChar) + "; tam:" + Constant::TipoToStr<int>(this->getText().length()), W_PARANOIC );
            } else {
                borrarSeleccion();
            }
        } else {

            if (selectionIni != -1 && selectionFin != -1 &&
                ((evento->keyMod & KMOD_LCTRL && evento->key == SDLK_v) || (evento->unicode >= 32 && evento->unicode <= 126))
            ){
                borrarSeleccion();
                tamInput = this->getText().length();
                posCursor = this->posChar;
            }

            Traza::print("Pulsada tecla: " + evento->unicode, W_PARANOIC);
            //guardamos la posicion actual del cursor porque se pierde al hacer setText
            char teclaPulsada = ' ';
            std::vector<std::string> strSplitted = Constant::splitPos(this->getText(), posCursor);

            if (evento->keyMod & KMOD_LCTRL && evento->key == SDLK_v){
                string clipText = Constant::GetClipboardText();
                Traza::print("intentado pegar el texto: " + clipText, W_PARANOIC);
                this->setText(strSplitted.at(0) + clipText + strSplitted.at(1));
                this->posChar = posCursor + clipText.length();
            } else if (evento->unicode >= 32 && evento->unicode <= 126){
                //Solo deberiamos poder meter texto unicode. para ello se tiene que habilitar con SDL_EnableUNICODE
                teclaPulsada = (char)(evento->unicode);
                this->setText(strSplitted.at(0) + teclaPulsada + strSplitted.at(1));
                this->posChar = posCursor + 1;
            }
            //Borramos el texto
            if ( evento->key != SDLK_LSHIFT && !(evento->keyMod & KMOD_LSHIFT) ){
                resetSelection();
            }
        }
    } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_RIGHT && (evento->mouse_state == SDL_PRESSED) ){
        Object::action(evento);
    }
}

/**
* Si hay una letra seleccionada, recorremos el array para buscar la posicion de esa letra. Asi la siguiente
* letra sera consecutiva
*/
int UIInput::buscaPosicion(char letra){
    int pos=0;
    char str[2] = {letra,'\0'};
    char str2[2] = {' ','\0'};
    bool found = false;

    for (unsigned int i=0;i<strlen(ARRAYLETRAS)-1 && !found;i++){
        str2[0] = ARRAYLETRAS[i];
        if (strcmp(str,str2) == 0){
            pos = i;
//            found = true;
        }
    }
    return pos;
}

/**
*
*/
unsigned int UIInput::getSize(){
    return textAreaText.length();
}

/**
*
*/
void UIInput::borrarSeleccion(){
    unsigned int posCursor = this->posChar;

    if (posCursor <= this->getText().length()){
        this->setText( (selectionIni > 0 ? this->getText().substr(0,selectionIni) : "")
                       + this->getText().substr(selectionFin));
    }

    if (selectionIni >= 0){
        this->posChar = selectionIni;
    }
    Traza::print("borrando::selectionIni: " + Constant::TipoToStr<int>(selectionIni) + "; selectionFin:" + Constant::TipoToStr<int>(selectionFin), W_PARANOIC );
    resetSelection();
}
