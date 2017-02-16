#ifndef UIBUTTON_H
#define UIBUTTON_H

#include <object.h>


class UIButton : public Object
{
    public:
        UIButton();
        virtual ~UIButton();
        void checkPos(tEvento evento);
        void action(tEvento *evento);
        void draw();


    protected:
    private:
};

#endif // UIBUTTON_H
