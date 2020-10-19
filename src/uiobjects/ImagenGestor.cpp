#include "ImagenGestor.h"


/**
*
*/
ImagenGestor::ImagenGestor(){
    offsets = NULL;
    zoomValue = 0;
    resize = true;
    bestfit = false;
    centrado = false;
    centradoX = false;
    centradoY = false;
    smooth = true;
    top = 0;
    left = 0;
    bordeTop = 0;
    bordeBottom = 0;
    bordeLeft = 0;
    bordeRight = 0;
    surface = NULL;
    imgOrigWidth = 0;
    imgOrigHeight = 0;
    moveSurface = NULL;
    enabledMoveImg = true;
    colorBackground = cBlanco;
    fillBackgroundColour = true;
    enabledZoom = false;
}

/**
*
*/
ImagenGestor::~ImagenGestor(){
    if (offsets != NULL){
        delete offsets;
    }

    if (this->surface != NULL){
        Traza::print("Eliminando Imagen", W_PARANOIC);
        SDL_FreeSurface(this->surface);
    }
}

/**
* catDiractualRuta
*/
void ImagenGestor::catDiractualRuta(string str){
    Dirutil *fio = new Dirutil();
    char rutaTemp[PATH_MAX];
    strcpy(rutaTemp, fio->getDirActual());
    fio->dircat(rutaTemp,str.c_str());
    delete fio;
    setRuta(rutaTemp);
}

/**
* setRuta
*/
void ImagenGestor::setRuta(string str){
    rutaImg = str;
}

/**
* cargarOffsets
*/
void ImagenGestor::cargarOffsets(){
    const int posExt = rutaImg.find(".bin");
    const int posIni = rutaImg.find_last_of(FILE_SEPARATOR);

    if (posExt > -1)
    {
        //Generamos el nombre del fichero .inf y lo cargamos en una lista
        string dir = rutaImg.substr (0,posIni+1); //Se incluye tambien la ultima barra del directorio
        string name = rutaImg.substr (posIni+1,posExt-posIni-1);
        rutaInf = dir + name + ".inf";
        if (offsets != NULL){
            delete offsets;
        }
        unsigned int lineas = contarLineas(rutaInf.c_str());
        offsets = new listaSimple<unsigned long long>(lineas+1); //+1 porque se incluye tambien el nombre del .bin abierto ademas de los offsets
        try{
            offsets->loadTipoFromFile(rutaInf);
        } catch (Excepcion &e){
            Traza::print("El fichero .inf no existe" + rutaInf, W_DEBUG);
        }

    } else {
        //throw(Excepcion(EFIO));
        Traza::print("El fichero .bin no existe" + rutaInf, W_DEBUG);
    }
}

/**
* extraerImg
*/
bool ImagenGestor::extraerImg(unsigned int pos){
    bool ret = false;
    
    if (offsets != NULL){
        unsigned long long inicio=0;
        unsigned long long tam=0;

        if (pos == 0)
        {
            tam = offsets->get(pos);
            inicio = 0;
        }
        else
        {
            tam = offsets->get(pos) - offsets->get(pos-1);
            inicio = offsets->get(pos) - tam +1;
        }

//        Traza::print("extraerImg.inicio:", inicio, W_DEBUG);
//        Traza::print("extraerImg.tam:", tam, W_DEBUG);
//        Traza::print("extraerImg.rutaImg:", rutaImg.c_str(), W_DEBUG);

        ret = this->loadFromFile(rutaImg.c_str(), inicio, tam);
        cifrarXOR(this->getFile(),tam);
        zoomValue = 0;

    } else {
        throw(Excepcion(ENULL));
    }
    
    return ret;
}

/**
* extraerImgBin
*/
bool ImagenGestor::extraerImgBin(unsigned long long  offset, unsigned long long  offsetAnt){
        unsigned long long inicio=0;
        unsigned long long tam=0;
        bool ret = false;

        if (offsetAnt == 0){
            tam = offset; //Tamanyo
            inicio = 0;  //Offset
        } else {
            tam = offset - offsetAnt; //Tamanyo
            inicio =  offsetAnt - 1; //Offset
        }

        if (tam > 0 && inicio >= 0){
            ret = this->loadFromFile(rutaImg.c_str(), inicio, tam);
            cifrarXOR(this->getFile(),tam);
            zoomValue = 0;
        } else {
             Traza::print("extraerImgBin: Error al calcular el tamanyo de la imagen en base a los offsets", W_ERROR);
        }
        
        return ret;
}

/**
* Carga el .bin y el .inf
*/
unsigned long long ImagenGestor::abrirImgBin(string password){
    Traza::print("abrirImgBin.inicio:", W_DEBUG);
    unsigned long long sumaOffset = 0;
    //Se cargan los campos del .inf en la lista de offsets
    cargarOffsets();
    //calcula el password para el cifrado
    calculaPass(password);
    //Si existia un .ini, devolvemos el tamanyo del ultimo elemento
    if (offsets != NULL && offsets->getSize() > 0){
        //Borramos el ultimo elemento porque siempre esta vacio
        offsets->del(offsets->getSize()-1);
        //Devolvemos el valor del ultimo elemento
        sumaOffset = offsets->get(offsets->getSize()-1) - 1;
    }
    return sumaOffset;
}
/**
* Antes de llamar a este metodo se debe haber llamado a setRuta, abrirImgBin y extraerImgBin
*/
unsigned long long ImagenGestor::guardarImgBin(string password, string rutaTemp, unsigned long long tempOffset){
    Traza::print("guardarImgBin.inicio:" + rutaTemp, W_DEBUG);
    unsigned long long tamImg = 0;
    //Se carga en memoria la imagen seleccionada
    this->loadFromFile(rutaTemp.c_str());
    //Se cifran los datos de la imagen anterior
    cifrarXOR(this->getFile(),this->getFileSize());
    //Se guarda la imagen en el .bin especificado
    this->writeToFile(this->rutaImg.c_str(), this->getFile(), this->getFileSize(), true);
    //Se Obtenemos el tamanyo de la imagen que hemos guardado
    tamImg = this->getFileSize();
    offsets->add(tempOffset + tamImg + 1);
    return tamImg;
}

/**
 * 
 * @param rutaTemp
 * @param regionPantalla
 * @param format
 * @return 
 */
unsigned long long ImagenGestor::writeImg(string rutaTemp, t_region regionPantalla, SDL_PixelFormat *format){
    Traza::print("writeImg.inicio:" + rutaTemp, W_DEBUG);
    UIImageEncoder imgEncoder;
    SDL_Surface *tmpSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, regionPantalla.selW, regionPantalla.selH, format->BitsPerPixel, rmask,
                gmask,
                bmask,
                amask);
    
    int quality = 85;
    setBordeBottom(0);
    setBordeLeft(0);
    setBordeRight(0);
    setBordeTop(0);
    drawImgMem(-1, regionPantalla.selW, regionPantalla.selH, regionPantalla, tmpSurface);
    imgEncoder.IMG_SaveJPG(rutaTemp.c_str(), tmpSurface, quality);
    SDL_FreeSurface(tmpSurface);
    return 0;
}

/**
 * 
 * @param rutaTemp
 * @param regionPantalla
 * @param format
 * @return 
 */
unsigned long long ImagenGestor::writeImg(string rutaTemp, SDL_Surface *surface){
    Traza::print("writeImg.inicio:" + rutaTemp, W_DEBUG);
    
    if (surface == NULL)
        return 1;
    
    if (surface->pixels == NULL || surface->w == 0 || surface->h == 0)
        return 1;
    
    UIImageEncoder imgEncoder;
    SDL_Surface *tmpSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, rmask,
                gmask,
                bmask,
                amask);
    
    int quality = 85;
    setBordeBottom(0);
    setBordeLeft(0);
    setBordeRight(0);
    setBordeTop(0);
    
    SDL_BlitSurface(surface, NULL, tmpSurface, NULL);
    imgEncoder.IMG_SaveJPG(rutaTemp.c_str(), tmpSurface, quality);
    SDL_FreeSurface(tmpSurface);
    return 0;
}



/**
* guardarImgIni
*/
void ImagenGestor::guardarImgIni(){
    offsets->writeToFile(rutaInf.c_str());
}

/**
* calculaPass
*/
char * ImagenGestor::calculaPass(string password){
    try{
        CSHA1 *sha1 = new CSHA1();
        char szReport[59]; //El tamanyo del valor hash del algoritmo SHA1 es de 160 bits. El resultado devuelto es en hexadecimal pero separado por espacios
        szReport[0] = 0;
        sha1->Reset();
        sha1->Update((unsigned char *)password.c_str(),password.length());
        sha1->Final();
        sha1->ReportHash(szReport, CSHA1::REPORT_HEX);

        hash[0] = szReport[0];
        hash[1] = szReport[1];
        hash[2] = szReport[3];
        hash[3] = szReport[4];

        return(hash);
    }
    catch(...){
        Traza::print("calculaPass: Error en el calculo del password", W_DEBUG);
        return NULL;
    }
}



/**
* cifrarXOR
*/
void ImagenGestor::cifrarXOR(char *finCifr, unsigned int tam){
        const unsigned int temp = tam / 2;
        const char hashtemp = (hash[0]*16 + hash[1])^(hash[2]*16 + hash[3]);

        for (unsigned int i=1;i<temp;i++)
            finCifr[i] ^= hashtemp;

        finCifr[0] ^= hash[0]*16 + hash[1];
        finCifr[temp] ^= hash[2]*16 + hash[3];
}

/**
* decZoom
*/
double ImagenGestor::decZoom(){
    zoomValue += ZOOMINCREMENT;
    return zoomValue;
}

/**
* incZoom
*/
double ImagenGestor::incZoom(){
    zoomValue -= ZOOMINCREMENT;
    return zoomValue;
}

SDL_Surface * ImagenGestor::getSurface(){
    return this->surface;
}

void ImagenGestor::setSurface(SDL_Surface *srf){
    if (this->surface != NULL){
        SDL_FreeSurface(this->surface);
    }
    this->surface = srf;
}


/**
* throws Excepcion
*/
bool ImagenGestor::loadImgFromFile(const char *uri, SDL_Surface **destino){
    try{
        bool cargado = this->loadFromFile(uri); //Almacena el fichero en la memoria para cargarlo despues
        if (this->getFileSize() > 0 && cargado){
            loadImgFromMem(this->getFile(), this->getFileSize(), destino);
            return true;
        } else {
            return false;
        }

    } catch (Excepcion &e){
        return false;
    }
}

/**
* throws Excepcion
*/
bool ImagenGestor::loadImgDisplay(const char *uri, SDL_Surface **destino){
    try{
        bool cargado = this->loadFromFile(uri); //Almacena el fichero en la memoria para cargarlo despues
        if (this->getFileSize() > 0 && cargado){
            SDL_Surface *tmpImage;
            loadImgFromMem(this->getFile(), this->getFileSize(), &tmpImage);
            if (tmpImage != NULL){
                *destino = SDL_DisplayFormat(tmpImage);
                SDL_FreeSurface(tmpImage);
                return true;
            } else {
                return false;
            }
            
        } else {
            return false;
        }

    } catch (Excepcion &e){
        return false;
    }
}

/**
* throws Excepcion
*/
bool ImagenGestor::loadImgFromFileZoom(const char *uri, SDL_Surface **destino, SDL_Rect *dstrect){
    try{
        bool cargado = this->loadFromFile(uri); //Almacena el fichero en la memoria para cargarlo despues
        if (this->getFileSize() > 0 && cargado){
            loadImgFromMem(this->getFile(), this->getFileSize(), destino);
            float i_relacion = relacion(*destino,dstrect->h,dstrect->w); //Calcula cuanto hay que reducir la imagen para que quepa por pantalla
            SDL_Surface *surfRes = rotozoomSurface(*destino, 0, 1/i_relacion, 1);
            SDL_FreeSurface(*destino);
            *destino = surfRes;
            return true;
        } else {
            Traza::print("Error loadImgFromFileZoom. cargado: " + string(cargado ? "S":"N")
                         + "; Size: " + Constant::TipoToStr(this->getFileSize()),W_ERROR);
            return false;
        }
    } catch (Excepcion &e){
        Traza::print("Error loadImgFromFileZoom: " + string(e.getMessage()),W_ERROR);
        return false;
    }
}



/**
* Carga la imagen desde el array binario creado en memoria y establece el tamanyo
* original de la imagen para poder utilizarlo posteriormente
*/
bool ImagenGestor::loadImgFromMem(SDL_Surface **destino){
    bool ret = loadImgFromMem(this->getFile(), this->getFileSize(), destino);
    if (*destino != NULL){
        //Guardamos la posicion de la imagen relativa a la superficie de destino,
        //que suele ser la propia pantalla. Es util para dibujar sobre una imagen
        //en unas posiciones determinadas por su tamanyo
        this->setImgOrigWidth(((SDL_Surface *)*destino)->w);
        this->setImgOrigHeight(((SDL_Surface *)*destino)->h);
    }
    return ret;
}

/**
* Carga la imagen desde el array binario creado en memoria
*/
bool ImagenGestor::loadImgFromMem(char *fileArray, int size, SDL_Surface **destino){
    /*
    IMG_Load_RW(SDL_RWops *src, int freesrc);
    src - The source SDL_RWops as a pointer. The image is loaded from this.
    freesrc - A non-zero value mean is will automatically close/free the src for you.
    */
    bool ret = false;

    if (moveSurface != NULL){
        SDL_FreeSurface(moveSurface);
        moveSurface = NULL;
    }
    if (size > 0 && fileArray != NULL){
        SDL_RWops *tempSurfWop = SDL_RWFromMem(fileArray,size);
        if (tempSurfWop != NULL){
            if (VIEWALPHA){
                SDL_Surface *memImg = IMG_Load_RW(tempSurfWop,0);
                SDL_FreeRW(tempSurfWop);
                if (memImg == NULL) 
                    throw Excepcion(EFIO);

                *destino = SDL_DisplayFormatAlpha(memImg);
                SDL_FreeSurface(memImg);
                ret = true;
            } else {
                *destino = IMG_Load_RW(tempSurfWop,0);
                if (destino == NULL) 
                    throw Excepcion(EFIO);
                ret = true;
            }
        }
    }
    return ret;
}

/**
 * 
 * @param mySurface
 * @param w
 * @param h
 */
void ImagenGestor::makeMoveSurface(SDL_Surface *mySurface, int w, int h){
    if (mySurface != NULL) {
        if (mySurface->w > w || mySurface->h > h){
            if (moveSurface != NULL){
                SDL_FreeSurface(moveSurface);
                moveSurface = NULL;
            }
            moveSurface = SDL_ConvertSurface(mySurface, mySurface->format, mySurface->flags);
        }
    }
}

/**
 * 
 * @param src
 * @param dst
 * @param dirMove
 */
void ImagenGestor::blitImgMoved(SDL_Surface *src, SDL_Surface *dst, int dirMove){
    
    if (src == NULL || dst == NULL)
        return;

    if (src->h > dst->h){
        
        if (dirMove == MOVE_UP){
            decTopDif();
        } else if (dirMove == MOVE_DOWN){
            incTopDif();
        }
        
        if (top > src->h - dst->h){
            top = src->h - dst->h;
        } else if (top < 0){
            top = 0;
        }
    }
    
    if (src->w > dst->w){
        
        if (dirMove == MOVE_LEFT){
            decLeftDif();
        } else if (dirMove == MOVE_RIGHT){
            incLeftDif();
        }
        
        if (left - imgLocationRelScreen.x < 0){
            left = imgLocationRelScreen.x;
        }

        if (left - imgLocationRelScreen.x > src->w - dst->w){
            left = src->w - dst->w + imgLocationRelScreen.x;
        }
    }
    
    
    SDL_Rect srcRect = { left - imgLocationRelScreen.x, top, dst->w, dst->h};
    
    string log = Constant::TipoToStr(left - imgLocationRelScreen.x) + ", " + 
            Constant::TipoToStr(top) + ", " + 
            Constant::TipoToStr(dst->w) + ", " + 
            Constant::TipoToStr(dst->h);
    Traza::print(log, W_DEBUG);
    
    if (isFillBackgroundColour()){
        SDL_FillRect( dst, NULL, SDL_MapRGB(dst->format, colorBackground.r, colorBackground.g, colorBackground.b) );
    }
    SDL_BlitSurface(src, &srcRect, dst, NULL);
}

/**
 * 
 * @param srcSurface
 * @param dstSurface
 * @return 
 */
bool ImagenGestor::updateImgScr(SDL_Surface * srcSurface, SDL_Surface *dstSurface){
    SDL_Rect dstRect = { 0,0,0,0 };
    SDL_Surface *bitmap = NULL;
    bool salida = false;

    if (srcSurface == NULL || dstSurface == NULL)
        return false;
    if (srcSurface->w <= 0 || srcSurface->h <= 0 || dstSurface->w <= 0 || dstSurface->h <= 0 ) 
        return false;
    
    if (this->isResize()){
        if (redimension(srcSurface, dstSurface->w, dstSurface->h, &bitmap)){
            if (isEnabledMoveImg() && zoomValue < 0.0){
                makeMoveSurface(bitmap, srcSurface->w, srcSurface->h);    //Para que podamos movernos por la imagen si la estamos viendo a pantalla completa
            }
            salida = centerAndBlit(bitmap, dstSurface, &dstRect);
        } else {
            salida = centerAndBlit(srcSurface, dstSurface, &dstRect);
        }
    } else {
        salida = centerAndBlit(srcSurface, dstSurface, &dstRect);
    }
    
    return salida;
}

/**
 * 
 * @param srcSurface
 * @param dstW
 * @param dstH
 * @param dstSurface
 * @return 
 */
bool ImagenGestor::updateImgScr(SDL_Surface * srcSurface, int dstW, int dstH, SDL_Surface **dstSurface, SDL_PixelFormat *format){
    SDL_Rect dstRect = { 0,0,0,0 };
    SDL_Surface *bitmap = NULL;
    bool salida = false;

    if (srcSurface == NULL)
        return false;
    if (srcSurface->w <= 0 || srcSurface->h <= 0) 
        return false;
    
    if (this->isResize()){
        if (redimension(srcSurface, dstW, dstH, &bitmap)){
            if (isEnabledMoveImg() && zoomValue < 0.0){
                makeMoveSurface(bitmap, srcSurface->w, srcSurface->h);    //Para que podamos movernos por la imagen si la estamos viendo a pantalla completa
            }
        }
    } 
        
    *dstSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap != NULL ? bitmap->w : srcSurface->w, 
                                                     bitmap != NULL ? bitmap->h : srcSurface->h, 
                                                     format->BitsPerPixel,0,0,0,0);
    
    if (isFillBackgroundColour()){
        //Si se pinta la imagen en el recuadro que haya definido para ella, se pinta el fondo total
        //del recuadro del color del fondo de la imagen
        SDL_FillRect(*dstSurface, NULL, SDL_MapRGB(format, colorBackground.r, colorBackground.g, colorBackground.b) );
    }
    salida = centerAndBlit(bitmap != NULL ? bitmap : srcSurface, *dstSurface, &dstRect);
    
    return salida;
}

/**
 * 
 * @param srcSurface
 * @param dstSurface
 * @param dstRect
 * @return 
 */
bool ImagenGestor::centerAndBlit(SDL_Surface *srcSurface, SDL_Surface *dstSurface, SDL_Rect *dstRect){
    if (isEnabledZoom()){
        calcRectCent(dstRect, srcSurface->w, srcSurface->h, dstSurface->w, dstSurface->h);
    }
    return blitImage(srcSurface, dstSurface,dstRect, true);
}

/**
* calculamos el rectangulo para que quede centrada la imagen
*/
void ImagenGestor::calcRectCent( SDL_Rect *rectCentrado, int srcW, int srcH, int dstW, int dstH){

//    int bordesX = this->getBordeLeft() + this->getBordeRight();
//    int bordesY = this->getBordeTop() + this->getBordeBottom();

    int centrox = (dstW - (int)srcW + this->getBordeLeft())/2;
    int centroy = (dstH - (int)srcH + this->getBordeTop())/2;
    
    if (!centrado){
        if (!centradoX) centrox = this->getBordeLeft();
        if (!centradoY) centroy = this->getBordeTop();
    }

    rectCentrado->x=centrox;
    rectCentrado->y=centroy;
    rectCentrado->w=dstW - this->getBordeRight();
    rectCentrado->h=dstH - this->getBordeBottom();
    rectCentrado->x -= this->getLeftDif();
    rectCentrado->y -= this->getTopDif();

    if (srcW < dstW){
        //Si el ancho de la imagen cabe en pantalla no necesitamos moverla.
        //La devolvemos al centro de la pantalla
        if (rectCentrado->x > 0)
            rectCentrado->x = centrox;
    } else {
        //Si la imagen no cabe, intentamos que no sobrepase los limites de la pantalla
        if (rectCentrado->x >= 0){
            rectCentrado->x = 0;
            this->setLeftDif(centrox);
        } else if (rectCentrado->x <= dstW - srcW){
            rectCentrado->x = dstW - srcW;
            this->setLeftDif(centrox + (srcW - dstW) );
        }
    }

    if (srcH < dstH){
        //Si el alto de la imagen cabe en pantalla no necesitamos moverla.
        //La devolvemos al centro de la pantalla
        if (rectCentrado->y > 0)
            rectCentrado->y = centroy;
    } else {
        //Si la imagen no cabe, intentamos que no sobrepase los limites de la pantalla
        if (rectCentrado->y >= 0){
            rectCentrado->y = 0;
            this->setTopDif(centroy);
        } else if (rectCentrado->y <= dstH - srcH){
            rectCentrado->y = dstH - srcH;
            this->setTopDif(centroy + (srcH - dstH) );
        }
    }
    
    
}

/**
* Devuelve un nuevo surface en base a un contenedor
* *** LIBERA LA MEMORIA DEL SURFACE PASADO POR PARAMETRO
*
* Si redimensiona la imagen, la deja en el puntero destino y devuelve true.
* Si no la redimensiona, solo devuelve false y deja tal cual la variable srcSurface
*/
bool ImagenGestor::redimension(SDL_Surface *srcSurface, int dstW, int dstH, SDL_Surface **destino){
    float i_relacion = 1;
    int h_destino = dstH;
    int w_destino = dstW;

    i_relacion = relacion(srcSurface,h_destino,w_destino);//Calcula cuanto hay que reducir la imagen para que quepa por pantalla

    if (i_relacion != 1. && i_relacion != 0.){
        if (this->isBestfit()){
            *destino = rotozoomSurfaceXY(srcSurface, 0., 1/this->getZoomX(), 1/this->getZoomY(), this->isSmooth());
        } else {
            *destino = rotozoomSurface(srcSurface, 0., 1/i_relacion, this->isSmooth());
        }
        SDL_FreeSurface(srcSurface);

        if (destino == NULL){
            this->setZoom(0);
        }
        return true;
    } else {
        return false;
    }
}

/**
* Devuelve la relacion de una imagen para hacerla mas pequenya o mas grange
* Si isBestfit=true intenta aprovechar toda la pantalla disponible aunque la imagen sea
* mas pequenya
*/
float ImagenGestor::relacion(SDL_Surface *source, int alto, int ancho)
{
   int destWidth = ancho;
   int destHeight = alto;
   float nPercent = 0;

   Traza::print("source->w: " + Constant::TipoToStr(source->w) + " source->h: " + Constant::TipoToStr(source->h) ,W_PARANOIC);
   Traza::print("destWidth: " + Constant::TipoToStr(destWidth) + " destHeight: " + Constant::TipoToStr(destHeight) ,W_PARANOIC);

   this->setZoomX(((float)source->w / (float)destWidth));
   this->setZoomY(((float)source->h / (float)destHeight));

   if (this->getZoomY() > this->getZoomX())
      nPercent = this->getZoomY();
   else
      nPercent = this->getZoomX();

   Traza::print("nPercent",(int)nPercent, W_PARANOIC);
   Traza::print("zoom",(int)this->getZoom(), W_PARANOIC);

   if (nPercent + this->getZoom() < 0.5){
        this->setZoom(0.5 - nPercent);
        nPercent = 0.5; //No podemos pasar del doble del tamanyo de la imagen
   } else {
        nPercent += this->getZoom();
   }

   if (!this->isBestfit() && nPercent < 1. && this->getZoom() == 0) nPercent = 1.;

   return nPercent;
}

/**
* *** ATENCION: LIBERA LA MEMORIA DEL SURFACE PASADO POR PARAMETRO
*/
bool ImagenGestor::blitImage(SDL_Surface *src, SDL_Surface *dst, SDL_Rect *dstRect, bool freesrc){
    if (src != NULL){
       SDL_BlitSurface(src, NULL, dst, NULL);
       if (freesrc){
            SDL_FreeSurface(src);
            src = NULL;
       }
       return true;
    } else {
        //drawTextCent("Se ha producido algun error al cargar la imagen",0,0,true, true,0,0,0);
        Traza::print("Error: blitImage:: Se ha producido algun error al cargar la imagen:" + string(IMG_GetError()), W_ERROR);
        return false;
    }
}

/**
 * 
 * @param indice
 * @param destw
 * @param desth
 * @param regionPantalla
 * @param imgLocation
 * @return 
 */
bool ImagenGestor::calcImgLocationFromIndex(int indice, int destw, int desth, t_region regionPantalla, SDL_Rect *imgLocation){
    boolean ret = false;
    
    if (indice < 0){
        imgLocation->x = regionPantalla.selX;
        imgLocation->y = regionPantalla.selY;
        imgLocation->w = (short unsigned int)destw;
        imgLocation->h = (short unsigned int)desth;
        return true;
    }
    
    int maxX = calcMaxX(destw, regionPantalla.selW);
    int maxY = calcMaxY(desth, regionPantalla.selH);
    Traza::print("maxX: " + Constant::TipoToStr(maxX) + " maxY: " + Constant::TipoToStr(maxY), W_PARANOIC);

    
    if ((destw <= 0 || desth <= 0) && maxX > 0 && maxY > 0){
        destw  = (regionPantalla.selW - SEPTHUMB - SEPTHUMB * maxX) / maxX;
        desth = (regionPantalla.selH - SEPTHUMB - SEPTHUMB * maxY) / maxY;
    }
    const int maxImg = maxX * maxY;

    if (indice >= maxImg) 
        return ret;
    
    int fillEmptySpaceX = 0;
    int fillEmptySpaceY = 0;
    
    fillEmptySpaceX = (regionPantalla.selW - maxX * (destw + SEPTHUMB)) / (maxX + 1);
    fillEmptySpaceY = (regionPantalla.selH - maxY * (desth + SEPTHUMB)) / (maxY + 1);
    
    Traza::print("Calculando posiciones", W_PARANOIC);
    //Calculamos las filas por columnas y la posicion
    const int fila = maxX > 0 ? indice / maxX : 0; //El cociente son las filas
    Traza::print("filas: " + Constant::TipoToStr(fila), W_PARANOIC);
    const int col  = maxX > 0 ? indice % maxX : 0; //El resto son las columnas
    Traza::print("cols: " + Constant::TipoToStr(col), W_PARANOIC);

    const short int posX = (short int)(regionPantalla.selX + col*(destw+SEPTHUMB+fillEmptySpaceX) 
        + ((col > 0) ? SEPTHUMB + fillEmptySpaceX: fillEmptySpaceX));
    const short int posY = (short int)(regionPantalla.selY + fila*(desth+SEPTHUMB+fillEmptySpaceY) 
        + ((fila > 0) ? SEPTHUMB + fillEmptySpaceY: fillEmptySpaceY));

    Traza::print("Drawing at position posX: " + Constant::TipoToStr(posX) + " posY: " + Constant::TipoToStr(posY), W_PARANOIC);
    imgLocation->x = posX;
    imgLocation->y = posY;
    imgLocation->w = (short unsigned int)destw;
    imgLocation->h = (short unsigned int)desth;
    ret = true;

    return ret;
}

/**
 * 
 * @param indice
 * @param destw
 * @param desth
 * @param regionPantalla
 * @param dst
 * @return 
 */
bool ImagenGestor::drawImgMem(int indice, int destw, int desth, t_region regionPantalla, SDL_Surface *dst){
    SDL_Rect *imgLocation = new SDL_Rect();
    drawImgMem(indice, destw, desth, regionPantalla, dst, imgLocation);
    delete imgLocation;
}


/**
 * drawImgMem - Dibuja por pantalla la imagen pasada por parametro en un conjunto de
 * imagenes en forma de grid.
 * @param indice
 * @param destw
 * @param desth
 * @param regionPantalla
 * @param dst
 * @param imgLocation: Devuelve el tamanyo y posicion de la imagen dentro del surface dst 
 * @return 
 */
bool ImagenGestor::drawImgMem(int indice, int destw, int desth, t_region regionPantalla, SDL_Surface *dst, SDL_Rect *imgLocation){
        bool ret = false;
        try{
            SDL_Surface *mySurface = NULL;
            //Cargamos la imagen
            ret = loadImgFromMem(&mySurface);
            
            if (mySurface != NULL && ret) {
                calcImgLocationFromIndex(indice, destw, desth, regionPantalla, imgLocation);
                Traza::print("Imagen cargada de memoria: " + Constant::TipoToStr(mySurface->w) + "x" + Constant::TipoToStr(mySurface->h), W_PARANOIC);
                //Creamos la imagen y la pintamos por pantalla
                SDL_Surface *thumbSurface = NULL;
                destw = destw - getBordeLeft() - getBordeRight();
                desth = desth - getBordeTop() - getBordeBottom();

                updateImgScr(mySurface, destw, desth, &thumbSurface, dst->format);
                
                int xAntesCentrar = imgLocation->x;
                int yAntesCentrar = imgLocation->y;
                
                if (imgLocation->w > thumbSurface->w && (centrado || centradoX)){
                    //Si no tenemos que pintar el fondo de la imagen, pintaremos la imagen desplazada para que quede centrada
                    imgLocation->x += (imgLocation->w - thumbSurface->w) / 2;
                } else {
                    imgLocation->x += bordeLeft;
                }

                if (imgLocation->h > thumbSurface->h && (centrado || centradoY)){
                    //Si no tenemos que pintar el fondo de la imagen, pintaremos la imagen desplazada para que quede centrada
                    imgLocation->y += (imgLocation->h - thumbSurface->h) / 2;
                } else {
                    imgLocation->y += bordeTop;
                }
                
                SDL_BlitSurface(thumbSurface, NULL, dst, imgLocation);
                //Guardamos el tamanyo y la posicion de la imagen respecto a la pantalla
                imgLocationRelScreen.x = imgLocation->x - xAntesCentrar;
                imgLocationRelScreen.y = imgLocation->y - yAntesCentrar;
                imgLocationRelScreen.w = imgLocation->w;
                imgLocationRelScreen.h = imgLocation->h;
                SDL_FreeSurface( thumbSurface );
                
                Traza::print("Imagen dibujada", W_PARANOIC);
                ret = true;
            } else {
                Traza::print("Imagen null no dibujada", W_PARANOIC);
            }
        } catch (Excepcion &e) {
             Traza::print("Excepcion Ioutil::drawImgMem:" + string(e.getMessage()), W_ERROR);
        }
        return ret;
}

/**
 * 
 * @param destw
 * @param surfaceW
 * @return 
 */
int ImagenGestor::calcMaxX(int destw, int surfaceW)
{
    int maxX = MAXTHUMBX;
    if (destw > 0 && destw + SEPTHUMB > 0 && surfaceW >= SEPTHUMB){
        maxX = (surfaceW - SEPTHUMB) / (destw + SEPTHUMB);
    }
    //if (maxX == 0) return 1;
    return maxX;
}

/**
 * 
 * @param desth
 * @param surfaceH
 * @return 
 */
int ImagenGestor::calcMaxY(int desth,  int surfaceH)
{
    int maxY = MAXTHUMBY;
    if (desth > 0 && desth + SEPTHUMB > 0 && surfaceH >= SEPTHUMB){
        maxY = (surfaceH - SEPTHUMB) / (desth + SEPTHUMB);
    }
    if (maxY == 0) return 1;
    return maxY;
}

/**
 * 
 * @param mouse_x
 * @param mouse_y
 * @param regionPantalla
 * @return 
 */
unsigned int ImagenGestor::getPosThumb(int mouse_x, int mouse_y, int destw, int desth, t_region regionPantalla){
    int posClick_x = 0;
    int posClick_y = 0;
    int maxX = calcMaxX(destw, regionPantalla.selW);
    int maxY = calcMaxY(desth, regionPantalla.selH);
    unsigned int posicion = 0;
    
    if (regionPantalla.selW > 0 && maxX){
        posClick_x = mouse_x / (regionPantalla.selW / maxX);
    }
    
    if (regionPantalla.selH > 0 && maxY){
        posClick_y = mouse_y / (regionPantalla.selH / maxY);
    }
   posicion = posClick_y * maxX + posClick_x;

    return posicion;
}


/**
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 ImagenGestor::getpixel(SDL_Surface *surface, const int x, const int y)
{
    //int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void ImagenGestor::putpixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel)
{
    //int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


