#include "uipicture.h"



UIPicture::UIPicture(){
    setObjectType(GUIPICTURE);
    alpha = -1;
    safeToDraw = false;
}

UIPicture::~UIPicture(){

}

void UIPicture::action(tEvento *evento){
    //cout << "Evento de UIPicture: "  << getObjectType() << endl;
    static unsigned long lastClick = 0;

    if (evento->isKey && evento->key == SDLK_s){
        this->imgGestor->setResize(!this->imgGestor->isResize());
        this->setImgDrawed(false);
    } else if (evento->isKey && evento->key == SDLK_f){
        this->imgGestor->setSmooth(!this->imgGestor->isSmooth());
        this->setImgDrawed(false);
    }  else if ((evento->isKey && evento->key == SDLK_KP_MINUS ) || (evento->isJoy && (evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_VOLDOWN)))
            || (evento->isMouse && evento->mouse == MOUSE_BUTTON_WHEELDOWN && evento->mouse_state == 1) ){
            this->imgGestor->decZoom();
            this->setImgDrawed(false);
    } else if ( (evento->isKey && evento->key == SDLK_KP_PLUS)  || (evento->isJoy && (evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_VOLUP)))
            || (evento->isMouse && evento->mouse == MOUSE_BUTTON_WHEELUP && evento->mouse_state == 1) ){
            this->imgGestor->incZoom();
            this->setImgDrawed(false);
    } else if ( (evento->isKey && evento->key == SDLK_KP6)){
            this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_RIGHT);
            this->setImgDrawed(true);
    } else if ( (evento->isKey && evento->key == SDLK_KP4)){
            this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_LEFT);
            this->setImgDrawed(true);
    } else if ( (evento->isKey && evento->key == SDLK_KP8)){
            this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_UP);
            this->setImgDrawed(true);
    } else if ( (evento->isKey && evento->key == SDLK_KP2)){
            this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_DOWN);
            this->setImgDrawed(true);
    } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
           Traza::print("UIPicture::action: Mouse Pressed: " + this->getName(), W_DEBUG);
           if (SDL_GetTicks() - lastClick < DBLCLICKSPEED){
               lastClick = SDL_GetTicks() - DBLCLICKSPEED;  //reseteo del dobleclick
            } else {
                lastClick = SDL_GetTicks();
            }
           checkPos(*evento);
           //this->setImgDrawed(false);
    } else {
        Object::action(evento);
    }
}

/**
*
*/
bool UIPicture::loadImgFromFile(string ruta){
    imgGestor->setRuta(ruta);
    imgGestor->setResize(true)->setZoom(0)
            ->setTopDif(0)->setLeftDif(0);
    
    bool ret = false;
    
    try{
        Dirutil dir;
        if (dir.existe(ruta)){
            if (dir.isDir(ruta)){
                Traza::print("UIPicture::loadImgFromFile. Es un directorio: " + ruta, W_DEBUG);
                ret = true;
            } else {
                this->clearImg();
                ret = imgGestor->loadFromFile(ruta);
                setImgDrawed(false);
            }
            
        } else {
            Traza::print("UIPicture::loadImgFromFile. No existe la ruta: " + ruta, W_ERROR);
        }
    } catch (Excepcion &e) {
         Traza::print("Excepcion loadImgFromFile: " + string(e.getMessage()), W_ERROR);
    }
    return ret;
}


/**
 * 
 * @param ruta
 * @param password
 * @param offset
 * @param offsetAnt
 */
bool UIPicture::loadImgFromBin(const char* ruta, const char *password, unsigned long long offset, unsigned long long offsetAnt){
    imgGestor->setRuta(ruta);//Especificamos la ruta del fichero a abrir
    imgGestor->calculaPass(password); //Ponemos el password si no lo teniamos
    imgGestor->setBestfit(false)->setResize(true)->setZoom(0)
            ->setTopDif(0)->setLeftDif(0);
    bool ret = false;
    
     try{
        ret = imgGestor->extraerImgBin(offset, offsetAnt);
        setImgDrawed(false);
     } catch (Excepcion &e) {
         Traza::print("Excepcion loadImgFromFile" + string(e.getMessage()), W_ERROR);
     }
    
    return ret;
}

void UIPicture::clearImg(){
    imgGestor->clearImg();
    setImgDrawed(false);
}

/**
*
*/
void UIPicture::checkPos(tEvento evento){
    int tempY = evento.mouse_y;
    int tempX = evento.mouse_x;
    if (tempX >= this->getX() && tempX <= this->getX() + this->getW() &&
        tempY >= this->getY() && tempY <= this->getY() + this->getH()){
        //Se setea el checked cuando se hace doble click
        if (evento.isMousedblClick){
            //this->setChecked(true);
            this->setChecked(true);
        }
    }
}

void UIPicture::setSafeToDraw(bool safeToDraw) {
    this->safeToDraw = safeToDraw;
}

bool UIPicture::isSafeToDraw() const {
    return safeToDraw;
}

