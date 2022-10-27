/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   XmlParser.cpp
 * Author: Dani
 *
 * Created on 5 de febrero de 2022, 0:21
 */

#include "XmlParser.h"

XmlParser::XmlParser() {
}

XmlParser::XmlParser(const XmlParser& orig) {
}

XmlParser::~XmlParser() {
}

/**
 *
 * @param data
 * @param output
 * @return
 */
bool XmlParser::parseString(string data, map<string, unique_ptr<vector<string>> > &output){
    GumboOutput* gumboOutput = gumbo_parse(data.c_str());
    if (gumboOutput != NULL){
        getNodeInfo(gumboOutput->root, output);
        return true;
    }
    return false;
}

/**
 *
 * @param data
 * @param output
 * @return
 */
vector<string> * XmlParser::getValues(string data, map<string, unique_ptr<vector<string>> > &output){
    auto elem = output.find(data);
    if (elem != output.end()){
        return elem->second.get();
    }
    return NULL;
}

/**
 *
 * @param data
 * @param output
 * @return
 */
string XmlParser::getFirst(string data, map<string, unique_ptr<vector<string>> > &output){
    string ret;
    auto elems = getValues(data, output);
    if (elems != NULL){
        return elems->at(0);
    }
    return ret;
}

/**
 *
 * @param node
 * @param output
 * @return
 */
string XmlParser::getNodeInfo(GumboNode* node, map<string, unique_ptr<vector<string>> > &output) {
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        string nombreTag = "";
        if (node->v.element.tag == GUMBO_TAG_UNKNOWN){
            nombreTag = getTagName(node->v.element.original_tag.data);
        }
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            const std::string text = getNodeInfo((GumboNode*) children->data[i], output);

            auto search = output.find(nombreTag);
            if (search != output.end()) {
                 search->second->push_back(text);
            } else {
                auto valuesForKey = std::make_unique<vector<string>>();
                valuesForKey->push_back(text);
                output.insert(std::make_pair(nombreTag, std::move(valuesForKey)));
            }
        }
    }
    return "";
}

/**
 *
 * @param data
 * @return
 */
string XmlParser::getTagName(const char* data) {

    int len = strlen(data);
    int i=0;
//    int ini = 0;
    int fin = 0;
    string tagName = "";
    char tmp[2] = {' ','\0'};

    while(i < len && fin == 0){
//        if (data[i] == '<')
//            ini = i;
//        else
        if (data[i] == '>')
            fin = i;
        else {
            tmp[0] = data[i];
            tagName.append(tmp);
        }
        i++;
    }
    return tagName;
}

