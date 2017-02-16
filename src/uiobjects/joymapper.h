#ifndef JOYMAPPER_H
#define JOYMAPPER_H

#include "Constant.h"
#include "ListaSimple.h"

class JoyMapper
{
    protected:
    private:
        static int *joyButtonMapper;
        static string rutaIni;

    public:
        JoyMapper();
        virtual ~JoyMapper();
        static void initJoyMapper();
        static int getJoyMapper(int button);
        static void setJoyMapper(int button, int value);
        static void clearJoyMapper();
        static void saveJoyConfig();


};

#endif // JOYMAPPER_H
