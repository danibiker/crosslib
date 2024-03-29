#ifndef UIIMGDOWNLOADER_H
#define UIIMGDOWNLOADER_H

#include "httpcurl/httputil.h"
#include "uiobjects/Dirutil.h"
#include "SDL/SDL.h"
#include "uiobjects/ImagenGestor.h"
#include "httpcurl/image/uiimgencoder.h"


class UIImgDownloader
{
    public:
        /** Default constructor */
        UIImgDownloader();
        /** Default destructor */
        virtual ~UIImgDownloader();

        bool download(string url, string strFileName, int w, int h);
        bool downloadToJpg(string url, string strFileName);
        bool downloadToJpgAndThumbnail(string url, string strFileName, int w, int h);

        int getQuality(){return quality;}
        void setQuality(int var){quality = var;}
        t_color getBgColor(){return bgColor;}
        void setBgColor(t_color var){bgColor = var;}

    protected:
    private:
        int quality;
        t_color bgColor;
};

#endif // UIIMGDOWNLOADER_H
