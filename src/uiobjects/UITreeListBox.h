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
        TreeNode(){
            ico = -1;
            dest = -1;
            id="";
            text="";
            value="";
            show = false;
        }
        
        string id;
        string text;
        string value;
        int ico;
        int dest;
        bool show;
    
        TreeNode &operator=(const TreeNode &c) {
            if(this != &c) {
                   id=c.id;
                   text=c.text;
                   value=c.value;
                   ico=c.ico;
                   dest=c.dest;
                   show=c.show;
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
    void unfold(listaSimple<TreeNode> *lista, string id, bool unfold);
    void mostrar();
    listaSimple<TreeNode> lista;
    
    TreeNode get(int row);
    string getValue(int row);
    int getDestino(int row);
    
    
private:
};

#endif /* UITREELISTBOX_H */

