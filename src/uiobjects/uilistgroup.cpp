#include "uilistgroup.h"
#include <stdlib.h>     /* abs */

UIListGroup::UIListGroup(){
    inicializarObjeto(GUILISTGROUPBOX);
    numCols = 0;
    colsAdjustedToHeader = true;
    colAnchorPressed = false;
    posColAnchorPressed = 0;
    initMouseClick = 0;
    this->colSelected = -1;
}

UIListGroup::~UIListGroup(){

}

/**
*
*/
unsigned int UIListGroup::getSize(){
    return listaAgrupada.size();
}

/**
*
*/
void UIListGroup::action(tEvento *evento){

    //Si hay un popup lanzado por este elemento, debe dejar de procesar los eventos
    if (popup == false){
        if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
            this->detectarAnchorClick(evento);
            if (!colAnchorPressed){
                Traza::print("UIListGroup::action: Mouse Pressed: " + this->getName(), W_PARANOIC);
                checkPos(*evento);
                this->setImgDrawed(false);
            }
        } else if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_RELEASED){
           Traza::print("UIListGroup::action: Mouse Released: " + this->getName(), W_DEBUG);
           this->setImgDrawed(false);
           colAnchorPressed = false;
        } else if (evento->isMouseMove && evento->mouse == MOUSE_BUTTON_LEFT && colAnchorPressed){
            int my = evento->mouse_y;
            int mx = evento->mouse_x;
            Traza::print("UIListGroup::action: almacenando posiciones de columna: "
                              + Constant::TipoToStr(mx) + "x" + Constant::TipoToStr(my), W_PARANOIC);
            int tamColCalc = mx - initMouseClick;
            Traza::print("UIListGroup::action: hay que sumar a la cabecera: "
                         + Constant::TipoToStr(tamColCalc) + "En total: "
                         + Constant::TipoToStr(initColWith + tamColCalc), W_PARANOIC);
            if (mx <= this->getX() || (initColWith + tamColCalc) <= 0){
                //Para no ocultar completamente ninguna columna si la arrastramos a los limites de la tabla
                setHeaderWith(posColAnchorPressed, INPUTCONTENT * 5);
                this->setImgDrawed(false);
            } else if (mx > this->getX() && mx < this->getX() + this->getW()){
                //Calculamos la posicion de la columna mientras arrastramos
                setHeaderWith(posColAnchorPressed, initColWith + tamColCalc);
                this->setImgDrawed(false);
            }
        } else if (evento->isMouseMove){
            if (cursorInAnchor(evento) >= 0){
                this->setCursor(cursor_resize);
            } else {
                this->setCursor(cursor_arrow);
            }
        } else {
            //Traza::print("UIListGroup::action: Pasando al padre: " + this->getName(), W_DEBUG);
            UIListCommon::action(evento);
        }
    }
}

/**
*
*/
void UIListGroup::draw(){
    cout << "drawing ListGroup" << endl;
}

/**
*
*/
void UIListGroup::checkPos(tEvento evento){

    int my = evento.mouse_y;
    int mx = evento.mouse_x;
    if (mx >= this->getX() && mx <= this->getX() + this->getW() &&
        my >= this->getY() && my <= this->getY() + this->getH()){
        int tempY = this->getY();
        //La posicion y de esta lista agrupada es distinta porque debemos reservar espacio para la cabecera
        //Con este workaround conseguimos que el tamanyo del objeto se reduzca lo necesario para que
        //los calculos de la superclase sean correctos
        this->setY(tempY + Constant::getMENUSPACE());
        UIListCommon::checkPos(evento);
        //Restablecemos la posicion del objeto
        this->setY(tempY);

        /**Buscando la columna pulsada*/
        int const posmxCol = evento.mouse_x - INPUTCONTENT;
        //Guardamos el ancho acumulado de cada columna
        int acumWidth = 0, lastAcumWidth = 0;
        this->colSelected = -1;
        for (unsigned int i=0; i < headersWith.size() && colSelected == -1; i++){
            acumWidth += headersWith.at(i);
            if (posmxCol >= this->getX() + lastAcumWidth &&
                posmxCol <= this->getX() + acumWidth)
            {
                colSelected = i;
                Traza::print("Col. Pulsada",i, W_DEBUG);
            } else {
                lastAcumWidth = acumWidth;
            }
        }
    }
}

/**
*
*/
unsigned int UIListGroup::getElemVisibles(){
    if (this->getH() >= 0 && Constant::getMENUSPACE() > 0)
        return (this->getH() - Constant::getMENUSPACE()) / Constant::getMENUSPACE();
    else
        return 0;
}

void UIListGroup::addElemLista(ListGroupElement * newElement){
    //A la altura del objeto le restamos el espacio reservado para las cabeceras de la lista
    int elemVisibles = getElemVisibles();
    //Comprobamos que el numero de columnas a anyadir sea el mismo de las que ya existen
    if (numCols > 0 && numCols != newElement->GetListGroupCol().size()){
        throw(Excepcion(ENULL, "UIListGroup: Anyadiendo filas con numero distinto de columnas"));
    }
    numCols = newElement->GetListGroupCol().size();
    //Anaydimos la fila
    listaAgrupada.push_back(newElement);
    //Actualizamos los limites de la lista visible
    int tamLista = this->getSize();
    posFinLista = posIniLista + elemVisibles - 1;
    if ((this->getSize() - 1) < posFinLista){
        posFinLista = tamLista - 1;
    }
}

/**
 * 
 * @param newRow
 */
void UIListGroup::addElemLista(vector <ListGroupCol *> newRow){
    ListGroupElement *listGroupElement = new (ListGroupElement);
    listGroupElement->SetListGroupCol(newRow);
    addElemLista(listGroupElement);
}

/**
*
*/
void UIListGroup::setHeaderLista(vector <ListGroupCol *> newRow){
    listaHeaders.clear();
    listaHeaders.push_back(newRow);
}

/**
*
*/
ListGroupCol * UIListGroup::getCol(unsigned int row, unsigned int col){
    if (getRow(row)->GetListGroupCol().size() > col)
        return getRow(row)->GetListGroupCol().at(col);
    else
        throw(Excepcion(ENULL, "UIListGroup: Columna vacia"));
}

/**
*
*/
ListGroupElement * UIListGroup::getRow(unsigned int row){
    if (listaAgrupada.size() > row)
        return listaAgrupada.at(row);
    else
        throw(Excepcion(ENULL,"UIListGroup: Filas vacias"));
}

/**
*
*/
ListGroupCol * UIListGroup::getHeaderCol(int col){
    if (!listaHeaders.empty())
        return listaHeaders.at(0).at(col);
    else
        throw(Excepcion(ENULL, "UIListGroup: Columna de cabeceras vacia"));
}

/**
*
*/
void UIListGroup::calcularScrPos(){
    int tempH = this->getH();
    //La altura de esta lista agrupada es menor porque debemos reservar espacio para la cabecera
    //Con este workaround conseguimos que el tamanyo del objeto se reduzca lo necesario para que
    //los calculos de la superclase sean correctos
    this->setH(tempH - Constant::getMENUSPACE());
    UIListCommon::calcularScrPos();
    //Restablecemos el tamanyo del objeto
    this->setH(tempH);
}

/**
*
*/
int UIListGroup::getHeaderWith(unsigned int col){
    if (headersWith.size() > col )
        return headersWith.at(col);
    else
        return 100; //Valor por defecto
}

void UIListGroup::setHeaderWith(int col, int value){
    headersWith[col] = value;
}

/**
* Por defecto obtiene el valor de la primera columna de la fila que se le
* indica por parametro
*/
string UIListGroup::getValue(int row){
    return getCol(row, 0)->getValor();
}

/**
* Por defecto obtiene el valor de la primera columna de la fila que se le
* indica por parametro
*/
string UIListGroup::getText(int row){
    return getCol(row, 0)->getTexto();
}

/**
* Por defecto obtiene el destino de la primera columna de la fila que se le
* indica por parametro
*/
string UIListGroup::getDestino(int row){
    return getCol(row, 0)->getDestino();
}

/**
*
*/
void UIListGroup::resizeLista(unsigned int tam){
        listaAgrupada.clear();
        listaAgrupada.reserve(tam);
        UIListCommon::resizeLista(tam);
}

/**
*
*/
void UIListGroup::clearLista(){
        listaAgrupada.clear();
        UIListCommon::clearLista();
}

int UIListGroup::cursorInAnchor(tEvento *evento){
    //Le restamos el INPUTCONTENT porque Ioutil::drawUIListGroupBox se lo anyade antes de hacer
    //la llamada a drawListGroupContent
    int my = evento->mouse_y - INPUTCONTENT;
    int mx = evento->mouse_x - INPUTCONTENT;
    //Guardamos el ancho acumulado de cada columna
    int acumWidth = 0;
    //Especificamos el margen en pixeles para hacer click a la columna
    int margenClick = 5;
    //Variables para almacenar los margenes del click
    int posXI = 0;
    int posXD = 0;
    int posYU = this->getY() - INPUTCONTENT;
    int posYD = this->getY() - INPUTCONTENT + Constant::getMENUSPACE();
    //Variable para almacenar la columna que se ha presionado
    int anchorPos = -1;

    if (my >= posYU && my <= posYD){
        for (unsigned int i=0; i < this->getSizeCol() ; i++){
            acumWidth += this->getHeaderWith(i) + (i > 0 ? INPUTCONTENT : 0);
            posXI = this->getX() + acumWidth - margenClick;
            posXD = this->getX() + acumWidth + margenClick;

            //Comprobamos si la posicion del click pertenece a algun separador de columnas
            if (mx >= posXI && mx <= posXD){
                anchorPos = i;
            }
        }
    }

    return anchorPos;
}


/**
*
*/
void UIListGroup::detectarAnchorClick(tEvento *evento){
    //Le restamos el INPUTCONTENT porque Ioutil::drawUIListGroupBox se lo anyade antes de hacer
    //la llamada a drawListGroupContent
    int my = evento->mouse_y - INPUTCONTENT;
    int mx = evento->mouse_x - INPUTCONTENT;
    //Guardamos el ancho acumulado de cada columna
    int acumWidth = 0;
    //Variables para almacenar los margenes del click
    int posYU = this->getY() - INPUTCONTENT;
    int posYD = this->getY() - INPUTCONTENT + Constant::getMENUSPACE();

    //Variable para almacenar la columna que se ha presionado
    int colPressed = -1;
    //Especifica si se ha pulsado un anchor de la columna
    int cursorAnchor = cursorInAnchor(evento);

    if (!colAnchorPressed && my >= posYU && my <= posYD){
        for (unsigned int i=0; i < this->getSizeCol() ; i++){
            acumWidth += this->getHeaderWith(i) + (i > 0 ? INPUTCONTENT : 0);
            //Comprobamos si la posicion del click pertenece a algun separador de columnas
            if (cursorAnchor >= 0 && (unsigned int)cursorAnchor == i){
                Traza::print("UIListGroup::action: Pulsado el separador de columnas: " + this->getName() + " en la pos: ", i, W_DEBUG);
                colAnchorPressed = true;
                posColAnchorPressed = i;
                initMouseClick = evento->mouse_x;
                initColWith = getHeaderWith(posColAnchorPressed);
            }
            //Comprobamos si el click es a alguna columna
            if(mx < acumWidth + this->getX() && colPressed < 0){
                //Guardamos la columna que hemos presionado
                colPressed = i;
            } else {
                //Al resto de columnas las ponemos sin orden
                getHeaderCol(i)->setSortOrder(-1);
            }
        }
    }

    if (!colAnchorPressed && colPressed >= 0){
        sort(colPressed);
    }
}




/**
*
*/
void UIListGroup::sort(int col){
    if (getSize() > 0){
        int order = getHeaderCol(col)->getSortOrder() + 1;
        if (order > 1)
            order = 0;
        getHeaderCol(col)->setSortOrder(order);
        //Restamos 1 al tamanyo para que no ordene el boton de volver
        quickSort(listaAgrupada, 0, getSize(), col);
    }
}

/**
*
*/
void UIListGroup::quickSort(vector <ListGroupElement *>& A, int p, int q, int col){
    int r;
    if(p < q){
        r=partition(A, p,q, col);
        quickSort(A,p,r,col);
        quickSort(A,r+1,q,col);
    }
}

/**
*
*/
int UIListGroup::partition(vector <ListGroupElement *>& A, int p, int q, int col){
    string x = A[p]->GetListGroupCol().at(col)->getValor();
    char temp[x.size()+1];//as 1 char space for null is also required
    strcpy(temp, x.c_str());

    int i=p;
    int j;

    int order = getHeaderCol(col)->getSortOrder();
    ListGroupCol* column;
    
    for(j=p+1; j<q; j++){
        column = A[j]->GetListGroupCol().at(col);
        if (order == 0){
            if ( (!column->isNumber() && Constant::stricmp(column->getValor().c_str(), temp) <= 0)
                    || (column->isNumber() && Constant::strToTipo<size_t>(column->getValor()) <= Constant::strToTipo<size_t>(temp)) ){
                i=i+1;
                swap(A[i],A[j]);
            }
        } else {
            if ( (!column->isNumber() && Constant::stricmp(column->getValor().c_str(), temp) > 0)
                    || (column->isNumber() && Constant::strToTipo<size_t>(column->getValor()) > Constant::strToTipo<size_t>(temp)) ){
                i=i+1;
                swap(A[i],A[j]);
            }
        }
    }
    swap(A[i],A[p]);
    return i;
}


/**
* Funcion para buscar un elemento en la lista conforme vamos tecleando
* el texto mientras la lista esta con el foco. De esta forma se puede
* buscar el elemento que sea.
*/
bool UIListGroup::searchTextInList(int key){
    bool someFound = false;
    unsigned const int maxFindCadena = 20;
    unsigned const int maxTimeStroke = 1000;
    static char cadena[maxFindCadena] = {"\0"};
    static unsigned long lastTimeTick = 0;
    unsigned int lenCadena = strlen(cadena);

    //Se concatena el texto introducido para crear un string si el tiempo entre pulsaciones
    //de teclas es inferior a 1 segundo
    if (SDL_GetTicks() - lastTimeTick > maxTimeStroke){
        memset(cadena, '\0', maxFindCadena);
        cadena[0] = (char)(key);
        Traza::print("searchTextInList: Cadena reseteada", W_PARANOIC);
    } else {
        cadena[lenCadena] = (char)(key);
        Traza::print("searchTextInList: Concatenando: " + string(cadena), W_PARANOIC);
    }
    lenCadena = strlen(cadena);
    Traza::print("searchTextInList: lenCadena", lenCadena, W_PARANOIC);

    //Ahora recorremos todos los elementos de la lista para comprobar si hay alguno que es igual
    //o contiene el texto que hemos introducido
    if (this->getSize() > 0){
        unsigned int maxCount = this->getSize() - 1;
        unsigned int i = 0;
        unsigned int j = 0;
        string elemMenu = "";
        bool salirBucleString = false;
        string strCadena = Constant::TipoToStr(cadena);
        Constant::lowerCase(&strCadena);

        //Recorremos todos los elementos de la lista
        while (i < maxCount && !someFound){
            //Comprobamos que el texto de la lista no este vacio
            elemMenu = this->getCol(i,0)->getTexto();
            Constant::lowerCase(&elemMenu);
            if ( elemMenu.length() > 0 && lenCadena > 0){
                while (j < elemMenu.length() && j < lenCadena && !salirBucleString) {
                    //Vamos recorriendo las letras del elemento
                    if (elemMenu[j] == strCadena[j]){
                        //if (j == lenCadena - 1 && i > posInicialLista){
                        if (j == lenCadena - 1){
                            Traza::print("searchTextInList: seleccionamos: " + elemMenu, W_PARANOIC);
                            this->setPosActualLista(i);
                            someFound = true;
                        }
                    } else {
                        salirBucleString = true;
                    }
                    j++;
                }
                j = 0;
                salirBucleString = false;
            }
            i++;
        }

        Traza::print("searchTextInList: Calculando posiciones", W_PARANOIC);
        if (someFound){
            this->calcularScrPos();
        }
    }

    lastTimeTick = SDL_GetTicks();
    Traza::print("searchTextInList: end", W_PARANOIC);
    return someFound;
}
