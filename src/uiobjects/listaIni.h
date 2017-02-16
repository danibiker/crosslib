#ifndef LISTAINI_H_INCLUDED
#define LISTAINI_H_INCLUDED

#include "ListaSimple.h"




template<class TIPO>
class ListaIni : public listaSimple<TIPO>{

   public:
    ListaIni() : listaSimple<TIPO>(){};
    ListaIni(unsigned int a) : listaSimple<TIPO>(a){};
    void loadFromFile(string);
    void writeToFile(string ruta);
    void sort(); //El metodo sort solo funciona con una lista de strings
    int find(string);

   private:
    void QuickSort(nodoSimple<TIPO> *, int, int , int);
    int BuscarBin(nodoSimple<TIPO> *, int , int , string );

   protected:

};

template<class TIPO> void ListaIni<TIPO>::loadFromFile(string ruta){

    string str;
	ifstream infile;
	Fileio *fio = new Fileio();
    int nlines = fio->contarLineas(ruta.c_str());
    this->inicializar(nlines);
	infile.open (ruta.c_str());
	vector<string> x;

    if (infile.is_open()){
        while(!infile.eof()) {// To get you all the lines.
            TIPO linea;
            getline(infile,str); // Saves the line in STRING.
            if (!str.empty()){
                linea.addValue(str);
                this->add(linea);
            }
        }
        infile.close();

    } else {
         Traza::print("ListaIni.loadFromFile: Unable to open file:" + ruta, W_ERROR);
         infile.close();
         throw(Excepcion(EFIO));
    }
}

template<class TIPO> void ListaIni<TIPO>::writeToFile(string ruta){
    ofstream myfile (ruta.c_str());

    if (myfile.is_open()){
        for (unsigned int i = 0; i < this->getSize(); i++){
            myfile << this->get(i).getKey() << "=" << this->get(i).getValue() << VBCRLF;
        }
        myfile.close();
    } else {
         Traza::print("ListaIni.writeToFile: Unable to open file:" + ruta, W_ERROR);
         myfile.close();
         throw(Excepcion(EFIO));
    }
    #ifdef GP2X
        sync();
    #endif
}

/**
*
*/
template<class TIPO> void ListaIni<TIPO>::sort(){
    if (this->getSize() > 0)
    {
        QuickSort(this->plista, this->getSize()-1, 0, this->getSize()-1);
    }
}

/**
*
*/
template<class TIPO> void ListaIni<TIPO>::QuickSort(nodoSimple<TIPO> *A, int AHigh, int iLo, int iHi)
{
  int Lo, Hi;
  TIPO Mid;
  nodoSimple<TIPO> T;
  Lo = iLo;
  Hi = iHi;

  Mid = A[(Lo+Hi)/2].getValor();

  do
  {
    while (A[Lo].getValor().getKey().compare(Mid.getKey()) < 0)
        Lo++;
    while (A[Hi].getValor().getKey().compare(Mid.getKey()) > 0)
        Hi--;
    //free(Mid);
    if (Lo <= Hi)
    {
      T=A[Lo];
      A[Lo] = A[Hi];
      A[Hi]=T;
      Lo++;
      Hi--;
    }
  }
  while (Lo <= Hi);

  if (Hi > iLo)
    QuickSort(A, AHigh, iLo, Hi);
  if (Lo < iHi)
    QuickSort(A, AHigh, Lo, iHi);
}

/**
*
*/
template<class TIPO> int ListaIni<TIPO>::find(string palabra){
    if (this->getSize() > 0){
        return BuscarBin(this->plista,0, this->getSize() - 1, palabra);
    } else {
        return -1;
    }
}

/**
*
*/
template<class TIPO> int ListaIni<TIPO>::BuscarBin(nodoSimple<TIPO> *datos, int izq, int der, string palabra)
{
    bool encontrado=false;
    int centro=0,i=0;


    while ( (!encontrado) && (izq <= der) )
    {
        i++;
        centro=(izq+der)/2;
//        printf("centro: %d, i: %d, d: %d, %s\n",centro,izq,der,datos[centro].getValor().getKey().c_str());

        if (palabra.compare(datos[centro].getValor().getKey()) == 0)
        {
            encontrado=true;
            //printf("finded: %s %d\n",datos[centro].palabra,centro);
        }
        else
        {
            if (palabra.compare(datos[centro].getValor().getKey()) > 0)
                izq=centro+1;
            else
                der=centro-1;
        }
    }
    // El algoritmo de búsqueda binaria encuentra el número, pero no
    // asegura que sea el primero si hay repeticiones de números. Por eso
    // retrocedemos hasta el primer elemento
//     if (encontrado && centro > 0)
//     while(centro > 0 && palabra.compare(datos[centro-1].getValor().key) == 0)
//           centro--;

    if (encontrado) return (centro);
    else return(-1);
}


#endif // LISTAINI_H_INCLUDED
