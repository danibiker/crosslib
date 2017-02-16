#include "Excepcion.h"

/**
*
*/
const char* Excepcion::getMessage(){
    return msg.c_str();
}

/**
*
*/
const char* Excepcion::generateMessage(int code){
    string res = "";
    if (code < 1000){
        return Excepcion::msg.c_str();
    } else {
        switch (code){
            case ERANGO:   res = "Error de Rango";break;
            case EESPACIO: res = "No se ha reservado suficiente memoria";break;
            case ENULL: res = "Se ha intentado acceder a un objeto NULL";break;
            case EFIO: res = "Se ha producido un error al acceder a un fichero";break;
            case ERANGOSQL: res = "Error seteando campos en consulta sql"; break;
            default: res = "Error Desconocido";  //En rigor no debería ocurrir
        }
    }
    return res.c_str(); //Evita warnings
}

/**
* Constructor
*/
Excepcion::Excepcion(int m){
    msg = "Excepcion: ";
    msg.append(Constant::TipoToStr(m));
    msg.append(";");
    msg.append(generateMessage(m));
    //Traza::print(dato.c_str(), W_ERROR);
}

/**
* Constructor
*/
Excepcion::Excepcion(int motivo, string message){
    Excepcion::msg = message + "; " + Excepcion::generateMessage(motivo);
//    msg.append(Constant::TipoToStr(getCode()));
//    msg.append(";");
//    msg.append(generateMessage());
//    msg = msg + ";" + message;
//    Traza::print(string("Excepcion::Excepcion: " + msg).c_str(),code, W_ERROR);
}

/**
* Constructor
*/
Excepcion::Excepcion(){
}
