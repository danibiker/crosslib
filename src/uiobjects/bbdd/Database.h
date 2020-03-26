#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <sqlite3.h>
#include "Excepcion.h"
#include "Dirutil.h"
#include "tinyxml/tinyxml.h"


#include <string>
#include <vector>
#include <iostream>

class Database
{
    public:
        Database(const char* filename);
        ~Database();
        void close();
        bool execute(string);
        bool executeNoCommit(string);
        vector<vector<string> > query(string query);
        sqlite3 *getBbdd(){return database;}

        //Nuevos metodos para generar querys
        void prepareStatement(string);
        void setString(unsigned int pos, string var);
        void setInt(unsigned int pos, int var);
        void setRaw(unsigned int pos, string var);
        bool execute();
        bool executeNoCommit();
        vector<vector<string> > executeQuery();
        void setClauseWhere(bool var){clauseWhere = var;}
        bool getClauseWhere(){return clauseWhere;}
        string getColQuery(vector<vector<string> > *result, unsigned int col);

    private:
        sqlite3 *database;
        bool open(const char* filename);
        string scapeStr(string);
        listaSimple<DataList> *listaQuerys;
        void loadQuerys( TiXmlNode* pParent, listaSimple<DataList> *varElement);
        bool loadQuerys(string filename);
        bool generarQuery(string *query);
        vector<string> statementValues;
        DataList statementDataList;
        bool clauseWhere;
};

#endif
