/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   listGroupElement.h
 * Author: Ryuk
 *
 * Created on 4 de abril de 2020, 14:26
 */

#ifndef LISTGROUPELEMENT_H
#define LISTGROUPELEMENT_H

#include "uiobjects/beans/listgroupcol.h"
#include "uiobjects/uipicture.h"

class ListGroupElement {
public:
    ListGroupElement();
    virtual ~ListGroupElement();
    void SetUipicture(UIPicture* uipicture);
    UIPicture* GetUipicture();
    void SetListGroupCol(vector<ListGroupCol*> listGroupCol);
    vector<ListGroupCol*> GetListGroupCol();

private:
    vector <ListGroupCol *> listGroupCol;
    UIPicture *uipicture;

};

#endif /* LISTGROUPELEMENT_H */

