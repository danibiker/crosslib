#ifndef LISTGROUPCOL_H
#define LISTGROUPCOL_H

#include <iostream>       // std::cout
#include <string>         // std::string
#include <iostream>
#include <vector>
#include "../Constant.h"

using namespace std;

class ListGroupCol
{
    public:
        /** Default constructor */
        ListGroupCol();
        /** Default destructor */
        virtual ~ListGroupCol();
        ListGroupCol(string texto, string valor);
        ListGroupCol(string texto, string valor, int icono, string destino);
        string getTexto(){return texto;}
        string getValor(){return valor;}
        string getDestino(){return destino;}
        int getIcono(){return icono;}
        void setTexto(string var){texto = Constant::txtDisplay(var);}
        void setValor(string var){valor = Constant::txtDisplay(var);}
        void setDestino(string var){destino = var;}
        void setIcono(int var){icono = var;}
        int getSortOrder(){return sortOrder;}
        void setSortOrder(int var){sortOrder = var;}
        void setNumber(bool number);
        bool isNumber() const;
        
        

    protected:
        string texto;
        string valor;
        string destino;
        int icono;
        int sortOrder;
        bool number;
    private:
};

#endif // LISTGROUPCOL_H
