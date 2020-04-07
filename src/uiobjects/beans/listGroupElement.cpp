/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   listGroupElement.cpp
 * Author: Ryuk
 * 
 * Created on 4 de abril de 2020, 14:26
 */

#include "listGroupElement.h"

ListGroupElement::ListGroupElement() {
    uipicture = NULL;
}

ListGroupElement::~ListGroupElement() {
}


void ListGroupElement::SetUipicture(UIPicture* uipicture) {
    this->uipicture = uipicture;
}

UIPicture* ListGroupElement::GetUipicture() {
    return uipicture;
}

void ListGroupElement::SetListGroupCol(vector<ListGroupCol*> listGroupCol) {
    this->listGroupCol = listGroupCol;
}

vector<ListGroupCol*> ListGroupElement::GetListGroupCol() {
    return listGroupCol;
}

