#ifndef MENUOBJECT_H_INCLUDED
#define MENUOBJECT_H_INCLUDED

#include <sstream>
#include <string.h>
#include "Constant.h"
#include "Excepcion.h"
#include "ListaSimple.h"
#include "ImagenGestor.h"
#include "joymapper.h"

#include "object.h"
#include "uilabel.h"
#include "uibutton.h"
#include "uilist.h"
#include "uiprogressbar.h"
#include "uiart.h"
#include "uicheck.h"
#include "uipanel.h"
#include "uipanelborder.h"
#include "uipicture.h"
#include "uiinput.h"
#include "uipopupmenu.h"
#include "uicombobox.h"
#include "uilistgroup.h"
#include "uitextelementsarea.h"
#include "uispectrum.h"
#include "uislider.h"



using namespace std;

    /**
    * Gestor de objetos
    **/
    class tmenu_gestor_objects{
        private:
            int focusPos;   //Almacena la posicion del elemento que tiene el foco
            int size;   //Numero de elementos que se han anyadido
            int w, h;   //Almacena el ancho y el alto de la pantalla
            //int mouse_x, mouse_y;   //Almacena la posicion del mouse en la que se ha hecho click

            //Declaramos el array de punteros para almacenar los objetos del menu
            Object *arrayNewObj[MAXOBJECTS];
            void repaintObject(string tname);
            static Object foo;

        public:
            tmenu_gestor_objects(int, int); //Constructor
            ~tmenu_gestor_objects(); //Destructor

            void centrarObjeto(Object *object);
            Object * getObjByPos(int);
            Object * getObjByName(string); //Obtiene el elemento cuyo nombre se especifica por parametro
            Object * getObjByXY(int, int);


            void procEvent(tEvento );   //Procesa los eventos para el componentes del menu que tiene el foco
            void clear();   //Resetea variables
            Object * add(string tname, int ttype, int tx, int ty, int tw, int th, string tlabel, bool centered);    //Anyade un elemento
            void setAreaObjMenu(int width, int height);    //Especifica el area del menu. Es util para centrar los elementos en la pantalla

            void setFocus(int);     //Da el foco al elemento de la posicion especificada
            void setFocus(string ); //Da el foco al elemento
            void setFirstFocus();
            void clearFocus();
            int getFocus();  //Obtiene la posicion del elemento que tiene el foco
            void findNextFocus();   //Da el foco al elemento siguiente, ignorando los que no pueden tener el foco
            int getSize(){return size;}
            void procAllEvent(tEvento evento);
            void resetElements();
    };

#endif // MENUOBJECT_H_INCLUDED
