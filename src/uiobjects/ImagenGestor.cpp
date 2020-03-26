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
    screenHeight = 0;
    screenWidth = 0;
    enabledMoveImg = true;
    colorBackground = cBlanco;
    fillBackgroundColour = true;
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
void ImagenGestor::extraerImg(unsigned int pos){
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

        this->loadFromFile(rutaImg.c_str(), inicio, tam);
        cifrarXOR(this->getFile(),tam);
        zoomValue = 0;

    } else {
        throw(Excepcion(ENULL));
    }
}

/**
* extraerImgBin
*/
void ImagenGestor::extraerImgBin(unsigned long long  offset, unsigned long long  offsetAnt){
        unsigned long long inicio=0;
        unsigned long long tam=0;

        if (offsetAnt == 0){
            tam = offset; //Tamanyo
            inicio = 0;  //Offset
        } else {
            tam = offset - offsetAnt; //Tamanyo
            inicio =  offsetAnt - 1; //Offset
        }

        if (tam > 0 && inicio >= 0){
            this->loadFromFile(rutaImg.c_str(), inicio, tam);
            cifrarXOR(this->getFile(),tam);
            zoomValue = 0;
        } else {
             Traza::print("extraerImgBin: Error al calcular el tamanyo de la imagen en base a los offsets", W_ERROR);
        }
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
void ImagenGestor::loadImgFromMem(SDL_Surface **destino){
    loadImgFromMem(this->getFile(), this->getFileSize(), destino);
    if (*destino != NULL){
        //Guardamos la posicion de la imagen relativa a la superficie de destino,
        //que suele ser la propia pantalla. Es util para dibujar sobre una imagen
        //en unas posiciones determinadas por su tamanyo
        this->setImgOrigWidth(((SDL_Surface *)*destino)->w);
        this->setImgOrigHeight(((SDL_Surface *)*destino)->h);
    }
}

/**
* Carga la imagen desde el array binario creado en memoria
*/
void ImagenGestor::loadImgFromMem(char *fileArray, int size, SDL_Surface **destino){
    /*
    IMG_Load_RW(SDL_RWops *src, int freesrc);
    src - The source SDL_RWops as a pointer. The image is loaded from this.
    freesrc - A non-zero value mean is will automatically close/free the src for you.
    */

    if (moveSurface != NULL){
        SDL_FreeSurface(moveSurface);
        moveSurface = NULL;
    }
    SDL_RWops *tempSurfWop = SDL_RWFromMem(fileArray,size);

    if (VIEWALPHA){
        SDL_Surface *memImg = IMG_Load_RW(tempSurfWop,0);
        SDL_FreeRW(tempSurfWop);
        if (memImg == NULL) 
            throw Excepcion(EFIO);
        
        *destino = SDL_DisplayFormatAlpha(memImg);
        SDL_FreeSurface(memImg);
    } else {
        
        *destino = IMG_Load_RW(tempSurfWop,0);
        if (destino == NULL) 
            throw Excepcion(EFIO);
    }
}

void ImagenGestor::makeMoveSurface(SDL_Surface *mySurface, int w, int h){
    if (mySurface != NULL) {
        if (mySurface->w > w || mySurface->h > h)
            moveSurface = SDL_ConvertSurface(mySurface, mySurface->format, mySurface->flags);
    }
}

/**
*
*/
bool ImagenGestor::updateImgScr(SDL_Surface * srcSurface, SDL_Surface * dstSurface){
    SDL_Rect dstRect = { 0,0,0,0 };
    SDL_Surface *bitmap = NULL;

    bool salida = false;
    //double now = SDL_GetTicks();
    if (this->isResize()){
        if (redimension(srcSurface,dstSurface, &bitmap)){
            calcRectCent(&dstRect, bitmap->w, bitmap->h, dstSurface->w, dstSurface->h);
            salida = blitImage(bitmap, dstSurface,&dstRect, true);
        } else {
            calcRectCent(&dstRect, srcSurface->w, srcSurface->h, dstSurface->w, dstSurface->h);
            salida = blitImage(srcSurface, dstSurface,&dstRect, true);
        }
        //objTraza->print("Con resize",(int)(SDL_GetTicks()-now));
    } else if (screenHeight > 0 && screenWidth > 0){
        if (isEnabledMoveImg()){
            makeMoveSurface(srcSurface, screenWidth, screenHeight);    //Para que podamos movernos por la imagen si la estamos viendo a pantalla completa
        }
        calcRectCent(&dstRect, srcSurface->w, srcSurface->h, dstSurface->w, dstSurface->h);
        salida = blitImage(srcSurface, dstSurface,&dstRect, true);
        //objTraza->print("Sin resize",(int)(SDL_GetTicks()-now));
    }

    //Guardamos el tamanyo y la posicion de la imagen respecto a la pantalla
    this->getImgLocationRelScreen()->x = dstRect.x;
    this->getImgLocationRelScreen()->y = dstRect.y;
    this->getImgLocationRelScreen()->w = dstRect.w;
    this->getImgLocationRelScreen()->h = dstRect.h;


    return salida;
}

/**
* calculamos el rectangulo para que quede centrada la imagen
*/
void ImagenGestor::calcRectCent( SDL_Rect *rectCentrado, int srcW, int srcH, int dstW, int dstH){

    int bordesX = this->getBordeLeft() + this->getBordeRight();
    int bordesY = this->getBordeTop() + this->getBordeBottom();

    int centrox = (dstW - (int)srcW + bordesX)/2;
    int centroy = (dstH - (int)srcH + bordesY)/2;

    rectCentrado->x=centrox;
    rectCentrado->y=centroy;
    rectCentrado->w=dstW;
    rectCentrado->h=dstH;

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
bool ImagenGestor::redimension(SDL_Surface *srcSurface, SDL_Surface *dstSurface, SDL_Surface **destino){
    float i_relacion = 1;
    int h_destino = dstSurface->h - this->getBordeTop() - this->getBordeBottom();
    int w_destino = dstSurface->w - this->getBordeLeft() - this->getBordeRight();

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
       SDL_BlitSurface(src, NULL, dst, dstRect);
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
* hace lo mismo que drawImgMem pero almacena en un surface temporal la imagen redimensionada
* para que se pueda mover a trav�s de ella mas rapidamente
*/
bool ImagenGestor::drawZoomImgMem(SDL_Surface *dst){
    try{
        SDL_Surface *mySurface = NULL;
        SDL_Surface *bitmap = NULL;
        bool salida = false;
        SDL_Rect dstRect = { 0,0,0,0 };

        loadImgFromMem(&mySurface);

        if (mySurface != NULL) {
            if (isResize()){
                if (redimension(mySurface, dst, &bitmap)){
                    makeMoveSurface(bitmap, dst->w, dst->h);
                    calcRectCent(&dstRect, bitmap->w, bitmap->h, dst->w, dst->h);
                    salida = blitImage(bitmap, dst,&dstRect, true);
                } else {
                    makeMoveSurface(mySurface, dst->w, dst->h);
                    calcRectCent(&dstRect, mySurface->w, mySurface->h, dst->w, dst->h);
                    salida = blitImage(mySurface, dst,&dstRect, true);
                }
                //objTraza->print("Con resize",(int)(SDL_GetTicks()-now));
            } else {
                makeMoveSurface(mySurface, dst->w, dst->h);
                calcRectCent(&dstRect, mySurface->w, mySurface->h, dst->w, dst->h);
                salida = blitImage(mySurface, dst,&dstRect, true);
                //objTraza->print("Sin resize",(int)(SDL_GetTicks()-now));
            }
            return salida;
        } else {
            return false;
        }
    } catch (Excepcion &e) {
         Traza::print("Excepcion ImagenGestor::drawImg:" + string(e.getMessage()), W_ERROR);
         return false;
     }
}

/**
*
*/
bool ImagenGestor::drawImg(SDL_Surface *dst){
     try{
         SDL_Surface *mySurface = NULL;
         loadImgFromFile(getRuta().c_str(), &mySurface); //Se carga la imagen
         if (mySurface != NULL) {
             return updateImgScr(mySurface,dst);
         } else {
             return false;
         }
     } catch (Excepcion &e) {
         Traza::print("Excepcion Ioutil::drawImg:" + string(e.getMessage()), W_ERROR);
         return false;
     }
}

/**
*
*/
bool ImagenGestor::drawImgMem(SDL_Surface *dst){
        try{
            SDL_Surface *mySurface = NULL;
            loadImgFromMem(&mySurface);

            if (mySurface != NULL) {
                return updateImgScr(mySurface, dst);
            } else {
                return false;
            }
        } catch (Excepcion &e) {
             Traza::print("Excepcion ImagenGestor::drawImg:" + string(e.getMessage()), W_ERROR);
             return false;
         }
}

/**
* drawImgMem - Dibuja por pantalla la imagen pasada por parametro en un conjunto de
* imagenes en forma de grid.
*/
bool ImagenGestor::drawImgMem(int indice, int destw, int desth, t_region regionPantalla, SDL_Surface *dst){
        try{
            SDL_Surface *mySurface;
            int maxX = calcMaxX(destw, regionPantalla.selW);
            int maxY = calcMaxY(desth, regionPantalla.selH);
            Traza::print("maxX: " + Constant::TipoToStr(maxX) + " maxY: " + Constant::TipoToStr(maxY), W_PARANOIC);

            if ((destw <= 0 || desth <= 0) && maxX > 0 && maxY > 0){
                destw  = (regionPantalla.selW - SEPTHUMB - SEPTHUMB * maxX) / maxX;
                desth = (regionPantalla.selH - SEPTHUMB - SEPTHUMB * maxY) / maxY;
            }
            const int maxImg = maxX * maxY;

            if (indice >= maxImg) return false;
            Traza::print("Calculando posiciones", W_PARANOIC);
            //Calculamos las filas por columnas y la posicion
            const int fila = maxX > 0 ? indice / maxX : 0; //El cociente son las filas
            Traza::print("filas: " + Constant::TipoToStr(fila), W_PARANOIC);
            const int col  = maxX > 0 ? indice % maxX : 0; //El resto son las columnas
            Traza::print("cols: " + Constant::TipoToStr(col), W_PARANOIC);
            const short int posX = (short int)(regionPantalla.selX + col*(destw+SEPTHUMB)+ ((col > 0) ? SEPTHUMB : 0));
            const short int posY = (short int)(regionPantalla.selY + fila*(desth+SEPTHUMB)+ ((fila > 0) ? SEPTHUMB : 0));

            Traza::print("Drawing at position posX: " + Constant::TipoToStr(posX) + " posY: " + Constant::TipoToStr(posY), W_PARANOIC);

            SDL_Rect imgLocation = { posX,
                                     posY,
                                     (short unsigned int)destw,
                                     (short unsigned int)desth };


            //Cargamos la imagen
            loadImgFromMem(&mySurface);
            if (mySurface != NULL) {
                Traza::print("Imagen cargada de memoria: " + Constant::TipoToStr(mySurface->w) + "x" + Constant::TipoToStr(mySurface->h), W_PARANOIC);
                //Creamos la imagen y la pintamos por pantalla
                SDL_Surface *thumbSurface;
                if (this->isBestfit()){
                    //Como vamos a rellenar la superficie, nos da igual el tamanyo del recuadro a pintar aunque salgan
                    //bordes por la redimension de la imagen
                    thumbSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, destw, desth, dst->format->BitsPerPixel,0,0,0,0);
                } else {
                    //Creamos un surface con el tamanyo justo de la imagen si se da el caso de que es demasiado peque�a o demasiado grande.
                    float i_relacion = relacion(mySurface,desth,destw);//Calcula cuanto hay que reducir la imagen para que quepa por pantalla
                    if (i_relacion != 1. && i_relacion != 0.){
                        destw = mySurface->w / i_relacion;
                        desth = mySurface->h / i_relacion;
                        //Creamos una imagen que no es transparente que ocupa justo el mismo tamanyo que la imagen que vamos a dibujar
                        //estando reducida. De esta forma evitamos bordes indeseados
                        thumbSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, destw, desth, dst->format->BitsPerPixel,0,0,0,0);
                    } else{
                        //La imagen cabe tal cual en el recuadro, por lo que habra que reducirla y pintarla sobre un fondo
                        //transparente
                        thumbSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, destw, desth, dst->format->BitsPerPixel, rmask , gmask , bmask , amask);
                    }
                }
                Traza::print("Surface creada", W_PARANOIC);

                if (isFillBackgroundColour()){
                    //Si se pinta la imagen en el recuadro que haya definido para ella, se pinta el fondo total
                    //del recuadro del color del fondo de la imagen
                    SDL_FillRect( thumbSurface, NULL, SDL_MapRGB(dst->format, colorBackground.r, colorBackground.g, colorBackground.b) );
                }
                updateImgScr(mySurface, thumbSurface);

                if (imgLocation.w > thumbSurface->w && centrado){
                    //Si no tenemos que pintar el fondo de la imagen, pintaremos la imagen desplazada para que quede centrada
                    imgLocation.x += (imgLocation.w - thumbSurface->w) / 2;
                }

                if (imgLocation.h > thumbSurface->h && centrado){
                    //Si no tenemos que pintar el fondo de la imagen, pintaremos la imagen desplazada para que quede centrada
                    imgLocation.y += (imgLocation.h - thumbSurface->h) / 2;
                }

                SDL_BlitSurface(thumbSurface, NULL, dst, &imgLocation);
                SDL_FreeSurface( thumbSurface );
                Traza::print("Imagen dibujada", W_PARANOIC);
            } else {
                Traza::print("Imagen null no dibujada", W_PARANOIC);
            }
            return true;
        } catch (Excepcion &e) {
             Traza::print("Excepcion Ioutil::drawImgMem:" + string(e.getMessage()), W_ERROR);
             return false;
        }
}

/**
*
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
*/
unsigned int ImagenGestor::getPosThumb(int mouse_x, int mouse_y, t_region regionPantalla){
    int posClick_x = 0;
    int posClick_y = 0;
    int destw = THUMBW;
    int desth = THUMBH;
    int maxX = calcMaxX(destw, regionPantalla.selW);
    int maxY = calcMaxY(desth, regionPantalla.selH);
    unsigned int posicion = 0;

    if ((destw <= 0 || desth <= 0) && maxX > 0 && maxY > 0){
            destw  = (regionPantalla.selW - SEPTHUMB - SEPTHUMB * maxX) / maxX;
            desth = (regionPantalla.selH - SEPTHUMB - SEPTHUMB * maxY) / maxY;
    }

    posClick_x = (mouse_x - regionPantalla.selX) / (destw+SEPTHUMB);
    posClick_y = (mouse_y - regionPantalla.selY) / (desth+SEPTHUMB);

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


