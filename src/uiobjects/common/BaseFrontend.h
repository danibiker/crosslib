/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   BaseFrontend.h
 * Author: Ryuk
 *
 * Created on 29 de marzo de 2020, 12:40
 */

#ifndef BASEFRONTEND_H
#define BASEFRONTEND_H

#include <string>
#include <vector>

#include "uiobjects/common/Ioutil.h"
#include "uiobjects/Menuobject.h"
#include "uiobjects/Colorutil.h"
#include "uiobjects/ImagenGestor.h"
#include "uiobjects/common/Icogestor.h"
#include "uiobjects/Launcher.h"
#include "uiobjects/thread.h"
#include "uiobjects/uipicture.h"
#include "uiobjects/uiinput.h"
#include "uiobjects/uipicture.h"
#include "uiobjects/uiart.h"
#include "uiobjects/uibutton.h"
#include "uiobjects/uiprogressbar.h"
#include "uiobjects/uipopupmenu.h"
#include "uiobjects/uilistgroup.h"
#include "uiobjects/beans/listgroupcol.h"
#include "uiobjects/uilistcommon.h"
#include "uiobjects/UITreeListBox.h"
#include "uiobjects/common/BaseDefines.h"

const unsigned int ALPHABACKGROUND = 235;

class BaseFrontend : public Ioutil{
public:
    BaseFrontend();
    virtual ~BaseFrontend();
    bool drawMenu(tEvento );

protected:

    virtual void initUIObjs();
    virtual void setDinamicSizeObjects();
    virtual void cargaMenu(string, string, tEvento *);
    virtual bool procesarMenuActual(tmenu_gestor_objects *objMenu, tEvento *evento);

    struct tscreenobj{
        bool drawComponents;
        bool execFunctions;
    };

    typedef int (BaseFrontend::*typept2Func)(tEvento *); //Se declara el puntero a funcion

    struct tprops{
        string parms;
        typept2Func pt2Func;
    };

    tmenu_gestor_objects *createMenu(string menuName);
    tmenu_gestor_objects *getMenu(string menuName);

    void setSelMenu(string var);
    string getSelMenu();
    int getNumMenus(){
        return objectsMenu.size();
    }

    void addEvent(string, typept2Func);
    void addEvent(string, typept2Func, string);
    void setEvent(string, typept2Func);
    void setEvent(string, typept2Func, string);
    tprops * getEvent(string);
    bool procesarControles(tmenu_gestor_objects *, tEvento *, tscreenobj *);
    int casoDEFAULT(tEvento );
    int accionesMenu(tEvento *);
    bool procesarBoton(Object *, tmenu_gestor_objects *);
    void cargaMenuFromLista(UIListCommon *, tEvento *);

    void resizeMenu();
    void comprobarUnicode(string);
    int accionesGotoPantalla(tEvento *);
    string gotoPantalla(tEvento *evento);
    string casoPANTALLAPREGUNTA(string, string);
    bool casoPANTALLACONFIRMAR(string, string);
    int loadDirFromExplorer(tEvento *);
    int accionCombo(tEvento *evento);
    void loadComboUnidades(string objName, string pantalla,  int types);
    int accionesListaExplorador(tEvento *);
    void showMenuEmergente(string menu, string objImagenFondo);
    void iniciarExplorador(string lastDirOpened);
    string showExplorador(tEvento *);
    void setTextFromExplorador(tEvento *, UIInput *);
    UIPopupMenu * addPopup(string pantalla, string popupName, string callerName);
    bool procesarPopups(tmenu_gestor_objects *objMenu, tEvento *evento);
    bool waitAceptCancel(string btnAceptar, string btnCancelar, string pantalla);
    int accionCopiarTextoPopup(tEvento *evento);
    string casoJOYBUTTONS(tEvento *evento);
    //Eventos asignados a los botones
    int simularEscape(tEvento *);
    int simularIntro(tEvento *);
    int marcarBotonSeleccionado(tEvento *);
    void ComprobarPopupsCerrados(tmenu_gestor_objects *objMenu,tEvento *evento);
    int accionesCargaPantalla(tEvento *evento);


private:
    string selMenu;
    map<string, tprops*> propertiesPt2Func;
    map<string, tmenu_gestor_objects *> objectsMenu;
};

#endif /* BASEFRONTEND_H */

