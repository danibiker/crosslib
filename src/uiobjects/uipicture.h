#ifndef UIPICTURE_H
#define UIPICTURE_H

#include "object.h"


static const char* VALID_IMGS[] = {".jpg",".jpeg",".bmp", ".gif", ".png"};

class UIPicture : public Object
{
    public:
        UIPicture();
        virtual ~UIPicture();
        void action(tEvento *evento);
        bool loadImgFromFile(string);
        bool loadImgFromBin(const char*, const char *, unsigned long long, unsigned long long);
        void clearImg();
        void setCentrado(bool in){imgGestor->setCentrado(in);}
        void setBestfit(bool in){imgGestor->setBestfit(in);}
        void checkPos(tEvento evento);
        void setSafeToDraw(bool safeToDraw);
        bool isSafeToDraw() const;
        bool isValidImage(string ruta);

    protected:
    private:
        bool safeToDraw;
};

#endif // UIPICTURE_H
