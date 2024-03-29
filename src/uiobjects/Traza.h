// Class automatically generated by Dev-C++ New Class wizard

#ifndef TRAZA_H
#define TRAZA_H

#include "uiobjects/Constant.h"
#include <stdio.h>
#include <time.h>
#include <string>
#include <limits.h>
#include <iostream>
#include <fstream>

using namespace std;

/*
 * Clase para generar trazas
 */
class Traza
{
	public:
		// class constructor
		Traza(const char *ruta);
		Traza();
		// class destructor
		~Traza();
         static void print(string, int, int); //Pinta el mensaje en la traza
		 static void print(string , int);
		 void setTraza(bool);


    private:
         static ofstream *fout;
  	     static bool pintarTrazas;
  	     char ruta[PATH_MAX];
  	     void iniFile();
  	     void closeFile();
};

#endif // TRAZA_H
