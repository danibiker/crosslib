#ifndef OBJECT_H
#define OBJECT_H

using namespace std;

#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "ImagenGestor.h"
#include "joymapper.h"

class Object{
    public:
        Object();
        //A virtual member is a member function that can be redefined in a derived class,
        //while preserving its calling properties through references. The syntax for a
        //function to become virtual is to precede its declaration with the virtual keyword:
        virtual ~Object();
        virtual void action(tEvento *);
        virtual void draw();
        virtual void checkPos(tEvento);

        int getObjectType() { return classType; }
        void setObjectType(int val) { classType = val; }


        virtual int getX(){return x;}
        virtual int getY(){return y;}
        virtual int getW(){return w;}
        virtual int getH(){return h;}
        virtual void setX(int var){x = var;}
        virtual void setY(int var){y = var;}
        virtual void setW(int var){if (var > 0) w = var; else w = 0;}
        virtual void setH(int var){if (var > 0) h = var; else h = 0;}
        int getOrigx(){return origx;}
        int getOrigy(){return origy;}
        string getLabel(){return label;}
        string getName(){return name;}
        void setName(string var){name=var;}
        void setLabel(string var){label=var;}
        void setOrigx(int var) {origx = var;}
        void setOrigy(int var) {origy = var;}
        void setEnabled(bool var){enabled = var;}
        bool isEnabled(){return enabled;}
        bool isVisible(){return visible;}
        void setVisible(bool var){visible = var;}
        bool isAnchorBottom(){return anchorBottom;}
        int getMarginBottom(){return marginBottom;}
        void setMarginBottom(int var){marginBottom = var;}
        string getTag(){return tag;}
        void setTag(string var){tag = var;}
        bool isChecked(){return checked;}
        bool isFocus(){return focus;}
        bool isCentered(){return centered;}
        int getSizeLabel(){return sizeLabel;}
        void setChecked(bool var) {checked = var;}
        void setCentered(bool var) {centered = var;}
        void setFocus(bool var){focus = var;}
        void setSizeLabel(int var){sizeLabel = var;}
        void setShadow(bool var){shadow = var;}
        bool showShadow(){return shadow;}
        void setImgDrawed(bool var){imgDrawed = var;}
        bool getImgDrawed(){return imgDrawed;}
        void setOtherDrawed(bool var){otherDrawed = var;}
        bool isOtherDrawed(){return otherDrawed;}
        bool isVerContenedor(){return verContenedor;}
        Object* setVerContenedor(bool var){verContenedor = var; return this;}
        Object* setIcon(int var) {icon = var; return this;}
        int getIcon(){return icon;}
        ImagenGestor * getImgGestor(){return imgGestor;}
        ImagenGestor * getOtherCache(){return otherCache;}

        int getAlpha(){return alpha;}
        void setAlpha(int var){alpha = var;}

        void setTam(int varX, int varY, int varW, int varH){
                x = varX;
                y = varY;
                w = Constant::upperZero(varW);
                h = Constant::upperZero(varH);
                imgDrawed = false;
        }
        string getPopupName(){return popupName;}
        void setPopupName(string var){popupName = var;}
        bool isPopup(){return popup;}
        void setPopup(bool var){popup = var;}
        void setCursor(int var){cursor = var;}
        int getCursor(){return cursor;}
        bool cursorHover(tEvento *evento);

        t_color getColor(){return color;}
        void setColor(t_color var){this->color = var;}

        t_color getTextColor(){return textColor;}
        void setTextColor(t_color var){this->textColor = var;}


    protected:
        ImagenGestor *imgGestor;    //Gestor de cache para el objeto
        ImagenGestor *otherCache;   //Cache de cualquier otro elemento del objeto
        void clear();   //Reset de variables
        int origx, origy;           //Posicion original definida al crear el objeto. Al hacer el centrado sumamos el valor
                                        //de esta variable a las coordenadas que se han calculado para centrar el componente
        string label;               //Label del elemento
        string name;                //identificador del elemento
        bool checked;               //Almacena si el check ha sido seleccionado o no
        bool focus;                 //Especifica si el elemento tiene el foco
        bool centered;              //Espeficica si el elemento ha de mostrarse centrado
        int sizeLabel;              //Tamanyo del label de cada elemento
        int icon;                   //Si el objeto puede tener iconos, se define aqui
        bool enabled;               //Especifica si el componente está habilitado
        bool visible;               //Especifica si el componente es visible
        bool anchorBottom;          //Componenente anclado en el fondo
        int marginBottom;           //Margen inferior del componente
        string tag;                 //Para guardar datos temporales en el objeto
        bool verContenedor;         //Especifica si debe pintar por defecto el contenedor del objeto
        bool shadow;                //Especifica si se debe pintar la sombra del elemento
        bool imgDrawed;             //Especifica si se ha pintado la imagen o el elemento cacheado
        bool otherDrawed;           //Especifica si se ha pintado el label o algun otro elemento cacheado
        int alpha;
        bool popup;
        string popupName;
        int cursor;                 //Cursor que se utiliza en el objeto para cualquier cosa
        t_color color;
        t_color textColor;

    private:
        int x;          //Posicion horizontal del objeto
        int y;          //Posicion vertical del objeto
        int w;          //Ancho en pixeles del objeto
        int h;          //Alto en pixeles del objeto
        int classType;  //Tipo de elemento al que hace referencia el objeto



};

#endif // OBJECT_H
