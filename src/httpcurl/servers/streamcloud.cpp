#include "streamcloud.h"

Streamcloud::Streamcloud()
{
    //ctor
}

Streamcloud::~Streamcloud()
{
    //dtor
}

/**
//*********************************************************************************************************
//      Hay que buscar estos elementos en streamcloud
//*********************************************************************************************************
//    <input type="hidden" name="op" value="download1">
//    <input type="hidden" name="usr_login" value="">
//    <input type="hidden" name="id" value="512zjmlchbr7">
//    <input type="hidden" name="fname" value="bre501es.avi">
//    <input type="hidden" name="referer" value="">
//    <input type="hidden" name="hash" value="">
//    <input type="submit" name="imhuman" id="btn_download" class="button gray" value="Watch video now">
//*********************************************************************************************************
*/
string Streamcloud::download(string url){
    HttpUtil util;

    if (util.download(url)){
        //util.writeToFile("C:\\streamcloud_512zjmlchbr7.htm");
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
        const char *names[] = {"op","usr_login","id","fname","referer", "hash", "imhuman"};
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
        Constant::waitms(10000);
        map<string, string> v_post;
        v_post.insert( make_pair("Referer", url));
        util.post(url, postData, &v_post);
        //util.writeToFile("C:\\streamcloud_512zjmlchbr7_POST.htm");
        tag.htmlTag = GUMBO_TAG_SCRIPT;
        //El video final esta en un campo llamado file: "URL" dentro de <script>
        string script = parser.buscarTextoInTag(util.getData(), &tag);
        string urlvideo = parser.searchString(&script, "file: ", ",", false);
        return urlvideo;
    }
    return "";
}
