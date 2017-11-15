#ifndef LISTGROUPCOL_H
#define LISTGROUPCOL_H

#include <iostream>       // std::cout
#include <string>         // std::string
#include <iostream>
#include <vector>
#include "Constant.h"

using namespace std;

class ListGroupCol
{
    public:
        /** Default constructor */
        ListGroupCol();
        /** Default destructor */
        virtual ~ListGroupCol();
        ListGroupCol(string texto, string valor);
        ListGroupCol(string texto, string valor, int icono, int destino);
        string getTexto(){return texto;}
        string getValor(){return valor;}
        int getDestino(){return destino;}
        int getIcono(){return icono;}
        void setTexto(string var){texto = Constant::txtDisplay(var);}
        void setValor(string var){valor = Constant::txtDisplay(var);}
        void setDestino(int var){destino = var;}
        void setIcono(int var){icono = var;}
        int getSortOrder(){return sortOrder;}
        void setSortOrder(int var){sortOrder = var;}



    protected:
        string texto;
        string valor;
        int destino;
        int icono;
        int sortOrder;


    private:
};

#endif // LISTGROUPCOL_H
