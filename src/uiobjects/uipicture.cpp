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
    static unsigned long lastMoveRepaint = 0;
    static int lastTop = 0;
    static int lastLeft = 0;
    
    if (evento->isMouseMove && evento->isRegionSelected){
        if (SDL_GetTicks() - lastMoveRepaint > 150){
            this->imgGestor->setTopDif(lastTop - evento->region.selH);
            this->imgGestor->setLeftDif(lastLeft - evento->region.selW);
            this->setImgDrawed(false);
            lastMoveRepaint = SDL_GetTicks() - 150;
        } else {
            lastMoveRepaint = SDL_GetTicks();
        }
    } else if (evento->isKey && evento->key == SDLK_s){
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
            //this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_RIGHT);
            if (imgGestor->getMoveSurface() != NULL){
                this->imgGestor->incLeftDif();
                this->setImgDrawed(false);
            }
    } else if ( (evento->isKey && evento->key == SDLK_KP4)){
            this->imgGestor->decLeftDif();
            //this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_LEFT);
            if (imgGestor->getMoveSurface() != NULL){
                this->imgGestor->decLeftDif();
                this->setImgDrawed(false);
            }

    } else if ( (evento->isKey && evento->key == SDLK_KP8)){
//            this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_UP);
            if (imgGestor->getMoveSurface() != NULL){
                this->imgGestor->decTopDif();
                this->setImgDrawed(false);
            }
            this->setImgDrawed(false);
    } else if ( (evento->isKey && evento->key == SDLK_KP2)){
//            this->imgGestor->blitImgMoved(imgGestor->getMoveSurface(), this->surfaceCache, MOVE_DOWN);
            if (imgGestor->getMoveSurface() != NULL){
                this->imgGestor->incTopDif();
                this->setImgDrawed(false);
            }
            this->setImgDrawed(false);
    } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
           Traza::print("UIPicture::action: Mouse Pressed: " + this->getName(), W_DEBUG);
           lastTop = this->imgGestor->getTopDif();
           lastLeft = this->imgGestor->getLeftDif();
           
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
 * @param ruta
 * @return 
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

/**
 * 
 * @return 
 */
bool UIPicture::isSafeToDraw() const {
    return safeToDraw;
}

/**
 * 
 * @param ruta
 * @return 
 */
bool UIPicture::isValidImage(string ruta){
    bool ret = false;
    const int tam = sizeof(VALID_IMGS) / sizeof(VALID_IMGS[0]);
    int i=0;
    std::size_t found;
    
    string lowRuta = ruta;
    Constant::lowerCase(&lowRuta);
    do{
        found = lowRuta.find(VALID_IMGS[i]);
        if (found != std::string::npos){
            ret = true;
        }
        i++;
    } while (!ret && i < tam);
    return ret;
}

