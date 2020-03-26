#ifndef UICOMBOBOX_H
#define UICOMBOBOX_H

#include "uilist.h"

class UIComboBox : public UIList
{
    public:
        UIComboBox();
        virtual ~UIComboBox();
        void checkPos(tEvento evento);
        void action(tEvento *);
        string getSelectedValue();
        bool isValueChanged(){return valueChanged;}
        void setValueChanged(bool var){valueChanged = var;}
        int getH(){
            return isChecked() ? Constant::getCOMBOLISTHEIGHT() : COMBOHEIGHT;
        }
    protected:
    private:
        bool valueChanged;
};

#endif // UICOMBOBOX_H
