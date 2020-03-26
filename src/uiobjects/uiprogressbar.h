#ifndef UIPROGRESSBAR_H
#define UIPROGRESSBAR_H

#include "object.h"

typedef enum {HINT_TIME = 0,
              HINT_PERCENT} tTypesHint;

class UIProgressBar : public Object
{
    public:
        UIProgressBar();
        virtual ~UIProgressBar();
        virtual void checkPos(tEvento evento);
        virtual void action(tEvento *evento);

         //Para el progressbar
        void setProgressMin(int var) {min = var;}
        void setProgressMax(int var) {max = var;}
        void setProgressPos(int var) {pos = var;}
        int getProgressMin(){return min;}
        int getProgressMax(){return max;}
        int getProgressPos(){return pos;}

        void setLastTimeTick(unsigned long var) {lastTimeTick = var;}
        unsigned long getLastTimeTick(){return lastTimeTick;}

        void setPosXNow(int var) {posXNow = var;}
        int getPosXNow(){return posXNow;}

        void setPosYNow(int var) {posYNow = var;}
        int getPosYNow(){return posYNow;}

        void setTypeHint(int var) {typeHint = var;}
        int getTypeHint(){return typeHint;}

        void setMouseOverBar(bool var) {mouseOverBar = var;}
        bool getMouseOverBar(){return mouseOverBar;}

        void setShowHint(bool var) {showHint = var;}
        bool isShowHint(){return showHint;}



    protected:
        //Para el elemento progressBar
        int min;
        int max;
        int pos;

        unsigned long lastTimeTick;
        int posXNow;
        int posYNow;
        int typeHint;
        bool mouseOverBar;
        bool showHint;
    private:


};

#endif // UIPROGRESSBAR_H
