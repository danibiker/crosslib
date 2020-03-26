#ifndef UIART_H
#define UIART_H

#include "object.h"


class UIArt : public Object
{
    public:
        UIArt();
        virtual ~UIArt();
        void setLabel(string var){label = var;imgDrawed = false;}
    protected:
    private:
};

#endif // UIART_H
