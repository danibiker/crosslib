#ifndef UISLIDER_H
#define UISLIDER_H

#include "uiprogressbar.h"

class UISlider : public UIProgressBar
{
    public:
        /** Default constructor */
        UISlider();
        /** Default destructor */
        virtual ~UISlider();
        void checkPos(tEvento evento);

    protected:

    private:
};

#endif // UISLIDER_H
