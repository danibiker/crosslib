#ifndef UISPECTRUM_H
#define UISPECTRUM_H

#include "object.h"

typedef enum {
        BARSPECTRUM,          //0
        WAVESPECTRUM,           //1
        MAXSPECTRUMVIS        //2
    } spectrumTypes;

class UISpectrum : public Object{
    public:
        /** Default constructor */
        UISpectrum();
        /** Default destructor */
        virtual ~UISpectrum();
        Sint16 *buf;
        int *arrFreqVis;

        t_color getColorFondo(){return colorFondo;}
        void setColorFondo(t_color var){this->colorFondo = var;}
        long getBuffSize(){return buffSize;}
        void setBuffSize(long var){buffSize = var;}
        void setZoom(int var){zoom = var;}
        int getZoom(){return zoom;}
        void calcZoom(int width);

        int spectrumVisualizer;
        int getSpectrumVisualizer(){return spectrumVisualizer;}


        void incZoom(){
            if (zoom < maxZoom)
                zoom++;
        }

        void decZoom(){
            if (zoom > 1)
                zoom--;
        }

        void action(tEvento *evento);

    protected:
    private:
        t_color colorFondo;
        long buffSize;
        int zoom;
        int maxZoom;
};

#endif // UISPECTRUM_H
