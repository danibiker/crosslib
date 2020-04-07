#ifndef UILISTGROUP_H
#define UILISTGROUP_H

#include "uilistcommon.h"
#include "beans/listgroupcol.h"
#include "beans/listGroupElement.h"
#include <algorithm>

class UIListGroup : public UIListCommon
{
    public:
        /** Default constructor */
        UIListGroup();
        /** Default destructor */
        virtual ~UIListGroup();

        unsigned int getSize();
        unsigned int getSizeCol(){return numCols;}

        ListGroupElement * getRow(unsigned int row);
        ListGroupCol * getCol(unsigned int row, unsigned int col);
        ListGroupCol * getHeaderCol(int col);


        void action(tEvento *);
        void draw();
        void checkPos(tEvento evento);
        unsigned int getElemVisibles();

        void addElemLista(vector <ListGroupCol *> newRow);
        void addElemLista(ListGroupElement * newElement);
        
        void setHeaderLista(vector <ListGroupCol *> newRow);
        void calcularScrPos();
        void adjustToHeader(bool var) {colsAdjustedToHeader = var;}

        void setHeaderWith(int col, int value);
        int getHeaderWith(unsigned int col);
        void clearHeaderWith(){headersWith.clear();}
        bool isColsAdjustedToHeader(){return colsAdjustedToHeader;}
        void addHeaderWith(int value){headersWith.push_back(value);}
        int sizeHeader(){return headersWith.size();}
        bool isColAnchorPressed(){return colAnchorPressed;}
        int getPosColAnchorPressed(){return posColAnchorPressed;}
        string getValue(int row);
        string getDestino(int row);
        void resizeLista(unsigned int tam);
        void clearLista();
        void sort(int col);
        bool searchTextInList(int key);
        int getColSelected(){return colSelected;}

    protected:
        vector <ListGroupElement *> listaAgrupada;
        vector <vector <ListGroupCol *> > listaHeaders;
        vector<int> headersWith;
        bool colsAdjustedToHeader;
        unsigned int numCols;
        bool colAnchorPressed;
        int posColAnchorPressed;
        int initMouseClick;
        int initColWith;
        int cursorInAnchor(tEvento *evento);
        int colSelected;

       int partition(vector <ListGroupElement *>& A, int p,int q, int col);
       void quickSort(vector <ListGroupElement *>& A, int p,int q, int col);

    private:
        void detectarAnchorClick(tEvento *evento);
};

#endif // UILISTGROUP_H
