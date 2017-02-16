#ifndef UITEXTELEMENTSAREA_H
#define UITEXTELEMENTSAREA_H

#include <object.h>

struct t_element_style{
    t_posicion pos;
    int fontSize;
    bool bold;
};

class TextElement{
    protected :
        string name;
        string text;
        string label;
        t_element_style style;
        bool useMaxLabelMargin;
        string url;


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
        }

        TextElement(string name, string label, string text, t_posicion pos, bool useMaxLabelMargin ){
            this->name = name;
            this->text = text;
            this->label = label;
            this->useMaxLabelMargin = useMaxLabelMargin;
            this->style.pos = pos;
            this->style.fontSize = FONTSIZE;
            this->style.bold = false;
        }

        TextElement(string name, string label, string text, t_element_style style, bool useMaxLabelMargin ){
            this->name = name;
            this->text = text;
            this->label = label;
            this->useMaxLabelMargin = useMaxLabelMargin;
            this->style.pos = style.pos;
            this->style.fontSize = style.fontSize;
            this->style.bold = style.bold;
        }

        ~TextElement(){}

        string getName(){ return name;}
        void setName(string var){ name = var;}
        string getLabel(){ return label;}
        void setLabel(string var){ label = var;}
        string getText(){ return text;}
        void setText(string var){ text = var;}
        string getUrl(){ return url;}
        void setUrl(string var){ url = var;}


        t_posicion * getPos(){ return &style.pos;}
        void setPos(t_posicion var){ style.pos = var;}

        t_element_style * getStyle(){ return &style;}
        void setStyle(t_element_style var){ style = var;}


        bool isUseMaxLabelMargin(){ return useMaxLabelMargin;}
        void setUseMaxLabelMargin(bool var){ useMaxLabelMargin = var;}



};

class UITextElementsArea : public Object
{
    public:
        UITextElementsArea();
        virtual ~UITextElementsArea();
        vector <TextElement *> textVector;

        void addField(string name, string label, string text, t_posicion pos, bool useMaxLabelMargin );
        void addField(string name, string label, string text, t_element_style style, bool useMaxLabelMargin );
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
