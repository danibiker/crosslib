#ifndef UIPOPUPMENU_H
#define UIPOPUPMENU_H

#include <uilist.h>
#include <SDL/SDL_ttf.h>


class UIPopupMenu : public UIList
{
    public:
        UIPopupMenu();
        virtual ~UIPopupMenu();
        void action(tEvento *);
        void draw();
        void checkPos(tEvento evento);
        void checkPosAndExit(tEvento evento);

        void setCallerPopup(Object *var){callerPopup = var;}
        Object * getCallerPopup(){return callerPopup;}
        void addElemLista(string name, string value, int ico, int destino);
        void addElemLista(string name, string value, int ico);
        void addElemLista(string name, string value);
        void clearLista();
        void closePopup();

        void setFont(TTF_Font* var){font = var;}
        TTF_Font* getFont(){return font;}
        int fontStrLen(string str);

        void setAutosize(bool var){autosize = var;}
        bool getAutosize(){return autosize;}

    protected:
    private:
        Object *callerPopup;
        TTF_Font* font; //Fuente de textos
        bool autosize;
        int maxElemW;
};

#endif // UIPOPUPMENU_H
