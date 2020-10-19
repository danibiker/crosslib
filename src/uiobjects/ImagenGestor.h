#ifndef IMAGENGESTOR_H_INCLUDED
#define IMAGENGESTOR_H_INCLUDED

#include "Fileio.h"
#include "SHA1.h"
#include "ListaSimple.h"
#include "Excepcion.h"
#include "Dirutil.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "../httpcurl/image/uiimgencoder.h"
#include "common.h"

#define MOVE_UP 0 
#define MOVE_DOWN 1 
#define MOVE_LEFT 2
#define MOVE_RIGHT 3

class ImagenGestor : public Fileio{
    public :
        ImagenGestor();
        ~ImagenGestor();
        char * calculaPass(string );
        void cargarOffsets();
        bool extraerImg(unsigned int);
        //void extraerImgBin(string, string);
        bool extraerImgBin(unsigned long long , unsigned long long );
        listaSimple<unsigned long long> *offsets;

        void catDiractualRuta(string);
        void setRuta(string);
        string getRuta(){return rutaImg;}
        double incZoom();
        double decZoom();
        double getZoom(){return zoomValue;}
        bool isSmooth(){return smooth;}
        bool isResize(){return resize;}
        bool isBestfit(){return bestfit;}
        bool isEnabledMoveImg(){return enabledMoveImg;}
        void setEnabledMoveImg(bool var){enabledMoveImg = var;}
        bool isEnabledZoom(){return enabledZoom;}
        
        bool isCentrado(){return centrado;}
        bool isCentradoX(){return centradoX;}
        bool isCentradoY(){return centradoY;}
        

        ImagenGestor* setCentrado(bool in){centrado = in; return this;}
        ImagenGestor* setBestfit(bool in){bestfit = in; return this;}
        ImagenGestor* setResize(bool in){resize = in; return this;}
        ImagenGestor* setZoom(double tempZoom){zoomValue = tempZoom; return this;}
        ImagenGestor* setSmooth(bool in){smooth = in; return this;}
        ImagenGestor* setTopDif(int var){top = var;return this;}
        ImagenGestor* setLeftDif(int var){left = var;return this;}
        ImagenGestor* setCentradoX(bool in){centradoX = in; return this;}
        ImagenGestor* setCentradoY(bool in){centradoY = in; return this;}
        ImagenGestor* setEnabledZoom(bool var){enabledZoom = var; return this;}



        int getTopDif(){return top;}
        int getLeftDif(){return left;}

//        string guardarImgBin(Menu *, string);
        unsigned long long guardarImgBin(string, string, unsigned long long);
        unsigned long long abrirImgBin(string);
        void guardarImgIni();

        void incTopDif(){top+=MOVEINCREMENT;}
        void decTopDif(){top-=MOVEINCREMENT;}
        void decLeftDif(){left-=MOVEINCREMENT;}
        void incLeftDif(){left+=MOVEINCREMENT;}

        ImagenGestor* setBordeTop(int var){bordeTop = var;return this;}
        ImagenGestor* setBordeBottom(int var){bordeBottom = var;return this;}
        ImagenGestor* setBordeLeft(int var){bordeLeft = var;return this;}
        ImagenGestor* setBordeRight(int var){bordeRight = var;return this;}

        int getBordeTop(){return bordeTop;}
        int getBordeBottom(){return bordeBottom;}
        int getBordeLeft(){return bordeLeft;}
        int getBordeRight(){return bordeRight;}

        SDL_Surface *surface;
        SDL_Surface *getSurface();
        void setSurface(SDL_Surface *srf);

        double getZoomX(){return zoomX;}
        double getZoomY(){return zoomY;}
        void setZoomX(double var){zoomX = var;}
        void setZoomY(double var){zoomY = var;}
        t_posicion getImgLocationRelScreen(){return imgLocationRelScreen;}

        int getImgOrigWidth(){return imgOrigWidth;}
        int getImgOrigHeight(){return imgOrigHeight;}
        void setImgOrigWidth(int var){imgOrigWidth = var;}
        void setImgOrigHeight(int var){imgOrigHeight = var;}
        

        void clearImg(){this->clearFile();}

        //Funciones para cargar las imagenes en memoria
        bool loadImgDisplay(const char *uri, SDL_Surface **destino);
        bool loadImgFromFile(const char *uri, SDL_Surface **destino);
        bool loadImgFromFileZoom(const char *uri, SDL_Surface **destino, SDL_Rect *dstrect);
        bool loadImgFromMem(SDL_Surface **destino);
        bool loadImgFromMem(char *fileArray, int size, SDL_Surface **destino);
        unsigned long long writeImg(string rutaTemp, t_region regionPantalla, SDL_PixelFormat *format);
        unsigned long long writeImg(string rutaTemp, SDL_Surface *surface);
        
        bool updateImgScr(SDL_Surface * srcSurface, SDL_Surface * dstSurface);
        bool updateImgScr(SDL_Surface * srcSurface, int dstW, int dstH, SDL_Surface **dstSurface, SDL_PixelFormat *format);
        float relacion(SDL_Surface *source, int alto, int ancho);
        void calcRectCent( SDL_Rect *rectCentrado, int srcW, int srcH, int dstW, int dstH);
        bool redimension(SDL_Surface *srcSurface, int dstW, int dstH, SDL_Surface **destino);
        bool blitImage(SDL_Surface *src, SDL_Surface *dst, SDL_Rect *dstRect, bool freesrc);
        void makeMoveSurface(SDL_Surface *mySurface, int w, int h);
        SDL_Surface *getMoveSurface(){return moveSurface;}
        bool drawImgMem(int indice, int destw, int desth, t_region regionPantalla, SDL_Surface *dst);
        bool drawImgMem(int indice, int destw, int desth, t_region regionPantalla, SDL_Surface *dst, SDL_Rect *imgLocation);
        unsigned int getPosThumb(int mouse_x, int mouse_y, int destw, int desth, t_region regionPantalla);
        int calcMaxX(int destw, int surfaceW);
        int calcMaxY(int desth,  int surfaceH);
        bool calcImgLocationFromIndex(int indice, int destw, int desth, t_region regionPantalla, SDL_Rect *imgLocation);
        t_color getColorBackground(){return colorBackground;}
        void setColorBackground(t_color var){colorBackground = var;}
        bool isFillBackgroundColour(){return fillBackgroundColour;}
        void setFillBackgroundColour(bool var){fillBackgroundColour = var;}
        Uint32 getpixel(SDL_Surface *surface, const int x, const int y);
        void putpixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel);
        bool centerAndBlit(SDL_Surface *srcSurface, SDL_Surface *dstSurface, SDL_Rect *dstRect);
        void blitImgMoved(SDL_Surface * src, SDL_Surface * dst, int dirMove);

    private :
        char hash[4];
        string rutaImg;
        string rutaInf;
        void cifrarXOR(char *, unsigned int);
        double zoomValue;
        bool resize; //Especifica si se debe redimensionar la imagen o dejarla a su tamanyo real
        bool bestfit; //Especifica si se debe redimensionar la imagen para encajar al m�ximo en la pantalla
        bool smooth;
        bool centrado;
        bool centradoX;
        bool centradoY;
        int bordeTop;
        int bordeBottom;
        int bordeLeft;
        int bordeRight;
        double zoomX;
        double zoomY;
        int top, left;
        t_posicion imgLocationRelScreen;
        int imgOrigWidth;
        int imgOrigHeight;
        bool enabledMoveImg;
        bool enabledZoom;
        //Para las cargas de imagenes en memoria y redimensionado
        SDL_Surface * moveSurface;
        t_color colorBackground;
        bool fillBackgroundColour;
        
};


#endif // IMAGENGESTOR_H_INCLUDED
