/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UITreeListBox.h
 * Author: dmarcobo
 *
 * Created on 9 de noviembre de 2017, 12:37
 */

#ifndef UITREELISTBOX_H
#define UITREELISTBOX_H

#include "uilistcommon.h"


// Función de prueba para recorridos del árbol
//template <class DATO> void Mostrar(DATO &d){
//    cout << d << ",";
//}

class TreeNode{
    public:
        /**
         * Default constructor
         */
        TreeNode(){
            ico = -1;
            dest = -1;
            id="";
            text="";
            value="";
            show = false;
            estado = -1;
            realPos = -1;
        }
        
        /**
         * Constructor
         * @param vid
         * @param vtext
         * @param vvalue
         * @param vico
         * @param vdest
         * @param vshow
         * @param vestado
         */
        TreeNode(string vid, string vtext, string vvalue, int vico, int vdest, int vshow, int vestado){
            id=vid;
            text=vtext;
            value=vvalue;
            ico=vico;
            dest=vdest;
            show=vshow;
            realPos = -1;
            estado = vestado;
        }
        
        /**
         * Constructor
         * @param vid
         * @param vtext
         * @param vvalue
         * @param vico
         * @param vdest
         * @param vshow
         */
        TreeNode(string vid, string vtext, string vvalue, int vico, int vdest, int vshow){
            id=vid;
            text=vtext;
            value=vvalue;
            ico=vico;
            dest=vdest;
            show=vshow;
            realPos = -1;
            estado = -1;
        }
        
        
        
        string id;      //id que identifica a la rama y sus hojas
        string text;    //Texto que se muestra al usuario
        string value;   //Valor que se almacena
        int ico;        //Icono que se muestra en la lista al usuario
        int dest;       //Destino 
        bool show;      //Indica si se esta mostrando el elemento en pantalla
        int estado;     //Para saber el estado del nodo en cualquier implementacion
        int realPos;    //Posicion real en la lista de, no en la posicion que se muestra al usuario.
                        //!!!SOLO SE INFORMA CUANDO SE RECUPERA MEDIANTE GET!!!!
    
        TreeNode &operator=(const TreeNode &c) {
            if(this != &c) {
                   id=c.id;
                   text=c.text;
                   value=c.value;
                   ico=c.ico;
                   dest=c.dest;
                   show=c.show;
                   estado=c.estado;
                   realPos=c.realPos;
            }
            return *this;
         }
        
        int comparar(const TreeNode &c) const {
            return id.compare(c.id);
        }

        bool operator==(const TreeNode &c) const {
            return comparar(c) == 0;
        }
        bool operator!=(const TreeNode &c) const {
            return comparar(c) != 0;
        }
        bool operator<(const TreeNode &c) const {
            return comparar(c) < 0;
        }
        bool operator>(const TreeNode &c) const {
            return comparar(c) > 0;
        }
        bool operator<=(const TreeNode &c) const {
            return comparar(c) <= 0;
        }
        bool operator>=(const TreeNode &c) const {
            return comparar(c) >= 0;
        }
};

class UITreeListBox : public UIListCommon{
public:
    UITreeListBox();
    virtual ~UITreeListBox();  
    
    void mostrar();
    void action(tEvento *evento);
    TreeNode get(int row);
    string getValue(int row);
    int getDestino(int row);
    unsigned int getSize();
    void sort();
    void add(string vid, string vtext, string vvalue, int vico, int vdest, int vshow);
    void add(string vid, string vtext, string vvalue, int vico, int vdest, int vshow, int vestado);
    void refreshNode(TreeNode tmpNode);
    void clearLista();
    
private:
    void unfold(listaSimple<TreeNode> *lista, string id, bool unfold);
    listaSimple<TreeNode> lista;
    
};

#endif /* UITREELISTBOX_H */

