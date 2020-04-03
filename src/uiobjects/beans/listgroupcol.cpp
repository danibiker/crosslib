#include "listgroupcol.h"

ListGroupCol::ListGroupCol()
{
    sortOrder = -1;
}

ListGroupCol::~ListGroupCol()
{
    //dtor
}

ListGroupCol::ListGroupCol(string texto, string valor){
    sortOrder = -1;
    this->texto = Constant::txtDisplay(texto);
    this->valor = Constant::txtDisplay(valor);
    this->icono = -1;
    this->destino = -1;
}

ListGroupCol::ListGroupCol(string texto, string valor, int icono, string destino){
    sortOrder = -1;
    this->texto = Constant::txtDisplay(texto);
    this->valor = Constant::txtDisplay(valor);
    this->icono = icono;
    this->destino = destino;
}
