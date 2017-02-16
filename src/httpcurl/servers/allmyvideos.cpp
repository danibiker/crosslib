#include "allmyvideos.h"

Allmyvideos::Allmyvideos()
{
    //ctor
}

Allmyvideos::~Allmyvideos()
{
    //dtor
}

/**
* Wrapper para bajar peliculas de allmyvideos
*/
string Allmyvideos::download(string url){
    HttpUtil util;

    if (util.download(url)){
        //util.writeToFile("C:\\allmyvideos.net_8cf8gd6ez70t.htm");
        HtmlParser parser;
        //cout << parser.CleanHTML(util.getData()) << endl;
        Tags tag;
        //tag.attr = "value";
        tag.htmlTag = GUMBO_TAG_INPUT;
        parser.buscarElementos(util.getData(), &tag);
        string name, value;
        map<string, string> *v;
        map<string, string> elements;
        //Se busca en la pagina todos los campos input y se recoge el valor de cada uno
        for (unsigned int i=0; i < tag.tagElement.size(); i++){
            v = &tag.tagElement.at(i).attrList;
            name = (v->find("name") !=  v->end()) ? v->find("name")->second : "";
            value = (v->find("value") !=  v->end()) ? v->find("value")->second : "";
            elements.insert( make_pair(name, value));
        }

        //Especificamos cuales son los valores de los input que nos interesan
        const char *names[] = {"op","usr_login","id","fname","referer", "method_free", "submitButton", "confirm"};
        int len = (sizeof(names) / sizeof(names[0]));
        //Generamos la informacion que ira en el POST
        string postData = "";
        for (int i=0; i < len; i++){
            name = names[i];
            value = (elements.find(names[i]) != elements.end()) ? elements.find(names[i])->second : "";
            value = Constant::replaceAll(value, " ","+");
            postData.append(name + "=" + value);
            if ( i < len - 1) postData.append("&");
        }
        //Esperamos 10s para simular el comportamiento de la pagina
        //Sleep(10000);
        map<string, string> v_post;
        v_post.insert( make_pair("Referer", url));
        util.post(url, postData, &v_post);
        //util.writeToFile("C:\\allmyvideos.net_8cf8gd6ez70t_POST.htm");
        tag.htmlTag = GUMBO_TAG_SCRIPT;
        //El video final esta en un campo llamado file: "URL" dentro de <script>
        string script = parser.buscarTextoInTag(util.getData(), &tag);
        string urlvideo = parser.searchString(&script, "\"file\" : ", ",", true);
        return urlvideo;
    }
    return "";
}
