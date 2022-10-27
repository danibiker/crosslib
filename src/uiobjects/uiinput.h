#ifndef UIINPUT_H
#define UIINPUT_H

#include "object.h"
#include "uiobjects/soutils/SOUtils.h"


class UIInput : public Object
{
    public:
        UIInput();
        virtual ~UIInput();
        void action(tEvento *);


        void setText(string var){ textAreaText = var; posChar = (var.length() > 0) ? var.length(): 0; offset = 0;}
        unsigned int getOffset(){return offset;}
        void setOffset(unsigned int var){offset = var;}
        string getText(){return textAreaText;}
        unsigned int getPosArrayLetras(){return posArrayLetras;}
        void setPosArrayLetras(unsigned int var){posArrayLetras = var;}
        int buscaPosicion(char letra);
        unsigned int getSize();
        unsigned int getPosChar(){ return posChar; }
        void setPosChar(unsigned int var){posChar = var;}
        void borrarSeleccion();

        int getSelectionIni(){return selectionIni;}
        int getSelectionFin(){return selectionFin;}


    protected:
    private:
        //Para la introduccion de datos en inputs
        unsigned int posChar; //Posicion dentro del input
        unsigned int offset; //Posicion dentro del input
        unsigned int posArrayLetras; //Posicion del array de letras que vamos ciclando
        string textAreaText;
        int selectionIni;
        int selectionFin;

        void resetSelection();


};

#endif // UIINPUT_H
