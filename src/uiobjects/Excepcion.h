#ifndef Excepcion_H
#define Excepcion_H

    #include <iostream>
    #include <cmath>
    #include "uiobjects/Traza.h"
    #include "uiobjects/Constant.h"

    using namespace std;

    //static const int ERANGO=100;
    //static const int EESPACIO=101;
    //static const int ENULL=102;
    //static const int EFIO=103;

    static const int ERANGO=1000;
    static const int EESPACIO=1001;
    static const int ENULL=1002;
    static const int EFIO=1003;
    static const int ERANGOSQL=1004;



    class Excepcion{
    public:
        Excepcion();
        Excepcion(int);
        Excepcion(int, string);
        const char* getMessage();
        //Errores de listas
        int getCode(){return code;}


    private:
        string msg;
        const char* generateMessage(int code);
        int code;
    };

#endif // Excepcion_H
