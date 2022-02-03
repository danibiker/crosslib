/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JsonParser.cpp
 * Author: Dani
 * 
 * Created on 2 de febrero de 2022, 10:17
 */

#include "JsonParser.h"

/**
 * 
 * @param root
 * @param raw_json
 * @param err
 * @return 
 */
bool JsonParser::parseJson(Json::Value *root, std::string raw_json, std::string *err){
    bool ret = false;
    try {
//        std::istringstream{str} >> *root;
//        ret = true;
        Json::CharReaderBuilder builder {};
        // Don't leak memory! Use std::unique_ptr!
        auto reader = std::unique_ptr<Json::CharReader>( builder.newCharReader() );
        
        std::string errors {};
        const auto is_parsed = reader->parse( raw_json.c_str(),
                                        raw_json.c_str() + raw_json.length(),
                                        root,
                                        &errors);
        ret = is_parsed;
        if (err != nullptr)
            *err = errors;
    } catch (const std::exception& e) {
        std::cerr << "Error parseando datos: " << e.what() << std::endl;
    }
    return ret;
}

/**
 * 
 * @param root
 * @param raw_json
 * @return 
 */
bool JsonParser::parseJson(Json::Value *root, std::string raw_json){
    return JsonParser::parseJson(root, raw_json, nullptr);
}

/**
 * 
 * @param raw_json
 * @param value
 * @return 
 */
bool JsonParser::parseJsonToString(std::string *raw_json, Json::Value value){
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ostringstream os;
    bool ret = writer->write(value, &os) == 0;
    *raw_json = os.str();
    return ret;
}