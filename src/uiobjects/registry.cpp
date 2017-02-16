#include "registry.h"

Registry::Registry()
{
    bits64 = false;
}

Registry::~Registry()
{
    //dtor
}

#ifdef WIN
/**
* hKey:
    HKEY_CLASSES_ROOT
    HKEY_CURRENT_CONFIG
    HKEY_CURRENT_USER
    HKEY_LOCAL_MACHINE
    HKEY_USERS

*/
long Registry::setKeyReg(HKEY hKey, string rama, string clave, string valor){
    HKEY hkey;
    DWORD opt = KEY_ALL_ACCESS;

    if (get64bits())
        opt = opt | KEY_WOW64_64KEY;

    RegOpenKeyEx(hKey, rama.c_str(), 0, opt, &hkey);
    long ret2 = RegSetValueEx(hkey, clave.c_str(), 0, REG_SZ, (const unsigned char*)valor.c_str(), valor.length());
    RegCloseKey(hkey);
    return ret2;
}

/**
*
*/
long Registry::deleteKeyReg(HKEY hKey, string rama, string clave){
    HKEY phkResult;
    DWORD ulOptions = 0;
    REGSAM samDesired = KEY_SET_VALUE;
    if (get64bits()){
        samDesired = samDesired | KEY_WOW64_64KEY;
    }
    RegOpenKeyEx(hKey, rama.c_str(), ulOptions, samDesired, &phkResult);
    long ret2 = RegDeleteValue(phkResult, clave.c_str());
    RegCloseKey(phkResult);
    return ret2;
}
#endif
