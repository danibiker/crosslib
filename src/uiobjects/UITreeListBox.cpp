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
#include "ListaSimple.h"

/**
 *
 */
UITreeListBox::UITreeListBox() {
   inicializarObjeto(GUITREELISTBOX);
}

/**
 *
 */
UITreeListBox::~UITreeListBox() {
}

/**
 *
 * @param evento
 */
void UITreeListBox::action(tEvento *evento){
    static unsigned long lastClick = 0;

    //Si hay un popup lanzado por este elemento, debe dejar de procesar los eventos
    if (popup == false){
        if (evento->isMouse && evento->mouse == MOUSE_BUTTON_LEFT && evento->mouse_state == SDL_PRESSED){
           Traza::print("UITreeListBox::action: Mouse Pressed: " + this->getName(), W_DEBUG);
           if (SDL_GetTicks() - lastClick < DBLCLICKSPEED){
               lastSelectedPos = this->posActualLista;
               lastClick = SDL_GetTicks() - DBLCLICKSPEED;  //reseteo del dobleclick
               TreeNode node = get(this->posActualLista);
               Traza::print("UITreeListBox::action: Double Click: " + node.id + (node.show ? " S" : " N"), W_DEBUG);
               unfold(&lista, node.id, node.show);
            } else {
                lastClick = SDL_GetTicks();
            }
           checkPos(*evento);
           this->setImgDrawed(false);
        } else if ( (evento->key == SDLK_RETURN && !(evento->keyMod & KMOD_LALT)) || evento->joy == JoyMapper::getJoyMapper(JOY_BUTTON_A)){
            lastSelectedPos = this->posActualLista;
            TreeNode node = get(this->posActualLista);
            Traza::print("UITreeListBox::action: return: " + node.id + (node.show ? " S" : " N"), W_DEBUG);
            unfold(&lista, node.id, node.show);
            this->setImgDrawed(false);
        } else {
            //Traza::print("UIListGroup::action: Pasando al padre: " + this->getName(), W_DEBUG);
            UIListCommon::action(evento);
        }
    }
}

/**
 *
 * @param lista
 * @param id
 * @param unfold
 */
void UITreeListBox::unfold(listaSimple<TreeNode> *lista, string id, bool unfold){
    //Recorremos al reves para saber si la rama tiene hojas
    int i = lista->getSize() - 1;
    string tmpId = "";
    string cut = "";
    bool salir = false;
    int subnivelPos = 0;

    while (i >= 0 && !salir){
        tmpId = lista->get(i).id;
        cut = tmpId.substr(0,id.length());
        subnivelPos = tmpId.find_last_of(".");

        if (cut.compare(id) == 0 && tmpId.compare(id) != 0){
            //tieneHojas = true;
            if (lista->get(i).show || (!lista->get(i).show && tmpId.substr(0,subnivelPos).compare(id) == 0)){
                TreeNode *tmpNode = new TreeNode();
                *tmpNode = lista->get(i);
                tmpNode->show = !tmpNode->show;
                lista->set(i, tmpNode);
                Traza::print("Marcando: " + lista->get(i).id + (lista->get(i).show ? " S" : " N"), W_DEBUG);
            }
        }
        i--;
    }

    calcularScrPos();
}

/**
 */
void UITreeListBox::mostrar(){
    size_t pos = 0;
    for (unsigned int i=0; i < lista.getSize(); i++){
       pos = lista.get(i).id.find_last_of(".");
       if (pos == string::npos){
           //parent = lista.get(i).id;
           cout << lista.get(i).id << " es un raiz."<< endl;
       } else if (lista.get(i).show){
           cout << lista.get(i).id <<endl;
       }
   }
}

/**
 *
 * @param row
 * @return
 */
TreeNode UITreeListBox::get(int row){
    unsigned int i = 0;
    int posLista = -1;
    string tmpId = "";
    size_t cut = 0;
    size_t posFound = 0;

    if (lista.getSize() > 0)
    while (i < lista.getSize() && posLista < row){
        tmpId = lista.get(i).id;
        cut = tmpId.find_last_of(".");
        if (lista.get(i).show == true || cut == string::npos){
            posLista++;
            posFound = i;
        }
        i++;
    }

    if (posFound < lista.getSize()){
        TreeNode res = lista.get(posFound);
        res.realPos = posFound;
        return res;
    } else
        throw(Excepcion(ENULL, "UITreeListBox: Columna vacia"));
}

/**
 *
 * @param realrow
 * @param estado
 */
void UITreeListBox::refreshNode(TreeNode tmpNode){
    lista.set(tmpNode.realPos, &tmpNode);
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
string UITreeListBox::getDestino(int row){
    return get(row).dest;
}

/**
 *
 * @return
 */
unsigned int UITreeListBox::getSize(){
    unsigned int i = 0;
    unsigned int nElems = 0;
    string tmpId = "";
    size_t cut = 0;

    while (i < lista.getSize()){
        tmpId = lista.get(i).id;
        cut = tmpId.find_last_of(".");
        if (lista.get(i).show == true || cut == string::npos){
            nElems++;
        }
        i++;
    }
    return nElems;
}

/**
 *
 */
void UITreeListBox::sort(){
    this->lista.sort();
}

/**
 *
 * @param vid
 * @param vtext
 * @param vvalue
 * @param vico
 * @param vdest
 * @param vshow
 */
void UITreeListBox::add(string vid, string vtext, string vvalue, int vico, int vdest, int vshow){
    this->lista.add(TreeNode(vid, vtext, vvalue, vico, vdest, vshow));
}

void UITreeListBox::add(string vid, string vtext, string vvalue, int vico, int vdest, int vshow, int vestado){
    this->lista.add(TreeNode(vid, vtext, vvalue, vico, vdest, vshow, vestado));
}

void UITreeListBox::clearLista(){
    this->lista.clear();
    UIListCommon::clearLista();
}
