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
    this->texto = texto;
    this->valor = valor;
    this->icono = -1;
    this->destino = -1;
}

ListGroupCol::ListGroupCol(string texto, string valor, int icono, int destino){
    sortOrder = -1;
    this->texto = texto;
    this->valor = valor;
    this->icono = icono;
    this->destino = destino;
}
