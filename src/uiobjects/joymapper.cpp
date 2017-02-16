#include "joymapper.h"

int *JoyMapper::joyButtonMapper;
//listaSimple<string> *JoyMapper::fileConfigJoystick;
string JoyMapper::rutaIni;

JoyMapper::JoyMapper()
{
//    fileConfigJoystick = NULL;
}

JoyMapper::~JoyMapper()
{
    clearJoyMapper();
}

/**
*
*/
void JoyMapper::saveJoyConfig(){
    if (joyButtonMapper != NULL && !rutaIni.empty()){
        listaSimple<string> *fileConfigJoystick = new listaSimple<string>();

        string fila = "";
        for (int i = 0; i < MAXJOYBUTTONS; i++){
            fila = "JOY_" + Constant::TipoToStr(i) + "=" + Constant::TipoToStr(joyButtonMapper[i]);
            fileConfigJoystick->add(fila);
        }
        Traza::print("Guardando en rutaIni: " + rutaIni, W_DEBUG);
        fileConfigJoystick->writeToFile(rutaIni);
        delete fileConfigJoystick;
        Traza::print("Configuracion guardada correctamente", W_DEBUG);
    }
}

/**
*
*/
void JoyMapper::initJoyMapper(){
    joyButtonMapper = NULL;
    rutaIni = Constant::getAppDir() + Constant::getFileSep() + "joystick.ini";
    listaSimple<string> *fileConfigJoystick = new listaSimple<string>();

    bool resetJoy = false;

    if (joyButtonMapper == NULL){
        joyButtonMapper = new int[MAXJOYBUTTONS];
    }

    try{
        fileConfigJoystick->loadStringsFromFile(rutaIni.c_str());
        if (fileConfigJoystick->getSize() > 0){
            string linea = "";
            for (unsigned int i=0; i<fileConfigJoystick->getSize(); i++){
                linea = fileConfigJoystick->get(i);
                if (i < MAXJOYBUTTONS){
                    int valorIni = Constant::strToTipo<int>(Constant::split(linea, "=").at(1));
                    joyButtonMapper[i] = valorIni;
                }
            }
        } else {
            resetJoy = true;
        }
        delete fileConfigJoystick;

    } catch (Excepcion &e){
        Traza::print("Error cargando la configuracion de joystick: " + string(e.getMessage()), W_ERROR);
        resetJoy = true;
    }

    if (resetJoy){
        for (int i = 0; i < MAXJOYBUTTONS; i++){
            joyButtonMapper[i] = -1;
        }
    }
}

/**
*/
int JoyMapper::getJoyMapper(int button){
    if (joyButtonMapper != NULL){
        if (button < MAXJOYBUTTONS && button >= 0){
            return joyButtonMapper[button];
        }
    }
    return -1;
}

void JoyMapper::setJoyMapper(int button, int value){
    if (joyButtonMapper != NULL){
        if (button < MAXJOYBUTTONS && button >= 0){
            joyButtonMapper[button] = value;
        }
    }
}

/**
*
*/

void JoyMapper::clearJoyMapper(){
        if (joyButtonMapper != NULL){
            delete [] joyButtonMapper;
            joyButtonMapper = NULL;
        }
}

