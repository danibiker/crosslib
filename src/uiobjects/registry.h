#ifndef REGISTRY_H
#define REGISTRY_H

#include "Constant.h"
#ifndef KEY_WOW64_64KEY
    #define KEY_WOW64_64KEY 0x0100
#endif // KEY_WOW64_64KEY
#ifndef KEY_WOW64_32KEY
    #define KEY_WOW64_32KEY 0x0200
#endif // KEY_WOW64_32KEY

class Registry
{
    public:
        Registry();
        virtual ~Registry();

        #ifdef WIN
        long setKeyReg(HKEY hKey, string rama, string clave, string valor);
        long deleteKeyReg(HKEY hKey, string rama, string clave);

        void set64bits(bool var){bits64 = var;}
        bool get64bits(){return bits64;}
        #endif

    protected:
    private:
        bool bits64;
};

#endif // REGISTRY_H
