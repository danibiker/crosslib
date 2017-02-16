#ifndef FILECIPHER_H
#define FILECIPHER_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "rijndael.h"
#include "sha256.h"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include "Constant.h"

#define vLength(x) (sizeof(x)/sizeof(x[0]))

//BlockSize en Bytes. ESTO NO DEBE CAMBIARSE PARA QUE SEA CORRECTO CON EL ESTANDAR DE AES
const int blockSize = 16;
const int PKCSITERATIONS = 5;
//Para los metodos faciles de cifrado y descifrado
const int SALCLAVE[32] = {0x5b,0xa3,0xc6,0xb3,0xc5,0x2e,0x50,0x5d,0x89,0xcd,0xab,0xc3,0x94,0x17,0xae,0x57,0xe7,0x47,0x92,0x4f,0x77,0,0x5a,0x42,0x6a,0x3b,0x72,0xc,0x87,0x6d,0x6b,0x79};
const int SALIV[16] =    {0xb2,0x62,0x00,0x78,0x63,0x8b,0x65,0x09,0x53,0x49,0x44,0xe6,0xf2,0x2d,0xfd,0x71};
const string SEMILLAFACIL = "5f4tWSwOPWcO2zWQfjsM";


using namespace std;

class filecipher
{
    public:
        filecipher();
        virtual ~filecipher();
        ifstream::pos_type cifrar(const char *, const char *);
        void descifrar(const char *, const char *);

        void setCipherMode(int var){cipherMode = var;}
        int getCipherMode(){return cipherMode;}

        void setClave(unsigned char *, int);
        void setIV(unsigned char *, int);

        string encodeEasy(string src, string password);
        string decodeEasy(string src, string password);

        unsigned char getLenK(){return lenK;}
        unsigned char getLenIV(){return lenIV;}
        int descifrarData(const char *origen, int tam, char *destino);
        ifstream::pos_type cifrarData(const char *origen, int tam, char *destino);
        void pruebaDescifrado(const char *, const char *, ifstream::pos_type, ifstream::pos_type);
        void generarHash(string password, string salt, int hashLen, unsigned char *output);
        void doXor(unsigned char *, const unsigned char *);
        void setPadding(bool var){
            padding = var;
        }


    private:
        unsigned char *k;
        unsigned char *iv;
        unsigned char cipherMode;
        int lenK;
        int lenIV;
        bool padding;
        bool comprobarDatos();

};

enum{ECB,CBC};

#endif // FILECIPHER_H
