#include "rijndael.h"

/**
* Constructor
*/
Rijndael::Rijndael(){
    b = NULL;
    k = NULL;
    //setKeyBlockLength(ks128/8, ks128/8);
}

/**
* Destructor
*/
Rijndael::~Rijndael(){
}

/**
* cifrar: Se encarga de cifrar el bloque recibido
*/
void Rijndael::cifrar(){
    //Creacion de claves
    //cout << "************ CLAVES ************" << endl << endl;
    //crearClaves();
    //Acciones para el cifrado
    //Ronda inicial
    //cout << "************ RONDA INICIAL ************" << endl << endl;
    addroundkey(b,kExp, 0);
    //print(b);
    //Se realizan las 9 rondas siguientes
    unsigned char i;

    for (i=1; i <= Nr - 1; i++){
        //cout << "************ RONDA " << i << " ************" << endl << endl;
        bytesub(b);
        //print(b);
        shiftrow(b);
        //print(b);
        mixcolumn(b);
        //print(b);
        addroundkey(b,kExp, i);
        //cout << (int)i << ": ";
        //print(b);
    }
    //cout << "************ RONDA 10 ************" << endl << endl;
    bytesub(b);
    //print(b);
    shiftrow(b);
    //print(b);
    addroundkey(b,kExp, Nr);
//    cout << (int)Nr << ": ";
//    print(b);
//    cout << "Texto cifrado: ";
//    print(b);

    //Acciones para la generacion de las claves
}

/**
* descifrar: Se encarga de descifrar el bloque recibido
*/
void Rijndael::descifrar(){
     //Creacion de claves
    //cout << "************ CLAVES ************" << endl << endl;
    //crearClaves();

    unsigned char i;

    //cout << "************ RONDA 10 ************" << endl << endl;
    addroundkey(b,kExp, Nr);
    //print(b);
    invShiftrow(b);
    //print(b);
    invBytesub(b);
    //print(b);

    //Se realizan las 9 rondas siguientes
    for (i = Nr - 1; i > 0; i--){
        //cout << "************ RONDA " << i << " ************" << endl << endl;
        addroundkey(b,kExp, i);
        //print(b);
        invMixcolumn(b);
        //print(b);
        invShiftrow(b);
        //print(b);
        invBytesub(b);
        //print(b);
    }
    //cout << "************ RONDA 0 ************" << endl << endl;
    addroundkey(b,kExp, 0);
//    cout << "Texto descifrado: ";
//    print(b);
    //print(b);

}

/**
* setClave
*/
void Rijndael::setClave(unsigned char *clave){
    k = clave;
}

/**
* setData
*/
void Rijndael::setData(unsigned char *data){
    b = data;
}

/**
* crearClaves
*/
void Rijndael::crearClaves(unsigned int lenk, unsigned int lenb){
    //Primero copiamos la clave original en las primeras posiciones
    unsigned char i;
    setKeyBlockLength(lenk, lenb);

    for (i = 0; i < Nk * 4; i++){
        kExp[i] = k[i];
    }
    expand_key(kExp);
}


/**
* setKeyBlockLength
*/
void Rijndael::setKeyBlockLength(unsigned int varKey, unsigned int varBlock){
    blocklength = varBlock;
    Nb = blocklength * 8 /32;

    keylength = varKey;
    Nk = keylength * 8 / 32;

    if (Nb == 8 || (Nb == 6 && Nk == 8) || (Nb == 4 && Nk == 8)){
        Nr = 14;    //CASO AES-256
    } else if ((Nb == 6 && Nk < 8) || (Nb == 4 && Nk == 6)){
        Nr = 12;    //CASO AES-192
    } else {
        Nr = 10;    //CASO AES-128
    }
}

/**************************************************/
/**Metodos para el proceso de creacion de la clave*/
/**************************************************/

/**
* expand_key
*/
void Rijndael::expand_key(unsigned char *in) {
    unsigned char t[4];
    /* c is 16 because the first sub-key is the user-supplied key
    128bits = 16
    192bits = 24
    256bits = 32
    */
    unsigned char incrOffset = 4 * Nk;
    unsigned char c = incrOffset;
    unsigned char i = 1;
    unsigned char a;
    //El tamanyo de las claves debe ser igual al tamanyo de los bloques para cuando hagamos el addroundkey
    unsigned char numsets = (Nr + 1) * Nb * 4;
//    bool flag = false;

    /* We need 11 sets of sixteen bytes each for 128-bit mode
     1 set for the original key and 10 sets for the rest of rounds in 128-bit mode*/
    while(c < numsets) {
//        flag = false;
        /* Copy the temporary variable over from the last 4-byte
         * block */
        for(a = 0; a < 4; a++)
            t[a] = in[a + c - 4];
        /* Every four blocks (of four bytes),
         * do a complex calculation */
        if(c % incrOffset == 0) {
            schedule_core(t,i);
            i++;
        } else if (c % incrOffset == (Nb * 4) and Nk > 6){
            for(a = 0; a < 4; a++) {
                 t[a] = sbox(t[a]);
            }
        }

        for(a = 0; a < 4; a++) {
            in[c] = in[c - incrOffset] ^ t[a];
            c++;
        }
    }
}

/**
 * This is the core key expansion, which, given a 4-byte value,
 * does some scrambling
 */
void Rijndael::schedule_core(unsigned char *in, unsigned char i) {
        unsigned char a;
        /* Rotate the input 8 bits to the left */
        rotate(in);
        /* Apply Rijndael's s-box on all 4 bytes */
        for(a = 0; a < 4; a++)
                in[a] = sbox(in[a]);
        /* On just the first byte, add 2^i to the byte */
        in[0] ^= rcon(i);
}


/**
* rotate
*/
void Rijndael::rotate(unsigned char *in) {
        unsigned char a,c;
        a = in[0];
        for(c=0;c<3;c++)
                in[c] = in[c + 1];
        in[3] = a;
        return;
}

/**
* Calculate the rcon used in key expansion
*/
unsigned char Rijndael::rcon(unsigned char in) {
        unsigned char c=1;
        if(in == 0)
                return 0;
        while(in != 1) {
		unsigned char b;
		b = c & 0x80;
		c <<= 1;
		if(b == 0x80) {
			c ^= 0x1b;
		}
                in--;
        }
        return c;
}

/******************************************/
/**Metodos para el proceso de codificacion*/
/******************************************/

/**
* bytesub
* La tabla ya hace la transformacion lineal
*/
void Rijndael::bytesub(unsigned char *myB){
    unsigned char i;

    for (i=0; i < Nb * 4; i++){
        ////cout << hex << showbase << (int)myB[i] << ": ";
        myB[i] = sbox(myB[i]);
    }
}

/**
* bytesub
* La tabla ya hace la transformacion lineal
*/
void Rijndael::invBytesub(unsigned char *myB){
    unsigned char i;

    for (i=0; i < Nb * 4; i++){
        ////cout << hex << showbase << (int)myB[i] << ": ";
        myB[i] =invSbox(myB[i]);
    }
}

/**
* sbox
*/
unsigned char Rijndael::sbox(unsigned char value){
    return s[value];
}

/**
* invSbox
*/
unsigned char Rijndael::invSbox(unsigned char value){
    return inv_s [value];
}

/**
* shiftrow
*/
void Rijndael::shiftrow(unsigned char *myB){
    //cout << "shiftrow" << endl;
    unsigned char temp1; //Variable temporal para almacenar la primera celda
    unsigned char i; //Contador para las filas
    unsigned char j; //Contador para las rotaciones
    unsigned char k; //Contador para recorrer las columnas de cada fila

    //Especificamos las rotaciones que hay que hacer para las filas 1, 2, 3
    unsigned char rotFilas[3] = {1,2,3};

    //Si Nb == 8 las filas 2 y 3 tienen que hacer 3 y 4 rotaciones respectivamente
    if (Nb == 8) {
        rotFilas[1] = 3;
        rotFilas[2] = 4;
    }

    //Las rotaciones son para las filas 1 hasta la 3
    for (i = 1; i < 4; i++){
        //Hacemos las rotaciones de cada fila dependiendo del tamaño de bloque
        for (j = 0; j < rotFilas[i-1]; j++){

            //Guardamos el primer byte
            temp1 = myB[i];
            //Rotamos los bytes hacia la izquierda
            for (k = 0 ;k < Nb-1; k++){
                //cout << "Movemos: " << (4 * (k+1) + i) << " a: " << (4 * (k) + i) << endl;
                myB[4 * k + i] = myB[4 * (k+1) + i];
            }
            //Ponemos el primer byte guardado en la ultima posicion
            //cout << "Pos last: " << (4 * (k) + i) << endl;
            myB[4 * k + i] = temp1;
        }
    }
}

/**
* invShiftrow
*/
void Rijndael::invShiftrow(unsigned char *myB){
    //cout << "invshiftrow" << endl;
    unsigned char temp1;  //Variable temporal para almacenar la ultima celda
    unsigned char i; //Contador para las filas
    unsigned char j; //Contador para las rotaciones
    unsigned char k; //Contador para recorrer las columnas de cada fila


    //Especificamos las rotaciones que hay que hacer para las filas 1, 2, 3
    unsigned char rotFilas[3] = {1,2,3};

    //Si Nb == 8 las filas 2 y 3 tienen que hacer 3 y 4 rotaciones respectivamente
    if (Nb == 8) {
        rotFilas[1] = 3;
        rotFilas[2] = 4;
    }

    //Las rotaciones son para las filas 1 hasta la 3
    for (i=1; i < 4; i++){
        //Hacemos las rotaciones de cada fila dependiendo del tamaño de bloque
        for (j = rotFilas[i-1]; j > 0; j--){
            k = Nb-1;
            //Guardamos el ultimo byte
            temp1 = myB[ 4 * k + i];
            //Rotamos los bytes hacia la derecha
            for (;k > 0; k--){
                myB[4 * k + i] = myB[4 * (k-1) + i];
            }
            //Ponemos el ultimo byte guardado en la primera posicion
            myB[i] = temp1;
        }
    }
}


/**
* mixcolumn
*/
void Rijndael::mixcolumn(unsigned char *myB){
    //cout << "mixcolumn" << endl;
    unsigned char a[4]; //Almacena las filas de cada columna
    unsigned char i; //Contador para las columnas de la matriz de datos
    unsigned char c; //Contador para las filas de la matriz de datos

    //Recorremos cada columna
    for (i=0; i < Nb; i++){
        //Almacenamos las filas de cada columna en "a"
        for(c=i*4; c < i*4 + 4; c++) {
                a[c - i*4] = myB[c];
//                cout << "c: " << (int)c << endl;
        }
        //Realizamos la multiplicacion de polinomios
        //c(x ) = ‘03’ x3 + ‘01’ x2 + ‘01’ x + ‘02’
        //Como es multiplicacion en el campo finito 2^8 mejor si usamos tablas. Sino seria
        //Ej. multiplicar x por 2: (2 * x) MOD 2^8
        //La suma la hacemos mediante XOR para que no pase del campo finito 2^8
        myB[i*4]     = galoisX2[a[0]] ^ a[3] ^ a[2] ^ galoisX3[a[1]]; /* 2 * a0 + a3 + a2 + 3 * a1 */
        myB[i*4 + 1] = galoisX2[a[1]] ^ a[0] ^ a[3] ^ galoisX3[a[2]]; /* 2 * a1 + a0 + a3 + 3 * a2 */
        myB[i*4 + 2] = galoisX2[a[2]] ^ a[1] ^ a[0] ^ galoisX3[a[3]]; /* 2 * a2 + a1 + a0 + 3 * a3 */
        myB[i*4 + 3] = galoisX2[a[3]] ^ a[2] ^ a[1] ^ galoisX3[a[0]]; /* 2 * a3 + a2 + a1 + 3 * a0 */
//        cout << "calculando"<<endl;

    }
}


/**
* invMixcolumn
*/
void Rijndael::invMixcolumn(unsigned char *myB){
    //cout << "invMixcolumn" << endl;
    unsigned char a[4]; //Almacena las filas de cada columna
    int i; //Contador para las columnas de la matriz de datos
    int c; //Contador para las filas de la matriz de datos

    //Recorremos cada columna
    for (i=0; i < Nb; i++ ){
        //Almacenamos las filas de cada columna en "a"
        for(c=i*4;c < i*4 + 4;c++) {
                a[c - i*4] = myB[c];
        }
        //Realizamos la multiplicacion de polinomios
        //d(x ) = ‘0B’ x3 + ‘0D’ x2 + ‘09’ x + ‘0E’
        //Como es multiplicacion en el campo finito 2^8 mejor si usamos tablas. Sino seria
        //Ej. multiplicar x por 2: (2 * x) MOD 2^8
        //La suma la hacemos mediante XOR para que no pase del campo finito 2^8
        myB[i*4]     = galoisX14[a[0]] ^ galoisX9[a[3]] ^ galoisX13[a[2]] ^ galoisX11[a[1]]; /* 14 * a0 + 9 * a3 + 13 * a2 + 11 * a1 */
        myB[i*4 + 1] = galoisX14[a[1]] ^ galoisX9[a[0]] ^ galoisX13[a[3]] ^ galoisX11[a[2]]; /* 14 * a1 + 9 * a0 + 13 * a3 + 11 * a2 */
        myB[i*4 + 2] = galoisX14[a[2]] ^ galoisX9[a[1]] ^ galoisX13[a[0]] ^ galoisX11[a[3]]; /* 14 * a2 + 9 * a1 + 13 * a0 + 11 * a3 */
        myB[i*4 + 3] = galoisX14[a[3]] ^ galoisX9[a[2]] ^ galoisX13[a[1]] ^ galoisX11[a[0]]; /* 14 * a3 + 9 * a2 + 13 * a1 + 11 * a0 */
    }
}

/**
* addroundkey
*/
void Rijndael::addroundkey(unsigned char *myB, unsigned char *myK, unsigned char ronda){
    //cout << "addroundkey" << endl;
    unsigned char i;
    unsigned char elemsRonda = Nb * 4;

    for (i = 0; i < elemsRonda; i++){
         myB[i] =  myB[i] ^ myK[(ronda * elemsRonda) + i];
         ////cout << hex << showbase << (int) b[i] << " XOR " <<  (int)k[(ronda * 16) + i] << " = " << (int)b[i] << endl;
    }
}

/**
* printKey
*/
void Rijndael::printKey(unsigned char *data){
    cout << "[" ;
    unsigned char i;

    for (i=0;i<176;i++){
        if (i % 16 == 0 && i > 0) cout << endl;
        cout << hex << showbase << (int)data[i];
        if (i < 175) cout << ",";
    }
    cout << "]" << endl ;
}

/**
* print
*/
void Rijndael::print(unsigned char *data){
    cout << "[" ;
    unsigned char i;
    for (i = 0; i < 16; i++){
       cout << hex << showbase << (int)data[i];
       if (i < 15) cout << ",";
    }
    cout << "]" << endl ;
}
