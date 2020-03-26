#ifndef UITEXTELEMENTSAREA_H
#define UITEXTELEMENTSAREA_H

#include "object.h"

struct t_element_style{
    t_posicion pos;
    int fontSize = FONTSIZE;
    bool bold = false;
};

class TextElement{
    public :
        TextElement(){
            name = "";
            text = "";
            label = "";
            style.pos.x = 0;
            style.pos.y = 0;
            style.pos.w = 0;
            style.pos.h = 0;
            useMaxLabelMargin = false;
            url = "";
            ico = -1;
        }
        ~TextElement(){}
        
        TextElement(const TextElement *var){
            name = var->name;
            text = var->text;
            label = var->label;
            style.pos.x = var->style.pos.x;
            style.pos.y = var->style.pos.y;
            style.pos.w = var->style.pos.w;
            style.pos.h = var->style.pos.h;
            style.fontSize = var->style.fontSize;
            style.bold = var->style.bold;
            useMaxLabelMargin = var->useMaxLabelMargin;
            url = var->url;
            ico = var->ico;
        }
        
        

        string getName(){ return name;}
        void setName(string var){ name = var;}
        string getLabel(){ return label;}
        void setLabel(string var){ label = var;}
        string getText(){ return text;}
        void setText(string var){ text = var;}
        string getUrl(){ return url;}
        void setUrl(string var){ url = var;}
        void setIco(int var){ico=var;}
        int getIco(){return ico;}


        t_posicion * getPos(){ return &style.pos;}
        void setPos(t_posicion var){ style.pos = var;}

        t_element_style * getStyle(){ return &style;}
        void setStyle(t_element_style var){ style = var;}


        bool isUseMaxLabelMargin(){ return useMaxLabelMargin;}
        void setUseMaxLabelMargin(bool var){ useMaxLabelMargin = var;}

protected :
    string name;
    string text;
    string label;
    t_element_style style;
    bool useMaxLabelMargin;
    string url;
    int ico;




};

class UITextElementsArea : public Object
{
    public:
        UITextElementsArea();
        virtual ~UITextElementsArea();
        vector <TextElement *> textVector;
        void addField(TextElement *textElem);
        
        void clear();
        vector <TextElement *> *getTextVector(){return &textVector;}
        void setFieldText(string name, string text);
        void setPosition(string name, t_posicion pos);
        void action(tEvento *evento);
        int getOffsetDesplazamiento(){return offsetDesplazamiento;}
        void setOffsetDesplazamiento(int var){offsetDesplazamiento = var;}
        void setIntervalDespl(int var){intervalDespl = var;}
        void setMaxOffsetY(int var){maxOffsetY = var;}
        int getMaxOffsetY(){return maxOffsetY;}
        int getSelectedPos(){return selectedPos;}

    protected:
    private:
        int offsetDesplazamiento;
        int intervalDespl;
        int maxOffsetY;
        int selectedPos;
};

#endif // UITEXTELEMENTSAREA_H
