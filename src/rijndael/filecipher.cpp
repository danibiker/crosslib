#include "filecipher.h"


/**
* Constructor
*/
filecipher::filecipher(){
    //ctor
    setCipherMode(CBC);
    iv = NULL;
    k = NULL;
    lenK = 0;
    lenIV = 0;
    setPadding(true);
}

/**
* Destructor
*/
filecipher::~filecipher()
{
    //dtor
}

/**
* cifrar
*/
ifstream::pos_type filecipher::cifrar(const char *rutaOrig, const char *rutaDest){

	char buffer[blockSize]; //Para bloques de 128 bits son 16 bytes
	unsigned char tempIV[blockSize];
	size_t size = 0; // here

	if (comprobarDatos()){
        Rijndael *cifrador = new Rijndael();

        if (iv != NULL && getCipherMode() == CBC) {
            memcpy(tempIV, iv, blockSize);
        }

        cifrador->setClave(k);
        cifrador->crearClaves(lenK, blockSize);

        ifstream myFile (rutaOrig, ios::in | ios::binary);
        ofstream myFile2 (rutaDest, ios::out | ios::binary);
        int leido = 0;
        int restBytes = 0;

        myFile.seekg(0, ios::end);
        size = myFile.tellg();
        myFile.seekg(0, ios::beg);

        cout << "iniciando lectura" << endl;
        try {
            if (myFile.is_open()){
                do{
                    myFile.read (buffer, blockSize);
                    leido = myFile.gcount();

                    restBytes = blockSize - leido;
                    if (leido != blockSize || myFile.eof()){
                        //cout << "Tenemos que rellenar con: " << restBytes << " bytes" << endl;
                        size += restBytes;
                        //Establecer relleno conforme PKCS#5 	2.0
                        for (int i = blockSize - restBytes; i < blockSize; i++){
                            buffer[i] = restBytes * 8;
                        }
                    }

                    if (getCipherMode() == CBC) doXor((unsigned char* )buffer, tempIV);
                    cifrador->setData((unsigned char* )buffer);
                    cifrador->cifrar();
                    if (getCipherMode() == CBC) memcpy(tempIV, buffer, blockSize);
                    myFile2.write(buffer, blockSize);
                } while (!myFile.eof() );
            }
            else cout << "Unable to open file" << endl;

        } catch (ifstream::failure const &e) {
            cout << "Exception opening/reading file";
        } catch (exception const &ex) {
            cout << "Excepcion no contemplada";
        }
        myFile.close();
        myFile2.close();
        delete cifrador;
	}
	return size;
}



/**
* Guarda el resultado del XOR en elem1
*/
void filecipher::doXor(unsigned char *elem1, const unsigned char *elem2){
    for (int i=0; i < blockSize; i++){
        elem1[i] = elem1[i] ^ elem2[i];
    }
}

/**
* descifrar
*/
void filecipher::descifrar(const char *rutaOrig, const char *rutaDest){
	int len = blockSize;
	char buffer[blockSize]; //Para bloques de 128 bits
	size_t size = 0; // here
	unsigned char lenRelleno = 0;
	unsigned char tempIV[blockSize];
	unsigned char textoCifrado[blockSize];
	Rijndael *cifrador = NULL;

	if (comprobarDatos()){

        cifrador = new Rijndael();

        if (iv != NULL) {
            memcpy(tempIV, iv, blockSize);
        }

        cifrador->setClave(k);
        cifrador->crearClaves(lenK, blockSize);

        ifstream myFile (rutaOrig, ios::in | ios::binary);
        ofstream myFile2 (rutaDest, ios::out | ios::binary);
        int leido = 0;

        cout << "iniciando lectura" << endl;

        if (myFile.is_open()){

            myFile.seekg(0, ios::end);
            size = myFile.tellg();
            myFile.seekg(0, ios::beg);

            while (!myFile.eof()){
                myFile.read (buffer, blockSize);
                leido = myFile.gcount();
                if (getCipherMode() == CBC) memcpy(textoCifrado, buffer, blockSize); //Guardamos una copia del texto cifrado para hacer un XOR en las siguientes rondas

                if (leido == blockSize && !myFile.eof()){
                    size -= blockSize;
                    //Procesar el bloque y descifrarlo
                    cifrador->setData((unsigned char* ) buffer);

                    cifrador->descifrar();
                    if (getCipherMode() == CBC){
                        doXor((unsigned char *)buffer, tempIV); //Deshacemos el XOR que hicimos en el cifrado
                        memcpy(tempIV, textoCifrado, blockSize);//Guardamos para hacer el XOR con el texto cifrado en la siguiente ronda
                    }

                    if (size == 0){
                        //lenRelleno = ((int)(unsigned char)buffer[blockSize - 1])/8;
                        lenRelleno = buffer[blockSize - 1];
                        lenRelleno = lenRelleno >> 3; //Dividimos entre 8
                        len -= lenRelleno;
                        //cout << "Detectado relleno de: " << (int)lenRelleno << " bytes" << endl;
                    }
                    if (len > 0) myFile2.write(buffer, len);
                } else if (leido != 0){
                    cout << "Ha ocurrido un error al descifrar" << endl;
                } else {
                    cout << "Fichero descifrado con exito" << endl;
                }
            }
        }
        else cout << "Unable to open file" << endl;

        myFile.close();
        myFile2.close();
        delete cifrador;

	}

}

/**
*
*/
ifstream::pos_type filecipher::cifrarData(const char *origen, int tam, char *destino){
    char buffer[blockSize];
    int posArray = 0;
    unsigned char tempIV[blockSize];
    int leido = 0;
    int restBytes = 0;

    if (comprobarDatos()){
        Rijndael *cifrador = new Rijndael();
        cifrador->setClave(k);
        cifrador->crearClaves(lenK, blockSize);

        if (iv != NULL && getCipherMode() == CBC) {
            memcpy(tempIV, iv, blockSize);
        }

        //Entramos hasta que procesemos todo el mensaje o en el caso de que hayamos
        //procesado todo el mensaje pero no nos haya cabido el indicador del relleno
        while (posArray < tam || (posArray == tam && restBytes == 0 && padding)){
            memset(buffer, '\0', blockSize);
            leido = 0;
            for (int posBuffer = 0; posBuffer < blockSize && posArray < tam; posBuffer++){
                buffer[posBuffer] = origen[posArray];
                posArray++;
                leido++;
            }

            restBytes = blockSize - leido;
            if (restBytes != 0){
                memset(&buffer[blockSize - restBytes], restBytes * 8, restBytes);
            }

            //Procesar el bloque y cifrarlo con un XOR con las IV
            if (getCipherMode() == CBC) doXor((unsigned char* )buffer, tempIV);
            cifrador->setData((unsigned char *)buffer);
            cifrador->cifrar();
            if (getCipherMode() == CBC) memcpy(tempIV, buffer, blockSize);
            memcpy(&destino[posArray - blockSize + restBytes], (char *)buffer, blockSize);
        }
        delete cifrador;
    }
    return tam + restBytes;
}

/**
* descifrar
*/
int filecipher::descifrarData(const char *origen, int tam, char *destino){
    char buffer[blockSize];
    int posArray = 0;
    unsigned char tempIV[blockSize];
    unsigned char textoCifrado[blockSize];
    int leido = 0;
    unsigned char lenRelleno = 0;
    int relleno = 0;

    if (comprobarDatos()){

        if (iv != NULL && getCipherMode() == CBC) {
            memcpy(tempIV, iv, blockSize);
        }

        Rijndael *cifrador = new Rijndael();
        cifrador->setClave(k);
        cifrador->crearClaves(lenK, blockSize);


        if (tam % blockSize == 0 && tam > 0){
            while (posArray < tam){
                memset(buffer, '\0', blockSize);
                leido = 0;
                for (int posBuffer = 0; posBuffer < blockSize && posArray < tam; posBuffer++){
                    buffer[posBuffer] = origen[posArray];
                    posArray++;
                    leido++;
                }

                if (getCipherMode() == CBC) memcpy(textoCifrado, buffer, blockSize); //Guardamos una copia del texto cifrado para hacer un XOR en las siguientes rondas
                //print((unsigned char *)buffer,blockSize);
                cifrador->setData((unsigned char *)buffer);
                cifrador->descifrar();
                if (getCipherMode() == CBC){
                    doXor((unsigned char *)buffer, tempIV); //Deshacemos el XOR que hicimos en el cifrado
                    memcpy(tempIV, textoCifrado, blockSize);//Guardamos para hacer el XOR con el texto cifrado en la siguiente ronda
                }
                //Si es el ultimo bloque desciframos y buscamos la cantidad de relleno
                if (posArray == tam && padding) {
                    lenRelleno = buffer[blockSize - 1];
                    lenRelleno = lenRelleno >> 3; //Dividimos entre 8
                    if (lenRelleno < blockSize){
                        //strncat(destino, (char *)buffer, blockSize - (int)lenRelleno);
                        memcpy(&destino[posArray - blockSize], (char *)buffer, blockSize - (int)lenRelleno);
                        relleno = lenRelleno;
                    }
                } else {
                    //strncat(destino, (char *)buffer, blockSize);
                    memcpy(&destino[posArray - blockSize], (char *)buffer, blockSize);
                    relleno = blockSize;
                }
            }
        } else {
             cout << "Error en el elemento cifrado. size: " << tam << "; blocksize: " << blockSize << endl;
        }
        delete cifrador;
    }
    return relleno;
}

/**
* Basado en PBKDF1 pero con SHA-2 que genera 256bits
*/
void filecipher::generarHash(string password, string salt, int hashLen, unsigned char *output){
    if (hashLen <= 32){
        string output1;
        string dato;
        char * p;
        long n = 0;
        output1 = password + salt;

        for (int i=0; i < PKCSITERATIONS; i++){
            output1 = sha256(output1);//Genera 256bits
        }

        for (int i=0; i < hashLen; i++){
            dato = output1.substr(i*2,2);
            n = strtol( dato.c_str(), &p, 16 );
            if ( * p != 0 ) {
                output[i] = 0x00;
            }
            else {
                output[i] = n;
            }
        }

    }
}



void filecipher::pruebaDescifrado(const char *rutaOrig, const char *rutaDest, ifstream::pos_type ini, ifstream::pos_type tam){

    ifstream myFile (rutaOrig, ios::in | ios::binary);
//    size_t size = 0; // here

//    myFile.seekg(0, ios::end);
//    size = myFile.tellg();
//    myFile.seekg(0, ios::beg);

    char *buffer = new char[tam]; //Para bloques de 128 bits

    myFile.seekg (ini, ios::beg);//Volvemos al principio del fichero
    myFile.read (buffer, tam);
    myFile.close();
    //descifrarData(buffer, rutaDest, tam);
    delete [] buffer;

}


/**
* comprobarDatos
*/
bool filecipher::comprobarDatos(){
    if (k == NULL){
        cout << "No se ha introducido ninguna clave" << endl;
        return false;
    }
    else if ((lenK == 16 || lenK == 24 || lenK == 32) == false){
        cout << "La longitud de la clave no coincide con los valores aceptados (128, 192, 256): " << (int)(lenK * 8) << endl;
        return false;
    }
    else if (iv == NULL && getCipherMode() == CBC){
        cout << "En modo CBC es necesario introducir un Vector de Inicializacion" << endl;
        return false;
    }
    else if (blockSize != lenIV && getCipherMode() == CBC){
        cout << "La longitud del IV (" << (int)lenIV << ") no coincide con la longitud del bloque (" << (int)blockSize << ")" << endl;
        return false;
    }
    else
    {
        return true;
    }
}

/**
* setClave
*/
void filecipher::setClave(unsigned char *var, int len){
    if (k == NULL){
        k = new unsigned char[len];
    } else {
        delete [] k;
        k = new unsigned char[len];
    }
    memcpy(k, var, len);
    lenK = len;
}

/**
* setIV
*/
void filecipher::setIV(unsigned char *var, int len){
    if (iv == NULL){
        iv = new unsigned char[len];
    } else {
        delete [] iv;
        iv = new unsigned char[len];
    }
    memcpy(iv, var, len);
    lenIV = len;
}

/**
*
*/
string filecipher::encodeEasy(string src, string password){
    unsigned char keyT[32];
    unsigned char ivT[16];

    if (!password.empty()){
        //Generamos la clave de 32 bytes
        this->generarHash(password, (char *)SALCLAVE, 32, keyT);
        //Generamos la semilla de 16 bytes
        this->generarHash(SEMILLAFACIL, (char *)SALIV, 16, ivT);
        this->setClave(keyT, 32);
        this->setIV(ivT, 16);
    }

    char *cipheredStr = new char[src.length() + blockSize + 1];
    long tam = this->cifrarData(src.c_str(), src.length(), cipheredStr);
    string cipherStrB64 = Constant::base64_encode(reinterpret_cast<const unsigned char*>(cipheredStr), tam);
    delete [] cipheredStr;
    return cipherStrB64;
}

/**
*
*/
string filecipher::decodeEasy(string src, string password){
    unsigned char keyT[32];
    unsigned char ivT[16];

    if (!password.empty()){
        //Generamos la clave de 32 bytes
        this->generarHash(password, (char *)SALCLAVE, 32, keyT);
        //Generamos la semilla de 16 bytes
        this->generarHash(SEMILLAFACIL, (char *)SALIV, 16, ivT);
        this->setClave(keyT, 32);
        this->setIV(ivT, 16);
    }
    string ciphered = Constant::base64_decode(src);
    char *decipheredStr = new char[ciphered.length()];
    memset(decipheredStr, '\0', ciphered.length());
    this->descifrarData(ciphered.c_str(), ciphered.length(), decipheredStr);
    string result = decipheredStr;
    delete [] decipheredStr;
    return result;
}
