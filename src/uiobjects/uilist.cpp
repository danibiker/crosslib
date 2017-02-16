#include "uilist.h"

/**
*
*/
UIList::UIList(){
    inicializarObjeto(GUILISTBOX);
}

/**
*
*/
UIList::~UIList(){
    delete listNames;
    delete listValues;
    delete listIcons;
    delete listDestinos;
    delete listImage;
}

/**
*
*/
void UIList::inicializarObjeto(int objType){
    UIListCommon::inicializarObjeto(objType);
    listValues = new listaSimple<string>();
    listNames = new listaSimple<string>();
    listIcons = new listaSimple<int>();
    listDestinos = new listaSimple<int>();
    listImage = new listaSimple<string>();
}

/**
*
*/
void UIList::action(tEvento *evento){
    UIListCommon::action(evento);
}

/**
*
*/
void UIList::draw(){
    //cout << "drawing List" << endl;
}

/**
*
*/
unsigned int UIList::getSize(){
    return listNames->getSize();
}

/**
*
*/
void UIList::addElemLista(string name, string value){
    addElemLista(name, value, -1);
}

/**
*
*/
void UIList::addElemLista(string name, string value, int ico){
    addElemLista(name, value, ico, -1);
}

void UIList::addElemLista(string name, string value, int ico, int destino){
    addElemLista(name, value, ico, "", destino);
}

/**
* name: Descripcion del registro en la lista
* value: valor del registro en la lista
* ico: incono que se mostrará en la lista
* destino: Pantalla destino
*/
void UIList::addElemLista(string name, string value, int ico, string imagePath, int destino){

        int elemVisibles = this->getH() / Constant::getMENUSPACE();
        listNames->add(name);
        listValues->add(value);
        listIcons->add(ico);
        listDestinos->add(destino);
        listImage->add(imagePath);

        int tamLista = this->getSize();

        posFinLista = posIniLista + elemVisibles - 1;
        if ((this->getSize() - 1) < posFinLista){
            posFinLista = tamLista - 1;
        }
}

/**
*
*/
void UIList::addElemListaScroll(string name, string value){
        unsigned int elemVisibles = this->getH() / Constant::getMENUSPACE();
        listNames->add(name);
        listValues->add(value);

        //Si se anyade un elemento hacemos scroll hasta el ultimo elemento
        posFinLista = this->getSize() - 1;
        //Calculamos el primer elemento que debe salir
        if (this->getSize() < elemVisibles){
            posIniLista = 0;
        } else {
            posIniLista = this->getSize() - elemVisibles;
        }
        posActualLista = posFinLista;
}


/**
*
*/
void UIList::resizeLista(unsigned int tam){
        delete listNames;
        delete listValues;
        delete listIcons;
        delete listDestinos;
        delete listImage;

        listNames = new listaSimple<string>(tam);
        listValues = new listaSimple<string>(tam);
        listIcons = new listaSimple<int>(tam);
        listDestinos = new listaSimple<int>(tam);
        listImage = new listaSimple<string>(tam);

        UIListCommon::resizeLista(tam);
}

/**
*
*/
void UIList::clearLista(){

        delete listNames;
        delete listValues;
        delete listIcons;
        delete listDestinos;
        delete listImage;

        listNames = new listaSimple<string>();
        listValues = new listaSimple<string>();
        listIcons = new listaSimple<int>();
        listDestinos = new listaSimple<int>();
        listImage = new listaSimple<string>();
        UIListCommon::clearLista();
}

/**
* Busca una cadena de texto en la lista y lo deja seleccionado
*/
bool UIList::selectValueInList(string texto){
    bool someFound = false;
    if (!texto.empty() && this->listValues->getSize() > 0){
        //Ahora recorremos todos los elementos de la lista para comprobar si hay alguno que es igual
        //o contiene el texto que hemos introducido
        for (unsigned int i=0; i < this->listValues->getSize(); i++){
            //Comprobamos que el texto de la lista no este vacio
            string elemMenu = this->listValues->get(i);
            //Traza::print("searchTextInList: comparando: " + elemMenu + " con " + texto, W_DEBUG);
            if (texto.compare(elemMenu) == 0){
                this->setPosActualLista(i);
                someFound = true;
                //Traza::print("searchTextInList: Encontrado", W_DEBUG);
            }
        }
        if (someFound){
            this->calcularScrPos();
        }
    }
    setImgDrawed(false);
    return someFound;
}

/**
* Funcion para buscar un elemento en la lista conforme vamos tecleando
* el texto mientras la lista está con el foco. De esta forma se puede
* buscar el elemento que sea.
*/
bool UIList::searchTextInList(int key){
    bool someFound = false;
    unsigned const int maxFindCadena = 20;
    unsigned const int maxTimeStroke = 500;
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
    if (this->listNames->getSize() > 0){
        unsigned int maxCount = this->listNames->getSize() - 1;
        unsigned int i = 0;
        unsigned int j = 0;
        string elemMenu = "";
        bool salirBucleString = false;
        string strCadena = Constant::TipoToStr(cadena);
        Constant::lowerCase(&strCadena);

        //Recorremos todos los elementos de la lista
        while (i < maxCount){
            //Comprobamos que el texto de la lista no este vacio
            elemMenu = ((string)this->listNames->get(i));
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


/**
* Por defecto obtiene el valor de la primera columna de la fila que se le
* indica por parametro
*/
string UIList::getValue(int row){
    return getListValues()->get(row);
}

/**
* Por defecto obtiene el destino de la primera columna de la fila que se le
* indica por parametro
*/
int UIList::getDestino(int row){
   return getListDestinos()->get(row);
}

