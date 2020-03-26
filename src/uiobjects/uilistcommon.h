#ifndef UIListCommon_H
#define UIListCommon_H

#include "object.h"

enum {
    SCHEMELIST,
    SCHEMEICONS
};

class UIListCommon : public Object
{
    public:
        /** Default constructor */
        UIListCommon();
        /** Default destructor */
        virtual ~UIListCommon();

        //Metodos a implementar en la clase hija
        void virtual action(tEvento *);
        void virtual draw();
        void virtual checkPos(tEvento evento);
        void virtual inicializarObjeto(int objType);
        unsigned virtual getSize();
        bool virtual searchTextInList(int key);
        string virtual getValue(int var);
        int virtual getDestino(int var);
        void virtual clearLista();
        void virtual resizeLista(unsigned int);
        unsigned int virtual getElemVisibles();

        //Metodos comunes
        void calcularScrPos();
        unsigned int getLetraPopup() { return letraPopup;}
        bool isShowLetraPopup(){return showLetraPopup;}
        void setLetraPopup(unsigned int var){letraPopup = var;}
        void setShowLetraPopup(bool var){showLetraPopup = var;}
        bool getBgLetraPopup(){return bgLetraPopup;}
        void setBgLetraPopup(bool var){ bgLetraPopup = var;}
        unsigned int getPosIniLista(){return posIniLista;}
        unsigned int getPosFinLista(){return posFinLista;}
        unsigned int getPosActualLista(){return posActualLista;}
        void setPosIniLista(unsigned int var){posIniLista = var;setImgDrawed(false);}
        void setPosFinLista(unsigned int var){posFinLista = var;setImgDrawed(false);}
        void setPosActualLista(unsigned int var){posActualLista = var;setImgDrawed(false);}

        unsigned int getLastSelectedPos(){return lastSelectedPos;}
        void setLastSelectedPos(unsigned int var){lastSelectedPos = var;}
        void refreshLastSelectedPos(){lastSelectedPos = posActualLista;}

        void nextSelectedPos() {
            setPosActualLista(getLastSelectedPos());
            nextElemLista();
            refreshLastSelectedPos();
        }
        void prevSelectedPos() {
            setPosActualLista(getLastSelectedPos());
            prevElemLista();
            refreshLastSelectedPos();
        }
        bool isEnableLastSelected(){return enableLastSelected;}
        void setEnableLastSelected(bool var){enableLastSelected = var;}


        void nextElemLista();
        void prevElemLista();
        void prevPageLista();
        void nextPageLista();
        void inicioLista();
        void FinLista();

        bool isShowScrollbar(){return showScrollbar;}
        bool isShowScrollbarAlways(){return showScrollbarAlways;}
        void setShowScrollbar(bool var){showScrollbar = var;}
        void setShowScrollbarAlways(bool var){showScrollbarAlways = var;}

        void setListScheme(int var){listScheme = var;}
        int getListScheme(){return listScheme;}

        void setNIconsHoriz(int var){nIconsHoriz = var;}
        int getNIconsHoriz(){return nIconsHoriz;}

        bool isSmoothDraw(){return smoothDraw;}
        void setSmoothDraw(bool var){smoothDraw = var;}

        SDL_Surface * smoothSfc;

        unsigned int getLastPosActualLista(){return lastPosActualLista;}


    protected:
         //Elementos para las listas
        unsigned int posIniLista;
        unsigned int posFinLista;
        unsigned int posActualLista;
        unsigned int letraPopup;
        bool showLetraPopup;
        bool bgLetraPopup;
        bool showScrollbar;
        bool showScrollbarAlways;
        unsigned int lastSelectedPos; //Ultima posicion que ha sido seleccionada con doble click o seleccion
        bool enableLastSelected;
        int listScheme;
        int nIconsHoriz;
        bool smoothDraw;

        unsigned int lastPosActualLista;



    private:
};

#endif // UIListCommon_H
