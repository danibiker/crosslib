/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   XmlParser.h
 * Author: Dani
 *
 * Created on 5 de febrero de 2022, 0:21
 */

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <memory>

#include <gumbo.h>

using namespace std;

class XmlParser {
public:
    XmlParser();
    XmlParser(const XmlParser& orig);
    virtual ~XmlParser();
    
    static bool parseString(string data, map<string, unique_ptr<vector<string>> > &output);
    static vector<string> * getValues(string data, map<string, unique_ptr<vector<string>> > &output);
    static string getFirst(string data, map<string, unique_ptr<vector<string>> > &output);
    
private:
    static string getNodeInfo(GumboNode* node, map<string, unique_ptr<vector<string>> > &output);
    static string getTagName(const char* data);
};

#endif /* XMLPARSER_H */

