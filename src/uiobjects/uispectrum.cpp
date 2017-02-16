#include "uispectrum.h"

UISpectrum::UISpectrum(){
    setObjectType(GUISPECTRUM);
    buf = NULL;
    zoom = 1;
    maxZoom = 1;
    arrFreqVis = NULL;
    spectrumVisualizer = 0;
}

UISpectrum::~UISpectrum(){
}

void UISpectrum::calcZoom(int width){
    width -= 2*INPUTBORDER;
    int lenX = width*2 > BUFFERSPECTRUMVIS ? BUFFERSPECTRUMVIS : width*2;
    zoom = floor(BUFFERSPECTRUMVIS / (float)lenX);
    if (zoom < 1) zoom = 1;
    maxZoom = zoom;

    if (zoom > 1 && zoom  % 2 == 0){
        zoom = zoom / 2;
    }
}

void UISpectrum::action(tEvento *evento){
    static unsigned long lastClick = 0;
    //Si hay un popup lanzado por este elemento, debe dejar de procesar los eventos
    if (popup == false){
        if ((evento->isMouse && evento->mouse_state == SDL_PRESSED &&
               (evento->mouse == MOUSE_BUTTON_WHEELDOWN || evento->mouse == MOUSE_BUTTON_WHEELUP)) ){

            Traza::print("UISpectrum::action: " + this->getName(), W_PARANOIC);
            this->setImgDrawed(false);

            if (evento->key == SDLK_UP || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_UP) || (evento->mouse == MOUSE_BUTTON_WHEELUP) ){
               decZoom();
            } else if (evento->key == SDLK_DOWN || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_DOWN) || evento->mouse == MOUSE_BUTTON_WHEELDOWN){
                incZoom();
            }
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
           Traza::print("UISpectrum::action: Mouse Pressed: " + this->getName(), W_PARANOIC);
           if (SDL_GetTicks() - lastClick < DBLCLICKSPEED){
               spectrumVisualizer = (spectrumVisualizer + 1) % MAXSPECTRUMVIS;
               lastClick = SDL_GetTicks() - DBLCLICKSPEED;  //reseteo del dobleclick
            } else {
                lastClick = SDL_GetTicks();
            }

           this->setImgDrawed(false);
        } else {
            Object::action(evento);
        }
    }
}

