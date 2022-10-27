#ifndef HTMLPARSER_H
#define HTMLPARSER_H


#include "uiobjects/Constant.h"
#include <gumbo.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <vector>

#ifdef WIN
    #ifndef WINVER
        #define WINVER 0x0501
    #endif
//  global compilation flag configuring windows sdk headers
    //  preventing inclusion of min and max macros clashing with <limits>
    #define NOMINMAX 1
    //  override byte to prevent clashes with <cstddef>
    #define byte win_byte_override
    #include <winsock2.h>
    #include <windows.h>
    //#undef byte
#endif

#include <tidy.h>
#include <buffio.h>


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

        void buscarElem2TagValue(char *html, Tags *tag1, string atributoElem1, string valAtributoElem1
                               , Tags *tag2, string atributoElem2);

        map<string, string> GetMapKeyValueFound() const;


    protected:
    private:
        string search_text(GumboNode* node, Tags *tag);
        void search_for_links(GumboNode* node, Tags *);
        std::string cleantext(GumboNode* node);
        bool obtainContentTag;
        vector <string> listAttrFound;
        map<string, string> mapKeyValueFound;

        GumboNode* search_for_elem_key_value(GumboNode* node, Tags *tag, string atributo, string valAtributo,
                                        Tags *tag2, string atributoElem2);

};

#endif // HTMLPARSER_H
