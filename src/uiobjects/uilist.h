#ifndef UILIST_H
#define UILIST_H

#include <uilistcommon.h>


class UIList : public UIListCommon
{
    public:
        UIList();
        virtual ~UIList();
        //Metodos implementados de la clase padre
        void action(tEvento *);
        void draw();
        void inicializarObjeto(int objType);
        unsigned getSize();
        bool searchTextInList(int key);

        //Metodos propios de la lista
        void addElemLista(string, string);
        void addElemLista(string, string, int);
        void addElemLista(string, string, int, int);
        void addElemLista(string, string, int, string imagePath, int);
        void addElemListaScroll(string, string);
        void clearLista();
        void resizeLista(unsigned int);
        bool selectValueInList(string texto);
        //Elementos de la lista
        listaSimple<string> * getListNames() {return listNames;}
        listaSimple<string> * getListValues() {return listValues;}
        listaSimple<string> * getListImage() {return listImage;}
        listaSimple<int> * getListIcons() {return listIcons;}
        listaSimple<int> * getListDestinos(){return listDestinos;}

        string getValue(int row);
        int getDestino(int row);

    protected:
        listaSimple<string> *listValues;
        listaSimple<string> *listNames;
        listaSimple<string> *listImage;
        listaSimple<int> *listIcons;
        listaSimple<int> *listDestinos;
    private:
};

#endif // UILIST_H
