#include "htmlparser.h"


HtmlParser::HtmlParser() {
    obtainContentTag = false;
}

HtmlParser::~HtmlParser() {
    //dtor
}

/**
*
*/
string HtmlParser::CleanHTML(const char* html) {
    // Initialize a Tidy document
    TidyDoc tidyDoc = tidyCreate();
    //TidyBuffer tidyOutputBuffer = {0};
    TidyBuffer tidyOutputBuffer;
    tidyBufInit(&tidyOutputBuffer);

    // Configure Tidy
    // The flags tell Tidy to output XML and disable showing warnings
    bool configSuccess = tidyOptSetBool(tidyDoc, TidyXmlOut, yes)
                         && tidyOptSetBool(tidyDoc, TidyQuiet, yes)
                         && tidyOptSetBool(tidyDoc, TidyNumEntities, yes)
                         && tidyOptSetBool(tidyDoc, TidyShowWarnings, no);

    int tidyResponseCode = -1;

    // Parse input
    if (configSuccess)
        tidyResponseCode = tidyParseString(tidyDoc, html);

    // Process HTML
    if (tidyResponseCode >= 0)
        tidyResponseCode = tidyCleanAndRepair(tidyDoc);

    // Output the HTML to our buffer
    if (tidyResponseCode >= 0)
        tidyResponseCode = tidySaveBuffer(tidyDoc, &tidyOutputBuffer);

    // Any errors from Tidy?
    if (tidyResponseCode < 0)
        throw ("Tidy encountered an error while parsing an HTML response. Tidy response code: " + tidyResponseCode);

    // Grab the result from the buffer and then free Tidy's memory
    std::string tidyResult = (char*)tidyOutputBuffer.bp;
    tidyBufFree(&tidyOutputBuffer);
    tidyRelease(tidyDoc);

    return tidyResult;
}

/**
*
*/
void HtmlParser::buscarElementos(char *html, Tags *tag) {
    GumboOutput* output = gumbo_parse(html);
    search_for_links(output->root, tag);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

/**
*
*/
string HtmlParser::buscarElem(char *html, Tags *tag, string atributo, string valAtributo) {
    GumboOutput* output = gumbo_parse(html);
    string ret = search_for_elem(output->root, tag, atributo, valAtributo);

    char *arr = new char[ret.length()+1];
    strcpy(arr, ret.c_str());
    Constant::utf8ascii(arr);
    ret = string(arr);
    delete [] arr;

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return ret;
}

/**
*
*/
void HtmlParser::buscarElem2(char *html, Tags *tag1, string atributoElem1, string valAtributoElem1
                               , Tags *tag2, string atributoElem2) {

    listAttrFound.clear();
    GumboOutput* output = gumbo_parse(html);
    search_for_elem2(output->root, tag1, atributoElem1, valAtributoElem1, tag2, atributoElem2);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

/**
*
*/
void HtmlParser::buscarElem2TagValue(char *html, Tags *tag1, string atributoElem1, string valAtributoElem1
                               , Tags *tag2, string atributoElem2) {

    mapKeyValueFound.clear();
    GumboOutput* output = gumbo_parse(html);
    search_for_elem_key_value(output->root, tag1, atributoElem1, valAtributoElem1, tag2, atributoElem2);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

map<string, string> HtmlParser::GetMapKeyValueFound() const {
    return mapKeyValueFound;
}

/**
*
*/
GumboNode* HtmlParser::search_for_elem2(GumboNode* node, Tags *tag, string atributo, string valAtributo,
                                        Tags *tag2, string atributoElem2)
    {

    if (node->type != GUMBO_NODE_ELEMENT) {
        return NULL;
    }

    GumboAttribute* attribute;
    if (node->v.element.tag == tag->htmlTag){
        map<string, string> attrElement;
        if ((attribute = gumbo_get_attribute(&node->v.element.attributes, tag->attr.c_str()))) {
            if (string(attribute->value).compare(valAtributo) == 0 &&
                 (string(attribute->name).compare(atributo) == 0 || valAtributo.empty())
                    ){

                    if (string(attribute->name).compare(atributoElem2) == 0){
//                        cout << attribute->name << ":2 " << attribute->value << endl;
                        listAttrFound.push_back(attribute->value);
                    }

                    GumboVector* children = &node->v.element.children;
                    for (unsigned int j=0; j < children->length; j++){
                        GumboNode* enlace = search_for_elem2(static_cast<GumboNode*>(children->data[j]), tag2, atributoElem2, "", tag2, atributoElem2);
                        return enlace;
                    }
            }
        } else {
            //Recogemos el resto de atributos y los incluimos en la variable tag
            for (unsigned int i=0; i<node->v.element.attributes.length; i++){
                attribute = ((GumboAttribute *)node->v.element.attributes.data[i]);
                if (string(attribute->name).compare(atributo) == 0 &&
                     (string(attribute->value).compare(valAtributo) == 0 || valAtributo.empty())
                    ){

                    if (string(attribute->name).compare(atributoElem2) == 0){
                        listAttrFound.push_back(attribute->value);
//                        cout << attribute->name << ":1 " << attribute->value << " :2 " << cleantext(static_cast<GumboNode*>(node->v.element.children.data[0])) << endl ;
                    }

                    GumboVector* children = &node->v.element.children;
                    for (unsigned int j=0; j < children->length; j++){
                        GumboNode* enlace = search_for_elem2(static_cast<GumboNode*>(children->data[j]), tag2, atributoElem2, "", tag2, atributoElem2);
                        if (enlace != NULL){
                            return enlace;
                        }
                    }
                }
            }
        }
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        GumboNode* ret = search_for_elem2(static_cast<GumboNode*>(children->data[i]), tag, atributo, valAtributo, tag2, atributoElem2);
        if (ret != NULL){
            return ret;
        }
    }

    return NULL;
}

/**
*
*/
GumboNode* HtmlParser::search_for_elem_key_value(GumboNode* node, Tags *tag, string atributo, string valAtributo,
                                        Tags *tag2, string atributoElem2)
    {

    if (node->type != GUMBO_NODE_ELEMENT) {
        return NULL;
    }

    GumboAttribute* attribute;
    if (node->v.element.tag == tag->htmlTag){
        map<string, string> attrElement;
        if ((attribute = gumbo_get_attribute(&node->v.element.attributes, tag->attr.c_str()))) {
            if (string(attribute->value).compare(valAtributo) == 0 &&
                 (string(attribute->name).compare(atributo) == 0 || valAtributo.empty())
                    ){

                    GumboVector* children = &node->v.element.children;
                    if (string(attribute->name).compare(atributoElem2) == 0){
                        string text = children->length > 0 ? cleantext(static_cast<GumboNode*>(children->data[0])) : string("");
//                        cout << attribute->name << ":2 " << attribute->value << " text: " << text << endl;
                        mapKeyValueFound.insert(std::pair<string,string>(attribute->value, text));
                    }


                    for (unsigned int j=0; j < children->length; j++){
                        GumboNode* enlace = search_for_elem_key_value(static_cast<GumboNode*>(children->data[j]), tag2, atributoElem2, "", tag2, atributoElem2);
                        return enlace;
                    }
            }
        } else {
            //Recogemos el resto de atributos y los incluimos en la variable tag
            for (unsigned int i=0; i<node->v.element.attributes.length; i++){
                attribute = ((GumboAttribute *)node->v.element.attributes.data[i]);
                if (string(attribute->name).compare(atributo) == 0 &&
                     (string(attribute->value).compare(valAtributo) == 0 || valAtributo.empty())
                    ){

                    GumboVector* children = &node->v.element.children;

                    if (string(attribute->name).compare(atributoElem2) == 0){
                        string text = children->length > 0 ? cleantext(static_cast<GumboNode*>(node->v.element.children.data[0])) : "";
//                        cout << attribute->name << ":1 " << attribute->value << " text: " << text << endl;
                        mapKeyValueFound.insert(std::pair<string,string>(attribute->value, text));
                    }


                    for (unsigned int j=0; j < children->length; j++){
                        GumboNode* enlace = search_for_elem_key_value(static_cast<GumboNode*>(children->data[j]), tag2, atributoElem2, "", tag2, atributoElem2);
                        if (enlace != NULL){
                            return enlace;
                        }
                    }
                }
            }
        }
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        GumboNode* ret = search_for_elem_key_value(static_cast<GumboNode*>(children->data[i]), tag, atributo, valAtributo, tag2, atributoElem2);
        if (ret != NULL){
            return ret;
        }
    }

    return NULL;
}

/**
*
*/
string HtmlParser::search_for_elem(GumboNode* node, Tags *tag, string atributo, string valAtributo) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return "";
    }

    GumboAttribute* attribute;
    if (node->v.element.tag == tag->htmlTag){
        map<string, string> attrElement;
        if ((attribute = gumbo_get_attribute(&node->v.element.attributes, tag->attr.c_str()))) {
            if (string(attribute->value).compare(valAtributo) == 0
                    && string(attribute->name).compare(atributo) == 0){
                    return cleantext(node);
            }
        } else {
            //Recogemos el resto de atributos y los incluimos en la variable tag
            for (unsigned int i=0; i<node->v.element.attributes.length; i++){
                attribute = ((GumboAttribute *)node->v.element.attributes.data[i]);
                if (string(attribute->value).compare(valAtributo) == 0
                    && string(attribute->name).compare(atributo) == 0){
                    return cleantext(node);
                }
            }
        }
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        string ret = search_for_elem(static_cast<GumboNode*>(children->data[i]), tag, atributo, valAtributo);
        if (!ret.empty()){
            return ret;
        }
    }
    return "";
}

/**
*
*/
std::string HtmlParser::cleantext(GumboNode* node) {
  if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT
             && node->v.element.tag != GUMBO_TAG_SCRIPT
             && node->v.element.tag != GUMBO_TAG_STYLE
             ) {

//        if (node->v.element.tag == GUMBO_TAG_A){
//            GumboAttribute* attribute;
//            attribute = ((GumboAttribute *)node->v.element.attributes.data[0]);
//            cout << node->v.element.tag << "," << attribute->name << ":" << attribute->value << endl;
//        }

        std::string contents = "";
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = cleantext((GumboNode*) children->data[i]);
            if (i != 0 && !text.empty()) {
                contents.append("\n");
            }
            contents.append(text);
        }
        return contents;
  } else {
    return "";
  }
}

/**
*
*/
string HtmlParser::buscarTextoInTag(char *html, Tags *tag) {

    GumboOutput* output = gumbo_parse(html);
    string res = search_text(output->root, tag);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return res;
}

/**
*
*/
void HtmlParser::search_for_links(GumboNode* node, Tags *tag) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    GumboAttribute* attribute;

    if (node->v.element.tag == tag->htmlTag){
        map<string, string> attrList;
        attrValue attrElement;

        if ( (attribute = gumbo_get_attribute(&node->v.element.attributes, tag->attr.c_str()))) {
            attrElement.attrList.insert( make_pair(attribute->name, attribute->value));
        } else {
            //Recogemos el resto de atributos y los incluimos en la variable tag
            for (unsigned int i=0; i<node->v.element.attributes.length; i++){
                attribute = ((GumboAttribute *)node->v.element.attributes.data[i]);
                attrElement.attrList.insert( make_pair(attribute->name, attribute->value));
            }
        }
        attrElement.content = isObtainContentTag() ? search_text(node, tag) : "";
        tag->tagElement.push_back(attrElement);
    }

    GumboVector* children = &node->v.element.children;
//    std::cout << "****** elementos: " << children->length << endl;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_links(static_cast<GumboNode*>(children->data[i]), tag);
    }
}

/**
*
*/
string HtmlParser::search_text(GumboNode* node, Tags *tag) {
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
    std::string contents = "";
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
      const std::string text = search_text((GumboNode*) children->data[i], tag);
      if (i != 0 && !text.empty()) {
        contents.append(" ");
      }
      contents.append(text);
    }
    return contents;
  } else {
    return "";
  }
}

/**
*
*/
string HtmlParser::searchString(string *data, string toFind, string endStr, bool reverseFind){
    if (data->find(toFind) != string::npos){
        int pos = ((reverseFind) ? data->rfind(toFind) : data->find(toFind));
        if (data->substr(pos).find(endStr) != string::npos){
            int end = data->substr(pos).find(endStr);
            return data->substr(pos + toFind.length(), end - toFind.length());
        }
    }
    return "";
}
