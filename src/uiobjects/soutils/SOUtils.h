/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SOUtils.h
 * Author: Ryuk
 *
 * Created on 23 de marzo de 2020, 18:34
 */

#ifndef SOUTILS_H
#define SOUTILS_H

#include "../Constant.h"
#include "../Dirutil.h"
#include "../listaIni.h"

#include <conio.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

class SOUtils {
public:
    SOUtils();
    virtual ~SOUtils();

    static string iniciarSistema(char** argv);
    static bool loadConfig();
    static void waitms(unsigned long ms);
    static string GetClipboardText();
    static int getChTimeout(int timeout);
private:

};

#endif /* SOUTILS_H */

