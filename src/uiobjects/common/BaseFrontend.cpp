/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   BaseFrontend.cpp
 * Author: Ryuk
 *
 * Created on 29 de marzo de 2020, 12:40
 */

#include "BaseFrontend.h"

/**
 *
 */
BaseFrontend::BaseFrontend() {
    Traza::print("Iniciando BaseFrontend", W_INFO);
    initUIObjs();
}

/**
 *
 */
BaseFrontend::~BaseFrontend() {
    Traza::print("Destructor de BaseFrontend", W_INFO);
    //  A los objetos no los podemos eliminar porque no fueron creados dinamicamente
    Traza::print("Eliminando objetos de cada Menu", W_INFO);
    // Create a map iterator and point to beginning of map
    //std::map<std::string, tmenu_gestor_objects *>::iterator it = objectsMenu.begin();
    // Iterate over the map using Iterator till end.
    //for(it; it != objectsMenu.end(); it++){
    for (auto it = objectsMenu.begin(); it != objectsMenu.end(); ++it){
        delete it->second;
    }

    Traza::print("Fin Destructor de BaseFrontend", W_INFO);
}

/**
 *
 */
void BaseFrontend::initUIObjs(){
    tmenu_gestor_objects *obj  = createMenu(PANTALLABROWSER2);
    obj->add(uiimgFondo, GUIPICTURE, 0, Constant::getINPUTH(), 0, 0, uiimgFondo, true)->setEnabled(false);
    obj->getObjByName(uiimgFondo)->setAlpha(150);
    obj->add(OBJLISTABROWSER2, GUILISTIMG, 0, 0, 0, 0, uilistadodir, false)->setVerContenedor(false)->setShadow(false);
    obj->add(BTNACEPTARBROWSER, GUIBUTTON, -(BUTTONW/2 + 5), 0, BUTTONW,BUTTONH, "Aceptar", true)->setIcon(tick);
    obj->add(BTNCANCELARBROWSER, GUIBUTTON, (BUTTONW/2 + 5), 0, BUTTONW,BUTTONH, "Cancelar", true)->setIcon(cross);
    obj->add(ARTDIRBROWSER, GUIARTSURFACE, 0, 0, INPUTW, Constant::getINPUTH(), "Direcci%C3%B3n Browser", false)->setEnabled(false);
    obj->add(uicomboBrowser, GUICOMBOBOX, 0, 0, 0, 0, "", false);
    getMenu(PANTALLABROWSER2)->getObjByName("uiimgFondo")->setVisible(false);

    obj = createMenu(PANTALLAPREGUNTA);
    obj->add(uivalor, GUIINPUTWIDE, 0, -20 * zoomText, INPUTW, Constant::getINPUTH(), "Dato:", true);
    obj->add(btnAceptarPregunta, GUIBUTTON, -(BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Aceptar", true)->setIcon(tick);
    obj->add(btnCancelarPregunta, GUIBUTTON, (BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Cancelar", true)->setIcon(cross);
    obj->add(uiborde, GUIPANELBORDER,0,0,0,0, "Introduzca el dato", false);
    obj->getObjByName(uivalor)->setColor(cBlanco);

    obj = createMenu(PANTALLACONFIRMAR);
    obj->add(uiborde, GUIPANELBORDER,0,0,0,0, "Seleccione una opci%C3%B3n", false);
    obj->add(uitextosBox, GUITEXTELEMENTSAREA, 0, -50 * zoomText, getWidth()-50, 120, "", true)->setVerContenedor(false);
    obj->add(btnSiConfirma, GUIBUTTON, -(BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Aceptar", true)->setIcon(tick);
    obj->add(btnNoConfirma, GUIBUTTON, (BUTTONW/2 + 5), 30,BUTTONW,BUTTONH, "Cancelar", true)->setIcon(cross);

    UITextElementsArea *infoTextRom = (UITextElementsArea *)obj->getObjByName(uitextosBox);
    TextElement detalleElement;
    detalleElement.setName(uilabelDetalle);
    detalleElement.setUseMaxLabelMargin(true);
    infoTextRom->addField(&detalleElement);
    infoTextRom->setTextColor(cBlanco);
    infoTextRom->setColor(cNegro);

    //Botones para la pantalla de los directorios
    addEvent(BTNACEPTARBROWSER, &BaseFrontend::marcarBotonSeleccionado);
    addEvent(BTNCANCELARBROWSER, &BaseFrontend::marcarBotonSeleccionado);
    addEvent(OBJLISTABROWSER2, &BaseFrontend::accionesListaExplorador);
    addEvent(uicomboBrowser, &BaseFrontend::accionCombo);
    addEvent(btnAceptarPregunta, &BaseFrontend::simularIntro);
    addEvent(btnCancelarPregunta, &BaseFrontend::simularEscape);
    //Botones para la pantalla de confirmacion
    addEvent(btnSiConfirma, &BaseFrontend::marcarBotonSeleccionado);
    addEvent(btnNoConfirma, &BaseFrontend::marcarBotonSeleccionado);
}

/**
 *
 * @param menuName
 * @return
 */
tmenu_gestor_objects *BaseFrontend::createMenu(string menuName){
    tmenu_gestor_objects *menuObj = new tmenu_gestor_objects(this->getWidth(), this->getHeight());
    objectsMenu.insert(make_pair(menuName, menuObj));
    return menuObj;
}

/**
 *
 * @param menuName
 * @return
 */
tmenu_gestor_objects * BaseFrontend::getMenu(string menuName){
    std::map<std::string, tmenu_gestor_objects *>::iterator it = objectsMenu.find(menuName);
    if (it != objectsMenu.end()){
        return it->second;
    } else {
        Traza::print("Menu " + menuName + " not found", W_ERROR);
        return NULL;
    }
}

/**
 *
 * @param var
 */
void BaseFrontend::setSelMenu(string var){
    comprobarUnicode(var);
    selMenu = var;
}

/**
 *
 * @return
 */
string BaseFrontend::getSelMenu(){
    return selMenu;
}

/**
*
*/
void BaseFrontend::setDinamicSizeObjects(){
    try{
        //Calculamos el tamanyo del titulo de los elementos que lo tengan, y redimensionamos el elemento
        //lista que tenga ese menu con el total de la ventana que queda
//        std::map<std::string, tmenu_gestor_objects *>::iterator it = objectsMenu.begin();
//        for(it; it != objectsMenu.end(); it++){
        for (auto it = objectsMenu.begin(); it != objectsMenu.end(); ++it){
            try{
                Object *posibleObj = NULL;
                posibleObj = it->second->getObjByName(TITLESCREEN);
                //Caso especial de redimension
                if(posibleObj != NULL)
                    posibleObj->setTam( 0, 0, this->getWidth(),Constant::getINPUTH());

                int j = 0;

                while (j < it->second->getSize()){
                    posibleObj = it->second->getObjByPos(j);
                    if(posibleObj != NULL){
                        if (posibleObj->getObjectType() == GUILISTBOX || posibleObj->getObjectType() == GUIPICTURE
                            || posibleObj->getObjectType() == GUILISTGROUPBOX){
                            posibleObj->setTam(0,Constant::getINPUTH(), this->getWidth(), this->getHeight()-Constant::getINPUTH());
                        }

                        if (it->second->getObjByPos(j)->getObjectType() == GUILISTBOX){
                            ((UIList *)posibleObj)->calcularScrPos();
                        } else if (it->second->getObjByPos(j)->getObjectType() == GUILISTGROUPBOX){
                            ((UIListGroup *)posibleObj)->calcularScrPos();
                        }
                    }
                    j++;
                }
            } catch (Excepcion &e){}
        }

        //Redimension para el browser de directorios2
        getMenu(PANTALLABROWSER2)->getObjByName(OBJLISTABROWSER2)->setTam(0, Constant::getINPUTH() + COMBOHEIGHT + 4,this->getWidth(), this->getHeight() - BUTTONH - Constant::getINPUTH() - COMBOHEIGHT - 10 - 4);
        getMenu(PANTALLABROWSER2)->getObjByName(uicomboBrowser)->setTam(1, Constant::getINPUTH() + 4, 160, 100);
        getMenu(PANTALLABROWSER2)->getObjByName(BTNACEPTARBROWSER)->setTam( (this->getWidth() / 2) -(BUTTONW + 5), this->getHeight() - BUTTONH - 5, BUTTONW,BUTTONH);
        getMenu(PANTALLABROWSER2)->getObjByName(BTNCANCELARBROWSER)->setTam( (this->getWidth() / 2) + 5, this->getHeight() - BUTTONH - 5, BUTTONW,BUTTONH);
        getMenu(PANTALLABROWSER2)->getObjByName(ARTDIRBROWSER)->setTam( 0, 0, this->getWidth(), Constant::getINPUTH());
    } catch (Excepcion &e){
        Traza::print("setDinamicSizeObjects: " + string(e.getMessage()), W_ERROR);
    }
}

/**
* Se asigna la funcion especificada al boton con el nombre especificado.
* No deberian haber botones del mismo nombre
*/
void BaseFrontend::addEvent(string nombre, typept2Func funcion){
    addEvent(nombre, funcion, "");
}

/**
*
*/
void BaseFrontend::addEvent(string nombre, typept2Func funcion, string parms){
        tprops *props = new tprops;
        props->parms = parms;
        props->pt2Func = funcion;
        propertiesPt2Func.insert(make_pair(nombre, props));
}

/**
* Se busca la posicion del puntero a funcion asociado con el nombre del boton y se modifica
*/
void BaseFrontend::setEvent(string nombre, typept2Func funcion){
    setEvent(nombre, funcion, "");
}

void BaseFrontend::setEvent(string nombre, typept2Func funcion, string parms){
    tprops *props = getEvent(nombre);
    if (props != NULL){
        propertiesPt2Func.at(nombre)->parms = parms;
        propertiesPt2Func.at(nombre)->pt2Func = funcion;
    }
}

/**
 *
 * @param eventName
 * @return
 */
BaseFrontend::tprops * BaseFrontend::getEvent(string eventName){
    std::map<string, BaseFrontend::tprops *>::iterator it;
    it = propertiesPt2Func.find(eventName);
    if (it != propertiesPt2Func.end()){
        return it->second;
    } else
        return NULL;
}

/**
* Con el menu pasado por parametro lo dibujamos entero
*/
bool BaseFrontend::drawMenu(tEvento evento){
    Traza::print("BaseFrontend::drawMenu Inicio", W_PARANOIC);
    bool salir = false;
    this->clearScr(Constant::getBACKGROUNDCOLOR());
    Traza::print("BaseFrontend::clearScr Fin", W_PARANOIC);
    //Realiza las acciones de cada elemento de pantalla
    salir = casoDEFAULT(evento);
    //Muestra un mensaje durante un tiempo determinado
    showAutoMessage();
    //Muestra el contador de cuadros por pantalla
    if (SHOWFPS) fps();
    Traza::print("BaseFrontend::drawMenu Fin", W_PARANOIC);
    this->flipScr();
    return salir;
}

/**
*
*/
int BaseFrontend::casoDEFAULT(tEvento evento){
    tmenu_gestor_objects *objMenu = getMenu(this->getSelMenu());
    bool salir = procesarControles(objMenu, &evento, NULL);
    return salir;
}



/**
* Se encarga de procesar las introducciones de teclas o de joystick para cambiar el contenido
* de los botones, inputs, checks, ... y almacenar en ellos los datos correpondientes.
*/
bool BaseFrontend::procesarControles(tmenu_gestor_objects *objMenu, tEvento *evento, tscreenobj *screenEvents){
    Traza::print("procesarControles: Inicio", W_PARANOIC);
    bool execFunc = true;
    bool drawComp = true;
    if (screenEvents != NULL){
        execFunc = screenEvents->execFunctions;
        drawComp = screenEvents->drawComponents;
    }

    //Se procesan los eventos de cada uno de los objetos de un menu
    Object *object;

    if (execFunc){
         //Procesando el redimensionado de ventana
        if (evento->resize)
            resizeMenu();
        //Se llama al action del objeto que esta seleccionado en este momento.
        //Cada objeto tiene el suyo propio o heredado de Object
        objMenu->procEvent(*evento);
        //objMenu->procAllEvent(*evento);
        //Mostramos popups si es necesario
        procesarPopups(objMenu, evento);
        //Hacemos llamada para hacer otros procesados. Por ahora solo en la pantalla de seleccion
        //de roms
        procesarMenuActual(objMenu, evento);
    }

//    int posBoton = 0;
    bool salir = false;
    bool botonPulsado = false;
    int estado = 0;
    vector<Object *> objPostProcesado;
    int cursorPrincipal = -1;
    bool updateCursor = false;
    tprops *function;

    //Recorremos todos los objetos para dibujarlos por pantalla
    try{


        //PINTAMOS ANTES DE PROCESAR LAS ACCIONES. NO SE SI ESTO ES BUENA IDEA
        for (int i=0;i<objMenu->getSize();i++){
            object = objMenu->getObjByPos(i);
            //Finalmente dibujamos el objeto
            if (drawComp && object != NULL){
                if (object->getObjectType() == GUICOMBOBOX){
                    objPostProcesado.push_back(object);
                } else {
                    drawObject(object, evento);
                }
            }
        }
        //Para los objetos que son prioritarios de pintar, lo hacemos en ultimo lugar
        //para que se dibujen sobre el resto
        for(vector<Object *>::iterator it = objPostProcesado.begin(); it < objPostProcesado.end(); ++it){
            Object * obj = *it;
            drawObject(obj, evento);
        }
        objPostProcesado.clear();

        //Procesamos las acciones
        for (int i=0;i<objMenu->getSize();i++){
            object = objMenu->getObjByPos(i);

            if (execFunc && object != NULL){
                if (!object->isPopup()){
                    switch(object->getObjectType()){
                        case GUIBUTTON:
                            //En el caso de los botones tambien procesamos sus eventos
                            botonPulsado = procesarBoton(object, objMenu);
                            estado = evento->mouse_state;
                            if (botonPulsado && ( (evento->isMouse && estado == SDL_RELEASED) || evento->isKey || evento->isJoy)){ //Comprobamos si se ha pulsado el elemento
                                function = getEvent(object->getName());
                                if (function != NULL){ //Si hemos encontrado una funcion
                                    //Forzamos a que se actualicen todos los elementos
                                    objMenu->resetElements();
                                    /**Los botones no pueden hacer que se salga de la aplicacion. Solo ejecutamos la funcion*/
                                    //salir = (*this.*pt2Func[posBoton])(evento); //Ejecutamos la funcion especificada en el puntero a funcion almacenado
                                    (*this.*function->pt2Func)(evento); //Ejecutamos la funcion especificada en el puntero a funcion almacenado
                                    Traza::print("procesarControles: Evento lanzado para " + object->getName(), W_INFO);
                                }
                            }
                            break;
                        case GUILISTBOX:
                        case GUIPROGRESSBAR:
                        case GUISLIDER:
                        case GUIPOPUPMENU:
                        case GUILISTGROUPBOX:
                        case GUICOMBOBOX:
                        case GUITEXTELEMENTSAREA:
                        case GUITREELISTBOX:
                        case GUIPICTURE:
                        case GUILISTIMG:
                            if (procesarBoton(object, objMenu)){ //Comprobamos si se ha pulsado el elemento
                                function = getEvent(object->getName());
                                if (function != NULL){ //Si hemos encontrado una funcion
                                    //Forzamos a que se actualicen todos los elementos
                                    objMenu->resetElements();
                                    salir = (*this.*function->pt2Func)(evento); //Ejecutamos la funcion especificada en el puntero a funcion almacenado
                                    Traza::print("procesarControles: Evento lanzado para " + object->getName(), W_INFO);
                                }
                            }
                            break;
                        default:
                            break;
                    } // FIN CASE
                }
            } // FIN IF
//            //Finalmente dibujamos el objeto
            /**Aqui iba el dibujado del objeto para aprovechar el bucle pero esto
            * daba problemas. Se corrige para que tenga su propio bucle al inicio
            * de las acciones
            */
            //Comprobamos si el objeto esta lanzando un evento para cambiar el mouse.
            //Solo aceptamos el primer objeto que pida cambiar el mouse
            if (object->getCursor() >= 0){
                updateCursor = true;
                if (cursorPrincipal < 0){
                    cursorPrincipal = object->getCursor();
                }
            }
        }
        if (!updateCursor){
            //Reseteamos el cursor al que hay por defecto
            cursorPrincipal = cursor_arrow;
        }
        //Dibujamos el cursor solo si procede
        this->pintarCursor(evento->mouse_x, evento->mouse_y, cursorPrincipal);
        //Comprobar si al cerrar
        this->ComprobarPopupsCerrados(objMenu, evento);

    } catch (Excepcion &e) {
         Traza::print("Excepcion procesarControles: " + string(e.getMessage()), W_ERROR);
    }
    return salir;
}

/**
 *
 * @param objMenu
 * @param evento
 * @return
 */
bool BaseFrontend::procesarMenuActual(tmenu_gestor_objects *objMenu, tEvento *evento){
    return true;
}

/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::accionesMenu(tEvento *evento){
     bool salir = false;

    try{
        tmenu_gestor_objects *objsMenu = getMenu(this->getSelMenu());
        Object *object = objsMenu->getObjByPos(objsMenu->getFocus());

        if (object != NULL){
            if (object->getObjectType() == GUILISTBOX ||
                object->getObjectType() == GUIPOPUPMENU ||
                object->getObjectType() == GUILISTGROUPBOX){

                UIListCommon *objList = (UIListCommon *)object;
                unsigned int pos = objList->getPosActualLista();
                string valorSelec = objList->getValue(pos);
                string destino = objList->getDestino(pos);
                objList->setImgDrawed(false);

                if (valorSelec.compare("salir") == 0){
                    salir = true;
                } else if (valorSelec.compare("shutdown") == 0 or valorSelec.compare("reboot") == 0){
                    Launcher *launcher = new Launcher();
                    FileLaunch *emulInfo = new FileLaunch();
                    string option = "";

                    #ifdef UNIX
                        if (valorSelec.compare("reboot") == 0){
                            option = "-r";
                        } else if(valorSelec.compare("shutdown") == 0){
                            option = "-h";
                        }

                        emulInfo->fileexe = "sudo shutdown " +  option + " now";
                        emulInfo->parmsexe = "";
                    #elif WIN
                        if (valorSelec.compare("reboot") == 0){
                            option = "-r";
                        } else if(valorSelec.compare("shutdown") == 0){
                            option = "-s";
                        }

                        flipScr();
                        emulInfo->rutaexe = Constant::TipoToStr(getenv("windir")) + "\\system32";
                        emulInfo->fileexe = "shutdown.exe";
                        emulInfo->parmsexe = option + " -f -t 1";
                    #endif

                    string pregunta = "";
                    if (valorSelec.compare("reboot") == 0){
                        pregunta = "%C2%BFEsta seguro de que desea reiniciar la maquina?";
                    } else if(valorSelec.compare("shutdown") == 0){
                        pregunta = "%C2%BFEsta seguro de que desea apagar la maquina?";
                    }

                    bool confirmed = casoPANTALLACONFIRMAR("Confirmacion", pregunta);

                    if (confirmed){
                        if (valorSelec.compare("reboot") == 0){
                            showMessage("El sistema se va a reiniciar. Espere un momento...", 1);
                        } else if(valorSelec.compare("shutdown") == 0){
                            showMessage("El sistema se va a apagar. Espere un momento...", 1);
                        }
                        Traza::print("Antes de ejecutar programa", W_DEBUG);
                        launcher->lanzarProgramaUNIXFork(emulInfo);
                        salir = true;
                    }

                    delete emulInfo;
                    delete launcher;

                } else if (!destino.empty()){
                    this->cargaMenuFromLista(objList, evento);
                }
            }
        }
    } catch (Excepcion &e) {
         Traza::print("Excepcion accionesMenu" + string(e.getMessage()), W_ERROR);
    }
    return salir;
}

/**
* Con el menu pasado por parametro lo dibujamos entero
*/
void BaseFrontend::resizeMenu(){
    for (auto it = objectsMenu.begin(); it != objectsMenu.end(); ++it){
        it->second->setAreaObjMenu(getWidth(),getHeight());
    }
    setDinamicSizeObjects();
}

/**
* El campo checked del control se da uivalor desde la llamada al procEvent del respectivo menu
* En esta funcion se comprueba el uivalor checked para saber si se ha pulsado el elemento
*/
bool BaseFrontend::procesarBoton(Object * obj, tmenu_gestor_objects *gestorMenu){
    try{
        //Traza::print("Checkeando name: " + string(name) + " uivalor: " + string(objMenu->getObjByName(name)->isChecked()?"S":"N"), W_ERROR);
        if (obj->getObjectType() == GUICOMBOBOX){
            if (((UIComboBox *)obj)->isValueChanged()){
                ((UIComboBox *)obj)->setValueChanged(false);
                ((UIComboBox *)obj)->setChecked(false);
                ((UIComboBox *)obj)->setFocus(false);
                gestorMenu->findNextFocus();
                return true;
            }
        } else if (obj->isChecked()){
            obj->setChecked(false);
            return true;
        }
    } catch (Excepcion &e) {
         Traza::print("Excepcion procesarBoton" + string(e.getMessage()), W_ERROR);
    }
    return false;
}

/**
 *
 * @param obj
 * @param evento
 */
void BaseFrontend::cargaMenuFromLista(UIListCommon *obj, tEvento *evento){

    if (obj->getPosActualLista() < 0){
        obj->setPosActualLista(0);
    } else {
        string menucarga = obj->getDestino(obj->getPosActualLista());
        string valorSelec = obj->getValue(obj->getPosActualLista());
        //Si hemos pulsado el boton de volver (que es el ultimo de la lista)
        //reiniciamos la posicion por si volvemos a entrar
        if (obj->getPosActualLista() >= obj->getSize() - 1){
            obj->setPosActualLista(0);
            obj->calcularScrPos();
        }
        //Ahora cargamos el siguiente menu
        cargaMenu(menucarga, valorSelec, evento);
    }
}

/**
*
*/
void BaseFrontend::cargaMenu(string menucarga, string valorSelec, tEvento *evento){
    comprobarUnicode(menucarga);
    tmenu_gestor_objects *objsMenu = getMenu(menucarga);
    if (objsMenu != NULL){
        this->setSelMenu(menucarga);
        //Damos el foco al primer elemento que haya en el menu
        this->getMenu(menucarga)->setFirstFocus();
    }
}

/**
* En este metodo se comprueba si tenemos que activar unicode para que
* las teclas del teclado se traduzcan sin problemas de dependencia de layouts
*/
void BaseFrontend::comprobarUnicode(string menu){

    tmenu_gestor_objects *objsMenu = getMenu(menu);
    if (objsMenu != NULL){
        int i=0;
        bool found = false;
        while (i < objsMenu->getSize() && !found){
            if (objsMenu->getObjByPos(i) != NULL)
                if (objsMenu->getObjByPos(i)->getObjectType() == GUIINPUTWIDE)
                    found = true;
            i++;
        }
        Traza::print("comprobarUnicode: " + Constant::TipoToStr(menu) + ((found == true) ? " UNICODE=S":" UNICODE=N"), W_PARANOIC);
        SDL_EnableUNICODE(found);
    }
}

/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::accionesGotoPantalla(tEvento *evento){
    return !gotoPantalla(evento).empty();
}

string BaseFrontend::gotoPantalla(tEvento *evento){
    tmenu_gestor_objects *objsMenu = getMenu(this->getSelMenu());
    if (objsMenu == NULL)
        return "";

    Object *object = objsMenu->getObjByPos(objsMenu->getFocus());
    tprops * event = getEvent(object->getName());

    if (event != NULL){ //Si hemos encontrado una funcion
        string menuToLoad = event->parms;
        if (!menuToLoad.empty()){
            this->setSelMenu(menuToLoad);
            this->getMenu(menuToLoad)->findNextFocus();
            comprobarUnicode(menuToLoad);
            return menuToLoad;
        }
    }
    return "";
}

/**
*
*/
string BaseFrontend::casoPANTALLAPREGUNTA(string titulo, string label){
    ignoreButtonRepeats = true;
    Traza::print("casoPANTALLAPREGUNTA: Inicio", W_INFO);
    bool salir = false;
    tEvento askEvento;
    clearEvento(&askEvento);
    string salida = "";
    string menuAnt = getSelMenu();
    setSelMenu(PANTALLAPREGUNTA);
    titulo = Constant::txtDisplay(titulo);
    label = Constant::txtDisplay(label);

    tmenu_gestor_objects *objMenu = getMenu(PANTALLAPREGUNTA);
    objMenu->getObjByName(uivalor)->setLabel(label);
    ((UIInput *)objMenu->getObjByName(uivalor))->setText("");
    objMenu->getObjByName(uiborde)->setLabel(titulo);


    long delay = 0;
    unsigned long before = 0;

    do{
        before = SDL_GetTicks();
        askEvento = WaitForKey();
        clearScr(cBgScreen);
        procesarControles(objMenu, &askEvento, NULL);

        flipScr();
        salir = (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)) ||
        (askEvento.isKey && askEvento.key == SDLK_ESCAPE);

        UIInput *input = (UIInput *)objMenu->getObjByName(uivalor);

        if (input->getSize() > 0 &&
            ( (askEvento.isKey && askEvento.key == SDLK_RETURN)
              || (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_A))) ){
            salida = input->getText();
            salir = true;
        }

        delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
        if(delay > 0) SDL_Delay(delay);
    } while (!salir);
    setSelMenu(menuAnt);

    return salida;
}

/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::accionesCargaPantalla(tEvento *evento){
    string posMenu = gotoPantalla(evento);
    if (!posMenu.empty()){
        cargaMenu(posMenu, "", evento);
        return true;
    }
    return false;
}

/**
*
*/
bool BaseFrontend::casoPANTALLACONFIRMAR(string titulo, string txtDetalle){
    ignoreButtonRepeats = true;
    Traza::print("casoPANTALLACONFIRMAR: Inicio", W_INFO);
    bool salir = false;
    tEvento askEvento;
    clearEvento(&askEvento);
    bool salida = false;
    string menuInicial = getSelMenu();
    titulo = Constant::txtDisplay(titulo);
    txtDetalle = Constant::txtDisplay(txtDetalle);

    //Procesamos el menu antes de continuar para que obtengamos la captura
    //de pantalla que usaremos de fondo
    procesarControles(getMenu(menuInicial), &askEvento, NULL);
    SDL_Rect iconRectFondo = {0, 0, (Uint16)this->getWidth(), (Uint16)this->getHeight()};
    SDL_Surface *mySurface = NULL;
    drawRectAlpha(iconRectFondo.x, iconRectFondo.y, iconRectFondo.w, iconRectFondo.h , cNegro, ALPHABACKGROUND);
    takeScreenShot(&mySurface, iconRectFondo);

    //Seguidamente cambiamos la pantalla a la de la confirmacion
    setSelMenu(PANTALLACONFIRMAR);
    tmenu_gestor_objects *objMenu = getMenu(PANTALLACONFIRMAR);
    objMenu->getObjByName(uiborde)->setLabel(titulo);

    UITextElementsArea *textElems = (UITextElementsArea *)objMenu->getObjByName(uitextosBox);
    textElems->setImgDrawed(false);
    textElems->setFieldText(uilabelDetalle, txtDetalle);

    int len = fontStrLen(txtDetalle);
    if (len < this->getWidth()){
        textElems->setW(len + 4 * INPUTBORDER + TEXLABELTEXTSPACE);
        textElems->setH(Constant::getMENUSPACE() + 2*INPUTBORDER);
        objMenu->centrarObjeto(textElems);
//        textElems->setX(this->getWidth() - len/2);
    } else {
        textElems->setTam(0, -50 * zoomText, getWidth()-50, 120);
        objMenu->centrarObjeto(textElems);
    }

    long delay = 0;
    unsigned long before = 0;
    objMenu->setFocus(0);

    do{
        before = SDL_GetTicks();
        askEvento = WaitForKey();
//        clearScr(cBgScreen);
        printScreenShot(&mySurface, iconRectFondo);

        procesarControles(objMenu, &askEvento, NULL);

        flipScr();
        salir = (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)) ||
        (askEvento.isKey && askEvento.key == SDLK_ESCAPE);

        if (objMenu->getObjByName(btnSiConfirma)->getTag().compare("selected") == 0){
            salir = true;
            salida = true;
            objMenu->getObjByName(btnSiConfirma)->setTag("");
            Traza::print("Detectado SI pulsado", W_DEBUG);
        } else if (objMenu->getObjByName(btnNoConfirma)->getTag().compare("selected") == 0){
            salir = true;
            salida = false;
            objMenu->getObjByName(btnNoConfirma)->setTag("");
            Traza::print("Detectado NO pulsado", W_DEBUG);
        }

        delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
        if(delay > 0) SDL_Delay(delay);
    } while (!salir);

    setSelMenu(menuInicial);
    return salida;
}

/**
* Muestra el menu pasado por parametro como si fuera emergente, dejando el menu anterior
* difuminado sobre el fondo
*/
void BaseFrontend::showMenuEmergente(string menu, string objImagenFondo){
    try{
         Traza::print("BaseFrontend::showMenuEmergente", W_INFO);
        //Procesamos el menu actual para que se vuelva a repintar
        procesarControles(getMenu(getSelMenu()), new tEvento(), NULL);
        //Seleccionamos el menu que queremos mostrar como si fuese emergente
        tmenu_gestor_objects *objMenu = getMenu(menu);
        //Realizamos una captura de pantalla en el objeto imagen de fondo del menu de destino
        UIPicture * fondoImg = (UIPicture *) objMenu->getObjByName(objImagenFondo);
        //Le damos un toque mas oscuro a la imagen
        fondoImg->setAlpha(150);
        //Obtenemos una captura de la pantalla entera
        takeScreenShot(&fondoImg->getImgGestor()->surface);
        //Seleccionamos el menu de destino y procesamos sus controles para que se pinten
        setSelMenu(menu);
        tEvento askEvento;
        clearEvento(&askEvento);
        procesarControles(objMenu, &askEvento, NULL);
    } catch (Excepcion &e) {
         Traza::print("Excepcion showMenuEmergente: " + string(e.getMessage()), W_ERROR);
    }
}

/**
 *
 * @param lastDirOpened
 * @return
 */
void BaseFrontend::iniciarExplorador(string lastDirOpened){
    Dirutil dir;
    this->setSelMenu(PANTALLABROWSER2);
    tmenu_gestor_objects *objMenu = getMenu(PANTALLABROWSER2);
    UIListGroup *obj = NULL;

    loadComboUnidades(uicomboBrowser, PANTALLABROWSER2, -1);
    obj = (UIListGroup *)objMenu->getObjByName(OBJLISTABROWSER2);
    obj->setFocus(true);
    obj->setTag("");
    obj->setPosActualLista(-1);
    //Forzamos a que se actualicen todos los elementos
    objMenu->resetElements();
    //Seleccionamos a la lista que esta en primer lugar
    //objMenu->findNextFocus();
    getMenu(PANTALLABROWSER2)->setFocus(OBJLISTABROWSER2);
    vector <ListGroupCol *> miCabecera;
    miCabecera.push_back(new ListGroupCol("Filename", "")); //Cancion
    miCabecera.push_back(new ListGroupCol("Date", ""));
    miCabecera.push_back(new ListGroupCol("Type", "")); //Album
    miCabecera.push_back(new ListGroupCol("Size", "")); //Duracion
    obj->setHeaderLista(miCabecera);
    obj->adjustToHeader(false);
    obj->addHeaderWith(100);
    obj->addHeaderWith(100);
    obj->addHeaderWith(100);
    obj->addHeaderWith(100);
    obj->setColor(cBlanco);
    obj->setTextColor(cNegro);

    if (!lastDirOpened.empty()){
        dir.changeDirAbsolute(dir.getFolder(lastDirOpened).c_str());
    }
    this->accionesListaExplorador(NULL);
}


/**
*
*/
string BaseFrontend::showExplorador(tEvento *evento){
    Traza::print("showExplorador: Inicio", W_INFO);
    bool salir = false;
    tEvento askEvento;
    clearEvento(&askEvento);
    string menuInicio = this->getSelMenu();
    this->setSelMenu(PANTALLABROWSER2);
    tmenu_gestor_objects *objMenu = getMenu(PANTALLABROWSER2);
    UIListGroup *obj = NULL;
    ignoreButtonRepeats = true;
    string fileUri = "";
    string fileTempSelec = "";
    Dirutil dir;
    static string lastDirOpened;

    try{
        iniciarExplorador(lastDirOpened);

        obj = (UIListGroup *)objMenu->getObjByName(OBJLISTABROWSER2);
        long delay = 0;
        unsigned long before = 0;

        do{
            before = SDL_GetTicks();
            askEvento = WaitForKey();
            clearScr(cBlanco);
            int pos = obj->getPosActualLista();
            //Carga de imagenes de fondo en la pantalla del explorador de ficheros
            if (pos >= 0){
                fileTempSelec = obj->getText(pos);
                string ruta = dir.getDirActual() +  Constant::getFileSep() + fileTempSelec;
                UIPicture *objPict = (UIPicture *)objMenu->getObjByName(uiimgFondo);

                if (objPict->getImgGestor()->getRuta().compare(ruta) != 0){
                    if (dir.findIcon(fileTempSelec.c_str()) == page_white_picture){
                        objPict->loadImgFromFile(ruta);
                        objPict->getImgGestor()->setBestfit(false);
                    } else {
                        if (objPict->getImgGestor()->clearFile())
                            objPict->setImgDrawed(false);
                    }
                    obj->setImgDrawed(false);
                    objMenu->getObjByName(BTNACEPTARBROWSER)->setImgDrawed(false);
                    objMenu->getObjByName(BTNCANCELARBROWSER)->setImgDrawed(false);
                    objPict->getImgGestor()->setRuta(ruta);
                }
            }

            procesarControles(objMenu, &askEvento, NULL);
            if (objMenu->getFocus() < 0){
                objMenu->findNextFocus();
            }

            //fps();
            flipScr();
            salir = (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)) ||
            ( ((askEvento.isKey && askEvento.key == SDLK_ESCAPE) || !obj->getTag().empty())
             || objMenu->getObjByName(BTNACEPTARBROWSER)->getTag().compare("selected") == 0
             || objMenu->getObjByName(BTNCANCELARBROWSER)->getTag().compare("selected") == 0)
             || askEvento.quit;


            delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
            if(delay > 0) SDL_Delay(delay);
        } while (!salir);

    } catch (Excepcion &e){
        Traza::print("Error en showExplorador: " + string(e.getMessage()), W_ERROR);
    }

    this->setSelMenu(menuInicio);
    string fileSelec;
    string diractual = dir.getDirActual();

    if (obj != NULL){
        // En el caso de que se haya pulsado el boton aceptar, obtenemos el elemento seleccionado
        if (objMenu->getObjByName(BTNACEPTARBROWSER)->getTag().compare("selected") == 0){
            int pos = obj->getPosActualLista();
            if (pos >= 0){
                fileSelec = obj->getText(pos);
                bool tieneFileSep = diractual.substr(diractual.length()-1).compare(Constant::getFileSep()) == 0;
                obj->setTag(diractual + (!tieneFileSep ? Constant::getFileSep() : "") + fileSelec);
            }
        }
        fileUri = obj->getTag();
    }

    try{
        objMenu->getObjByName(BTNCANCELARBROWSER)->setTag("");
        objMenu->getObjByName(BTNACEPTARBROWSER)->setTag("");
    } catch (Excepcion &e){
        Traza::print("showExplorador: reseteando botones: " + string(e.getMessage()), W_ERROR);
    }

    //No queremos que se seleccionen directorios incorrectos
    if (fileSelec.compare("..") == 0){
//        obj->setTag("");
//        showMessage("Directorio no valido.", 2000);
//        fileUri = showExplorador(evento);
        fileUri = fileUri.substr(0, fileUri.find_last_of(tempFileSep));
    }
    lastDirOpened = fileUri;
    return fileUri;
}

/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::accionesListaExplorador(tEvento *evento){
    try{
        Traza::print("BaseFrontend::accionesListaExplorador", W_INFO);
        tmenu_gestor_objects *objMenu = getMenu(PANTALLABROWSER2);
        UIListGroup * obj = (UIListGroup *)objMenu->getObjByName(OBJLISTABROWSER2);
        Dirutil dir;
        bool dirChanged = false;
        string diractual;
        int pos = obj->getPosActualLista();
        obj->setTag("");

        if (evento == NULL){
            pos = 0;
            dirChanged = true;
            obj->setPosActualLista(0);
            obj->calcularScrPos();
        } else if (pos >= 0){
            string fileSelec = obj->getText(pos);
            string extSelect = obj->getCol(pos, 2)->getValor();
            Traza::print("cambiando al directorio: " + fileSelec, W_DEBUG);
            if (extSelect.compare(STR_DIR_EXT) == 0){
                dirChanged = dir.changeDirRelative(fileSelec.c_str());
            } else {
                diractual = dir.getDirActual();
                obj->setTag(diractual + tempFileSep + fileSelec);
            }
        }

        if (dirChanged || pos < 0){
            ((UIArt *)objMenu->getObjByName(ARTDIRBROWSER))->setLabel(diractual);
             //Reseteamos la posicion del explorador para el siguiente directorio
            obj->setPosActualLista(0);
            obj->calcularScrPos();
            //Actualizamos la barra principal con la ruta actual
            diractual = dir.getDirActual();
            //Obtenemos el directorio actual y sus elementos
            listaSimple<FileProps> *filelist = new listaSimple<FileProps>();
            unsigned int numFiles = dir.listarDir(diractual.c_str(), filelist);
            Traza::print("Ficheros: " + Constant::TipoToStr(numFiles), W_DEBUG);

            //To refresh the images of the list
            if (obj->getObjectType() == GUILISTIMG){
                UIImgList * objImg = (UIImgList *)obj;
                objImg->setReloadImages(true);
                objImg->clearPrevImageCache(true);
                objImg->setLastIni(0);
                objImg->setLastEnd(0);
            }

            if (filelist != NULL && numFiles > 0){
                //Hacemos espacio en la lista para que la asignacion sea rapida
                obj->resizeLista(numFiles);
                //Recorremos la lista de ficheros y lo incluimos en el objeto de lista para mostrar los datos
                for (unsigned int i = 0; i < numFiles; i++){
                    //obj->addElemLista(filelist->get(i).filename , Constant::TipoToStr(filelist->get(i).filetype), filelist->get(i).ico );
                    ListGroupElement *listGroupElement = new ListGroupElement();
                    vector <ListGroupCol *> miFila;

                    ListGroupCol *colFileName = new ListGroupCol(filelist->get(i).filename, filelist->get(i).filename);
                    colFileName->setIcono(filelist->get(i).ico);
                    miFila.push_back(colFileName);

                    ListGroupCol *colFecha = new ListGroupCol(filelist->get(i).modificationTime, Constant::TipoToStr(filelist->get(i).iModificationTime));
                    colFecha->setNumber(true);
                    miFila.push_back(colFecha);

                    miFila.push_back(new ListGroupCol(filelist->get(i).extension, filelist->get(i).extension));

                    string strSize = Constant::printBytesSize(filelist->get(i).fileSize, 1);
                    ListGroupCol *colSize = new ListGroupCol(strSize, Constant::TipoToStr(filelist->get(i).fileSize));
                    colSize->setNumber(true);
                    miFila.push_back(colSize);

                    listGroupElement->SetListGroupCol(miFila);
                    listGroupElement->SetUipicture(NULL);
                    obj->addElemLista(listGroupElement);
                }
            } else {
               obj->resizeLista(1);
                ListGroupElement *listGroupElement = new ListGroupElement();
                vector <ListGroupCol *> miFila;
                miFila.push_back(new ListGroupCol("..", ".."));
                miFila.push_back(new ListGroupCol("", ""));
                miFila.push_back(new ListGroupCol(STR_DIR_EXT, STR_DIR_EXT));
                miFila.push_back(new ListGroupCol("0", "0"));
                listGroupElement->SetListGroupCol(miFila);
                listGroupElement->SetUipicture(NULL);
                obj->addElemLista(listGroupElement);
            }
            delete filelist;


        }


    }catch (Excepcion &e){
        Traza::print("accionesListaExplorador: " + string (e.getMessage()), W_ERROR);
    }

    getMenu(PANTALLABROWSER2)->setFocus(OBJLISTABROWSER2);
    return 0;
}

/**
 *
 * @param objName
 * @param pantalla
 * @param types
 */
void BaseFrontend::loadComboUnidades(string objName, string pantalla, int types){
    Traza::print("BaseFrontend::loadComboUnidades", W_INFO);
    UIList *combo = (UIList *)getMenu(pantalla)->getObjByName(objName);
    combo->clearLista();
    combo->setPosActualLista(0);
    vector<t_drive *> drives;

    Dirutil dir;
    dir.getDrives(&drives);
    int actualDrive = 0;
    string actualDir = dir.getDirActual();

    for (unsigned int i=0; i < drives.size(); i++){
        if (types == -1 || types == drives.at(i)->driveType){
            combo->addElemLista(drives.at(i)->drive.substr(0,2)
                                + " (" + drives.at(i)->driveTypeString + ") "
                                + drives.at(i)->label, drives.at(i)->drive, drives.at(i)->ico);
            if (actualDir.find(drives.at(i)->drive) != string::npos){
                actualDrive = i;
            }
        }
    }
    combo->setPosActualLista(actualDrive);
    combo->calcularScrPos();
}

/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::accionCombo(tEvento *evento){
    Traza::print("BaseFrontend::accionCombo", W_INFO);
    UIComboBox *combo = (UIComboBox *)getMenu(PANTALLABROWSER2)->getObjByName(uicomboBrowser);
    string unidad = combo->getValue(combo->getPosActualLista());
    Traza::print("BaseFrontend::accionCombo. Drive: " + unidad, W_DEBUG);
    Dirutil dir;
    bool cambioDir = dir.changeDirAbsolute(unidad.c_str());

    if (cambioDir){
        Traza::print("BaseFrontend::accionCombo. EXITO Drive: " + unidad, W_DEBUG);
    } else {
        Traza::print("BaseFrontend::accionCombo. ERROR Drive: " + unidad, W_ERROR);
    }

    clearEvento(evento);
    this->accionesListaExplorador(NULL);
    getMenu(PANTALLABROWSER2)->setFocus(OBJLISTABROWSER2);
    return 0;
}

/**
* Carga el resultado de la seleccion del explorador de archivos en un campo de texto
* - El objeto boton que llama a este metodo debe tener el campo tag rellenado con el campo de destino
*   En el que se quiere que se guarde el resultado
*/
int BaseFrontend::loadDirFromExplorer(tEvento *evento){

    try{
        Traza::print("BaseFrontend::loadDirFromExplorer", W_INFO);
        //Obtenemos los objetos del menu actual
        tmenu_gestor_objects *objMenu = getMenu(this->getSelMenu());
        //Obtenemos el objeto que ha sido seleccionado y que tiene el foco
        Object *obj = objMenu->getObjByPos(objMenu->getFocus());
        //Obtenemos el tag del elemento que indica en que campo deberemos dar uivalor a la seleccion
        //que hagamos del explorador de archivos
        string tag = obj->getTag();
        if (!tag.empty()){
            setTextFromExplorador(evento, (UIInput *)objMenu->getObjByName(tag));
        } else {
            Traza::print("loadDirFromExplorer. El objeto: " + obj->getLabel() + " no tiene tag asociado " +
                         "que indique donde cargar el texto del explorador", W_ERROR);
        }
    } catch (Excepcion &e){
        Traza::print("loadDirFromExplorer: " + string(e.getMessage()), W_ERROR);
    }
    return 0;
}

/**
 *
 * @param evento
 * @param objCampoEdit
 */
void BaseFrontend::setTextFromExplorador(tEvento *evento, UIInput *objCampoEdit){
    try{
        Traza::print("BaseFrontend::setTextFromExplorador", W_INFO);
        Dirutil dir;
        tmenu_gestor_objects *objMenu = getMenu(this->getSelMenu());
        //Si el objeto ya tiene datos, comprobamos si existe el directorio que supuestamente contiene
        string uri = dir.getFolder(objCampoEdit->getText());
        if (dir.existe(uri)){
            //Si resulta que existe, hacemos un cambio de directorio para que se muestre
            //el contenido del directorio nada mas pulsar en el boton
            dir.changeDirAbsolute(uri.c_str());
        }
        //Abrimos el explorador de archivos y esperamos a que el usuario seleccione un fichero
        //o directorio
        string fichName = showExplorador(evento);
        //Si se ha seleccionado algo, establecemos el texto en el objeto que hemos recibido por parametro
        if (!fichName.empty()){
            objCampoEdit->setText(fichName);
            objMenu->setFocus(objCampoEdit->getName());
        }
    } catch (Excepcion &e){
        Traza::print("setTextFromExplorador: " + string(e.getMessage()), W_ERROR);
    }
}

/**
 *
 * @param objMenu
 * @param evento
 * @return
 */
bool BaseFrontend::procesarPopups(tmenu_gestor_objects *objMenu, tEvento *evento){
        Object *object = objMenu->getObjByPos(objMenu->getFocus());
        if (object != NULL){
            try{
                //Comprobamos si el elemento que estamos pintando deberia mostrar su menu de popup
                if (object->isPopup() && object->isFocus()){
                    Traza::print("procesarPopups", W_DEBUG);
                    //Obtenemos el objeto popup
                    UIPopupMenu *objPopup = (UIPopupMenu *)objMenu->getObjByName(object->getPopupName());
                    //Mostramos el popup
                    objPopup->setVisible(true);
                    //Seteamos la posicion del menu popup
                    if (evento->mouse_x > 0 && evento->mouse_y > 0){
                        //Si no tenemos espacio horizontal por la derecha, intentamos mostrar el popup por
                        //la izquierda del lugar clickado
                        if (evento->mouse_x + objPopup->getW() > this->getWidth()){
                            objPopup->setX(evento->mouse_x - objPopup->getW());
                        } else {
                            objPopup->setX(evento->mouse_x);
                        }
                        //Si no tenemos espacio vertical por debajo, intentamos mostrar el popup por
                        //encima del lugar clickado
                        if (evento->mouse_y + objPopup->getH() > this->getHeight()){
                            objPopup->setY(evento->mouse_y - objPopup->getH());
                        } else {
                            objPopup->setY(evento->mouse_y);
                        }
                    }
                    //Asignamos el elemento que ha llamado al popup
                    objPopup->setCallerPopup(objMenu->getObjByPos(objMenu->getFocus()));
                    //Damos el foco al popup
                    objMenu->setFocus(object->getPopupName());
                    //Evitamos que el elemento procese cualquier evento mientras aparezca el popup
                    object->setEnabled(false);
                    //Forzamos a que se refresque el elemento padre que lanzo el popup
                    object->setImgDrawed(false);
                }
            } catch (Excepcion &e) {
                Traza::print("Excepcion en popup: " + object->getPopupName() + ". " + string(e.getMessage()), W_ERROR);
            }
        }

    return true;
}

/**
 *
 * @param pantalla
 * @param popupName
 * @param callerName
 * @return
 */
UIPopupMenu * BaseFrontend::addPopup(string pantalla, string popupName, string callerName){
    UIPopupMenu * popup1 = NULL;

    try{
        tmenu_gestor_objects *objMenu = getMenu(pantalla);
        objMenu->getObjByName(callerName)->setPopupName(popupName);
        getMenu(pantalla)->add(popupName, GUIPOPUPMENU, 0, 0, 170, 100, popupName, false)->setVisible(false);
        popup1 = (UIPopupMenu *) objMenu->getObjByName(popupName);
        popup1->setFont(getFont());
        popup1->setAutosize(true);
    } catch (Excepcion &e){
        Traza::print("addPopup: " + string(e.getMessage()), W_ERROR);
    }

    return popup1;
}

/**
 *
 * @param evento
 * @return
 */
string BaseFrontend::casoJOYBUTTONS(tEvento *evento){
    ignoreButtonRepeats = true;
    configButtonsJOY(evento);
    return "";
}

/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::simularIntro(tEvento *evento){
    evento->isKey = true;
    evento->key = SDLK_RETURN;
    evento->isJoy = true;
    evento->joy = JOY_BUTTON_A;
    return true;
}


/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::simularEscape(tEvento *evento){
    evento->isKey = true;
    evento->key = SDLK_ESCAPE;
    evento->isJoy = true;
    evento->joy = JOY_BUTTON_START;
    return true;
}

/**
 *
 * @param evento
 * @return
 */
int BaseFrontend::marcarBotonSeleccionado(tEvento *evento){
    tmenu_gestor_objects *objMenu = getMenu(getSelMenu());
    int pos = objMenu->getFocus();

    Traza::print("marcarBotonSeleccionado: " + objMenu->getObjByPos(pos)->getName(), W_DEBUG);
    if (pos >= 0){
        objMenu->getObjByPos(pos)->setTag("selected");
        return 1;
    }
    return 0;
}

/**
 *
 * @param btnAceptar
 * @param btnCancelar
 * @param pantalla
 * @return
 */
bool BaseFrontend::waitAceptCancel(string btnAceptar, string btnCancelar, string pantalla){
    string menuInicial = getSelMenu();
    tEvento askEvento;
    clearEvento(&askEvento);

    SDL_Rect iconRectFondo = {0, 0, (Uint16)this->getWidth(), (Uint16)this->getHeight()};
    SDL_Surface *mySurface = NULL;
    clearScr();

    procesarControles(getMenu(menuInicial), &askEvento, NULL);
    drawRectAlpha(iconRectFondo.x, iconRectFondo.y, iconRectFondo.w, iconRectFondo.h , cNegro, ALPHABACKGROUND);
    takeScreenShot(&mySurface, iconRectFondo);

    setSelMenu(pantalla);
    tmenu_gestor_objects *obj = getMenu(pantalla);
    long delay = 0;
    unsigned long before = 0;
    obj->setFocus(0);
    bool salir = false;
    bool salida = false;

    for (int i=0; i < obj->getSize(); i++){
        obj->getObjByPos(i)->setImgDrawed(false);
    }
    procesarControles(obj, &askEvento, NULL);
    flipScr();


    do{
        before = SDL_GetTicks();
        askEvento = WaitForKey();
        printScreenShot(&mySurface, iconRectFondo);

        procesarControles(obj, &askEvento, NULL);
        flipScr();
        salir = (askEvento.isJoy && askEvento.joy == JoyMapper::getJoyMapper(JOY_BUTTON_B)) ||
        (askEvento.isKey && askEvento.key == SDLK_ESCAPE);

        if (obj->getObjByName(btnAceptar)->getTag().compare("selected") == 0){
            salir = true;
            salida = true;
            obj->getObjByName(btnAceptar)->setTag("");
            Traza::print("Detectado SI pulsado", W_DEBUG);
        } else if (obj->getObjByName(btnCancelar)->getTag().compare("selected") == 0){
            salir = true;
            salida = false;
            obj->getObjByName(btnCancelar)->setTag("");
            Traza::print("Detectado NO pulsado", W_DEBUG);
        }

        delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
        if(delay > 0) SDL_Delay(delay);
    } while (!salir);

    setSelMenu(menuInicial);

    return salida;
}

/**
* Copia el texto seleccionado desde un popup al elemento que lo llama. Por ahora solo lo hace
* en campos input.
*/
int BaseFrontend::accionCopiarTextoPopup(tEvento *evento){
    Traza::print("accionCopiarTextoPopup", W_INFO);
    //Se obtiene el objeto menupopup que en principio esta seleccionado
    string menu = this->getSelMenu();
    tmenu_gestor_objects *objsMenu = getMenu(menu);
    Object *obj = objsMenu->getObjByPos(objsMenu->getFocus());
    //Comprobamos que efectivamente, el elemento es un popup
    if (obj->getObjectType() == GUIPOPUPMENU){
        UIPopupMenu *objPopup = (UIPopupMenu *)obj;
        //Obtenemos el uivalor del elemento seleccionado en el popup
        string selected = objPopup->getListValues()->get(objPopup->getPosActualLista());
        if (objPopup->getCallerPopup() != NULL){
            //Obtenemos el objeto llamador
            if (objPopup->getCallerPopup()->getObjectType() == GUIINPUTWIDE){
                UIInput *objInput = (UIInput *)objPopup->getCallerPopup();
                //Anyadimos el texto al input
                objInput->setText(objInput->getText() + selected);
                //Devolvemos el foco al elemento que llamo al popup
                objsMenu->setFocus(objPopup->getCallerPopup()->getName());
            }
        }
    }
    return 0;
}

/**
*
*/
void BaseFrontend::ComprobarPopupsCerrados(tmenu_gestor_objects *objMenu, tEvento *evento){
    //Comprobacion para evitar problemas con popups
    Object *object = objMenu->getObjByPos(objMenu->getFocus());
    if (object != NULL){
        if (object->isPopup() == false && object->isFocus() == false && evento
            && objMenu->getObjByName(object->getName())->getObjectType() == GUIPOPUPMENU)
        {
            if (evento->isMouse && (evento->mouse == MOUSE_BUTTON_RIGHT || evento->mouse == MOUSE_BUTTON_LEFT)
                && evento->mouse_state == SDL_PRESSED)
            {
                UIPopupMenu *objPopup = (UIPopupMenu *)objMenu->getObjByName(object->getName());
                objMenu->setFocus(objPopup->getCallerPopup()->getName());
                Traza::print("ComprobarPopupsCerrados. Foco a: " + objPopup->getCallerPopup()->getName(), W_DEBUG);
            }
        }
    }
}
