#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <buffio.h>
#include <tidy.h>
#include <gumbo.h>
#include "Constant.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

struct attrValue{
    map<string, string> attrList;
    string content;
};


class Tags{
    public :
        Tags(){};
        virtual ~Tags(){};
        string attr;
        int htmlTag;
        vector<attrValue> tagElement;
};

class HtmlParser
{
    public:
        HtmlParser();
        virtual ~HtmlParser();
        string CleanHTML(const char *);
        void buscarElementos(char *, Tags *);
        string buscarTextoInTag(char *html, Tags *tag);
        string searchString(string *data, string toFind, string endStr, bool reverseFind);
        string search_for_elem(GumboNode* node, Tags *tag, string atributo, string valAtributo);
        string buscarElem(char *html, Tags *tag, string atributo, string valAtributo);
        bool isObtainContentTag(){return obtainContentTag;}
        void setObtainContentTag(bool var){obtainContentTag = var;}
        void buscarElem2(char *html, Tags *tag1, string atributoElem1, string valAtributoElem1, Tags *tag2, string atributoElem2);
        GumboNode* search_for_elem2(GumboNode* node, Tags *tag, string atributo, string valAtributo,Tags *tag2, string atributoElem2);
        vector <string> *getListAttrFound(){return &listAttrFound;}

    protected:
    private:
        string search_text(GumboNode* node, Tags *tag);
        void search_for_links(GumboNode* node, Tags *);
        std::string cleantext(GumboNode* node);
        bool obtainContentTag;
        vector <string> listAttrFound;

};

#endif // HTMLPARSER_H
