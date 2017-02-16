#include <iostream>
#include "rijndael.h"
#include "filecipher.h"
#include <ctime>

using namespace std;

int salClave[32] = {0xba,0xa3,0x7,0x99,0x1b,0x56,0x66,0xb6,0x2a,0x36,0xe4,0x89,0xd2,0x36,0x55,0x55,0x54,0x46,0x9a,0x95,0x15,0x6c,0xad,0xf7,0,0xc4,0xf5,0x63,0x4a,0xb0,0xfb,0xf0};
int salIv[16] = {0xc3,0x49,0xf0,0xa0,0x72,0x22,0xc,0x63,0xf1,0xd3,0xe8,0x87,0x90,0x85,0,0xa5};

/**
*
*/
void print(unsigned char *data, int len){
    cout << "[" ;
    for (int i = 0; i < len; i++){
       cout << hex << showbase << (int)data[i];
       if (i < len-1) cout << ",";
    }
    cout << "]" << endl ;
}

void generarRandom(int size){
    std::srand(std::time(0)); // use current time as seed for random generator
    std::cout << "{";
    for (int i=0; i < size ;i++){
        int random_variable = std::rand() % 0xFF;
        std::cout << hex << showbase << random_variable;
        if (i < size-1)
            std::cout << ",";
    }
    std::cout << "}" << endl;
}


/**
*
*/
int main(){
    generarRandom(32);
    string pause;
    cin >> pause;
    generarRandom(16);

    filecipher file;
/*
    unsigned char k[32];
    unsigned char iv[16];
    cout << "---------------------" << endl;
    filecipher *file = new filecipher();
    //Generamos la clave de 32 bytes
    file->generarHash("Rouco Varela", (char *)salClave, 32, k);
    //Generamos la semilla de 16 bytes
    file->generarHash("alssj8289", (char *)salIv, 16, iv);

    file->setCipherMode(CBC);
    file->setClave(k, 32);
    file->setIV(iv, 16);

    char b2[] = {"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean eget interdum felis. Vestibulum venenatis vel metus sed lobortis. Integer ultrices elit eu odio suscipit, et pulvinar metus cursus. Maecenas a fermentum nulla. Pellentesque faucibus nec nibh fringilla sagittis. Etiam mattis ultricies quam vitae blandit. Etiam efficitur porttitor rutrum. Interdum et malesuada fames ac ante ipsum primis in faucibus. Sed id sem egestas, scelerisque ante eu, pulvinar massa. Integer dignissim aliquam arcu vitae sed. FIN"};
    //Un texto de tamanyo n producira un texto cifrado de tamanyo n + blockSize. Siendo
    //blocksize=16 para el estandar AES
    char *txtCifrado = new char[strlen(b2) + blockSize];
    //Ciframos el texto y lo guardamos en txtCifrado
    int lenCifrado = file->cifrarData(b2, strlen(b2), txtCifrado);
    //Sacamos el resultado por pantalla
    cout << "lenCifrado: " << lenCifrado << endl;
    //El texto descifrado tendra un tamanyo como mucho del tamanyo del cifrado
    char *txtDescifrado = new char[lenCifrado];
    //Desciframos el texto
    file->descifrarData(txtCifrado, lenCifrado, txtDescifrado);
    //Sacamos el resultado por pantalla
    cout << "txtDescifrado: " << txtDescifrado << endl;
    cout << "---------------------" << endl;

    file->cifrar("C:\\traza.txt", "c:\\traza2.txt");
    file->descifrar("C:\\traza2.txt", "c:\\traza3.txt");

    delete [] txtCifrado;
    delete [] txtDescifrado;

    delete file;
*/
    return 0;
}


