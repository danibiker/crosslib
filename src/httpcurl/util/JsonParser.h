/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JsonParser.h
 * Author: Dani
 *
 * Created on 2 de febrero de 2022, 10:17
 */

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <json/json.h>
#include <iostream>

class JsonParser {
public:
    static bool parseJson(Json::Value *root, std::string raw_json, std::string *err);
    static bool parseJson(Json::Value *root, std::string raw_json);
    static bool parseJsonToString(std::string *raw_json, Json::Value value);
private:

};

#endif /* JSONPARSER_H */

