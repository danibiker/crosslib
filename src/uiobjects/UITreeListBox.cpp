/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UITreeListBox.cpp
 * Author: dmarcobo
 * 
 * Created on 9 de noviembre de 2017, 12:37
 */

#include "UITreeListBox.h"
#include "Constant.h"
#include "ArbolAAB.h"
#include "ListaSimple.h"


void UITreeListBox::unfold(listaSimple<TreeNode> *lista, string id, bool unfold){
    int i = 0;
    
    string tmpId = "";
    size_t cut = 0;
    bool salir = false;
    
    do{
        tmpId = lista->get(i).id;
        cut = tmpId.find_last_of(".");
        if (cut != string::npos){
            if (tmpId.substr(0,cut).compare(id) == 0){
                (&lista->get(i))->show = true;
                TreeNode *tmpNode = new TreeNode();
                *tmpNode = lista->get(i);
                tmpNode->show = unfold;
                lista->set(i, tmpNode);
                cout << "Marcando " << lista->get(i).id << (lista->get(i).show ? "S" : "N") << endl;
            } 
        }
        i++;
    } while (i < lista->getSize() && !salir);
}

/**
 */
void UITreeListBox::mostrar(){
    int pos = 0;   
    for (int i=0; i < lista.getSize(); i++){
       pos = lista.get(i).id.find_last_of(".");
       if (pos == string::npos){
           //parent = lista.get(i).id;
           cout << lista.get(i).id << " es un raiz."<< endl;
       } else if (lista.get(i).show){
           cout << lista.get(i).id <<endl;
       }
   }   
}

UITreeListBox::UITreeListBox() {
   inicializarObjeto(GUITREELISTBOX);
    
   TreeNode node;
   node.id = "1";
   lista.add(node);
   node.id = "2";
   lista.add(node);
   node.id = "3";
   lista.add(node);
   node.id = "4";
   lista.add(node);
   node.id = "2.1";
   lista.add(node);
   node.id = "2.2";
   lista.add(node);
   node.id = "4.1";
   lista.add(node);
   node.id = "4.3";
   lista.add(node);
   node.id = "4.2";
   lista.add(node);
   node.id = "4.2.1";
   lista.add(node);
   node.id = "4.2.1.1";
   lista.add(node);
   node.id = "4.2.1.2";
   lista.add(node);
   lista.sort();
   
//   string parent = "";
//   size_t pos = 0;
//   
//   unfold(&lista, "4", true);
//   mostrar();
//   unfold(&lista, "4", false);
//   mostrar();
//   
////   unfold(&lista, "4", true);
//   unfold(&lista, "4.2.1", true);
//   mostrar();
   // Un árbol de enteros
   /*ABB<int> ArbolInt;

   // Inserción de nodos en árbol:
   ArbolInt.Insertar(10);
   ArbolInt.Insertar(5);
   ArbolInt.Insertar(12);
   ArbolInt.Insertar(4);
   ArbolInt.Insertar(7);
   ArbolInt.Insertar(3);
   ArbolInt.Insertar(6);
   ArbolInt.Insertar(9);
   ArbolInt.Insertar(8);
   ArbolInt.Insertar(11);
   ArbolInt.Insertar(14);
   ArbolInt.Insertar(13);
   ArbolInt.Insertar(2);
   ArbolInt.Insertar(1);
   ArbolInt.Insertar(15);
   ArbolInt.Insertar(10);
   ArbolInt.Insertar(17);
   ArbolInt.Insertar(18);
   ArbolInt.Insertar(16);

   cout << "Altura de arbol " << ArbolInt.AlturaArbol() << endl;

   // Mostrar el árbol en tres ordenes distintos:
   cout << "InOrden: ";
   ArbolInt.InOrden(Mostrar);
   cout << endl;
   cout << "PreOrden: ";
   ArbolInt.PreOrden(Mostrar);
   cout << endl;
   cout << "PostOrden: ";
   ArbolInt.PostOrden(Mostrar);
   cout << endl;

   // Borraremos algunos elementos:
   cout << "N nodos: " << ArbolInt.NumeroNodos() << endl;
   ArbolInt.Borrar(5);
   cout << "Borrar   5: ";
   ArbolInt.InOrden(Mostrar);
   cout << endl;
   ArbolInt.Borrar(8);
   cout << "Borrar   8: ";
   ArbolInt.InOrden(Mostrar);
   cout << endl;
   ArbolInt.Borrar(15);
   cout << "Borrar  15: ";
   ArbolInt.InOrden(Mostrar);
   cout << endl;
   ArbolInt.Borrar(245);
   cout << "Borrar 245: ";
   ArbolInt.InOrden(Mostrar);
   cout << endl;
   ArbolInt.Borrar(4);
   cout << "Borrar   4: ";
   ArbolInt.InOrden(Mostrar);
   ArbolInt.Borrar(17);
   cout << endl;
   cout << "Borrar  17: ";
   ArbolInt.InOrden(Mostrar);
   cout << endl;

   // Veamos algunos parámetros
   cout << "N nodos: " << ArbolInt.NumeroNodos() << endl;
   cout << "Altura de 1 " << ArbolInt.Altura(1) << endl;
   cout << "Altura de 10 " << ArbolInt.Altura(10) << endl;
   cout << "Altura de arbol " << ArbolInt.AlturaArbol() << endl;
    */
 
}

/**
 * 
 */
UITreeListBox::~UITreeListBox() {
}

/**
 * 
 * @param row
 * @return 
 */
TreeNode UITreeListBox::get(int row){
    int i = 0;
    int posLista = 0;
    do{
        if (lista.get(i).show == true){
            posLista++;
        }
        i++;
    } while (i < lista.getSize() && posLista > row);
    if (i<lista.getSize())
        return lista.get(i);
    else 
        throw(Excepcion(ENULL, "UITreeListBox: Columna vacia"));
}
/**
 * 
 * @param row
 * @return 
 */
string UITreeListBox::getValue(int row){
    return get(row).value;
}

/**
 * 
 * @param row
 * @return 
 */
int UITreeListBox::getDestino(int row){
    return get(row).dest;
}