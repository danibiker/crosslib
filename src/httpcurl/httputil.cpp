#include "httputil.h"
#include "util/ConstantHttp.h"
#include "Constant.h"

bool HttpUtil::aborted;
std::ifstream::pos_type HttpUtil::maxBytesDownload;
float HttpUtil::downloadProgress;

std::string HttpUtil::readBufferHeader;

/**
*
*/
HttpUtil::HttpUtil(){
    //cout << "HttpUtil: constructor" << endl;
    aborted = false;
    chunk.memory = NULL;
    chunk.filepath = NULL;
    chunk.size = 0;

    header.memory = NULL;
    header.filepath = NULL;
    header.size = 0;

    this->setProxyIP(Constant::getPROXYIP());
    this->setProxyPort(Constant::strToTipo<int>(Constant::getPROXYPORT()));
    this->setProxyUser(Constant::getPROXYUSER());
    this->setProxyPass(Constant::getPROXYPASS());
    this->setTimeout(0);
    this->setMaxBytesDownload(0);
    sendContentLength = true;
}

/**
*
*/
HttpUtil::~HttpUtil(){
    //cout << "HttpUtil: destructor" << endl;
    if(chunk.memory)
        free(chunk.memory);

    if(header.memory)
        free(header.memory);

    if(chunk.filepath)
        free(chunk.filepath);
}


/**
* Solo descarga una url en memoria
*/
bool HttpUtil::download(string url){
    return sendHttp(url, "", 0, 0, NULL, HTTP_GET);
}

/**
* Descarga una url al disco duro
*/
bool HttpUtil::download(string url, string pathfilename){
    return sendHttp(url, pathfilename.c_str(), 0, 0, NULL, HTTP_GET);
}

/**
* Descarga una url al disco duro pero utilizando cabeceras para la peticion http
*/
bool HttpUtil::download(string url, string pathfilename, map <string, string> *headers){
    return sendHttp(url, pathfilename.c_str(), 0, 0, headers, HTTP_GET);
}


/**
* Realiza una peticion http get con las cabeceras especificadas
*/
bool HttpUtil::get(string url, map <string, string> *headers){
    return sendHttp(url, "", 0, 0, headers, HTTP_GET);
}

/**
*
*   Este post se utiliza principalmente para enviar datos de un array que contiene principalmente
*   datos binarios de un fichero (ya sea .exe, .gif...) al cual es necesario pasarle el tamanyo
*   del array y que se interprete correctamente en el post
*/
bool HttpUtil::post(string url, const char* data, size_t tam, map <string, string> *headers){
    return sendHttp(url, data, tam, 0, headers, HTTP_POST);
}

/**
* data = "name=daniel&project=curl"
* http://streamcloud.eu/512zjmlchbr7
*/
bool HttpUtil::post(string url, string data, map <string, string> *headers){
    return sendHttp(url, data.c_str(), data.length(), 0, headers, HTTP_POST);
}

/**
* Realiza una peticion http put con las cabeceras especificadas para subir un fichero
*/
bool HttpUtil::put(string url, const char* data, size_t tam, size_t offset, map <string, string> *headers){
    return sendHttp(url, data, tam, offset, headers, HTTP_PUT);
}

/**
*
*/
bool HttpUtil::del(string url, map <string, string> *headers){
    return sendHttp(url, "", 0, 0, headers, HTTP_DELETE);
}



/**
* url    : Es la url a la que conectarse
*
* data   : En el caso de POST, especifica el contenido que se envia en el POSTDATA.
*          En el caso de PUT, especifica el fichero que se ha de subir al servidor por PUT
*          En el caso de GET, especifica el fichero en el que se guardara la respuesta del servidor
*
* tam    : En el caso del POST y del PUT, especifica el tamanyo del contenido que se va a subir
*
* offset : Solo aplica en el caso del PUT, para subir un fichero por partes. Especifica el offset en bytes
*          del fichero que se esta subiendo.
*
* headers: Cabeceras que se enviaran para todas las peticiones, GET, POST y PUT
*/
bool HttpUtil::sendHttp(string url, const char* data, size_t tam, size_t offset, map <string, string> *headers, long httpType){
    CURL *curl;
    CURLcode res;
    bool downState = false;
    FILE * hd_src = NULL;
    struct stat file_info;
    aborted = false;

    //Traza::print("HttpUtil::sendHttp " + string(httpType == 0 ? "POST" : httpType == 1 ? "GET" : "PUT") + ", " +  url, W_INFO);

    if(chunk.memory)
        free(chunk.memory);

    if(header.memory)
        free(header.memory);

    if (chunk.filepath)
        free(chunk.filepath);

    chunk.memory = (char *) malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;
    chunk.filepath = NULL;

    header.memory = (char *) malloc(1);  /* will be grown as needed by the realloc above */
    header.size = 0;
    header.filepath = NULL;
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    curl = curl_easy_init();
    /* specify proxy*/
    if (!proxyIP.empty()){
        string auth = proxyUser + ":" + proxyPass;
        curl_easy_setopt(curl, CURLOPT_PROXY, proxyIP.c_str());
        curl_easy_setopt(curl, CURLOPT_PROXYPORT, proxyPort);
        curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, auth.c_str() );
    }
    if(curl) {
        #ifdef SKIP_PEER_VERIFICATION
            /*
             * If you want to connect to a site who isn't using a certificate that is
             * signed by one of the certs in the CA bundle you have, you can skip the
             * verification of the server's certificate. This makes the connection
             * A LOT LESS SECURE.
             *
             * If you have a CA cert for the server stored someplace else than in the
             * default bundle, then the CURLOPT_CAPATH option might come handy for
             * you.
             */
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif

        #ifdef SKIP_HOSTNAME_VERIFICATION
            /*
             * If the site you're connecting to uses a different host name that what
             * they have mentioned in their server certificate's commonName (or
             * subjectAltName) fields, libcurl will refuse to connect. You can skip
             * this check, but this will make the connection less secure.
             */
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        #endif

        /*Especificamos la url a la que conectarse*/
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /*Sigue las redirecciones de los sitios*/
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        /* some servers don't like requests that are made without a user-agent
         field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT.c_str());

        switch (httpType){
            case HTTP_POST:
                curl_easy_setopt(curl, CURLOPT_POST, 1);
                /* Now specify the POST data */
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, tam);
                break;
            case HTTP_PUT:
//                /* enable verbose for easier tracing */
//                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                /* enable uploading */
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                /* HTTP PUT please */
                curl_easy_setopt(curl, CURLOPT_PUT, 1L);
                /* get a FILE * of the same file, could also be made with
                 fdopen() from the previous descriptor, but hey this is just
                 an example! */
                hd_src = fopen(data, "rb");
                fseek ( hd_src, offset, SEEK_SET);
                /* we want to use our own read function */
                curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
                /* now specify which file to upload */
                curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
                /* provide the size of the upload, we specicially typecast the value
                to curl_off_t since we must be sure to use the correct data size */
                if (sendContentLength)
                    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)tam);
                break;
            case HTTP_DELETE:
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
                curl_easy_setopt(curl, CURLOPT_HEADER, false);
//                curl_easy_setopt(curl, CURLOPT_RETURNTRANSFER, true);
                break;
            case HTTP_GET:
            default:
                curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
                if (data != NULL){
                    string resultado = data;
                    if (strcmp(data, "") != 0){
                        chunk.filepath = (char *) malloc(strlen(data) + 1);  /* will be grown as needed by the realloc above */
                        strcpy(chunk.filepath, data);
                        ofstream file;
                        file.open(data, ios::out | ios::binary | ios::trunc);
                        if (file.is_open()){
                            file.close();
                        }
                    }
                }
            break;
        }

        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        if (headers != NULL){
            struct curl_slist *curlheaders=NULL;
            map<string, string>::const_iterator itr;
            string tempStr = "";
            for(itr = headers->begin(); itr != headers->end(); ++itr){
                tempStr = (*itr).first + ": " + (*itr).second;
                curlheaders = curl_slist_append(curlheaders, tempStr.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlheaders);
        }

        prog.lastruntime = 0;
        prog.curl = curl;

        /*Funcion para el progreso de la descarga o para abortarla*/
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);


        #if LIBCURL_VERSION_NUM >= 0x072000
        /* xferinfo was introduced in 7.32.0, no earlier libcurl versions will
           compile as they won't have the symbols around.

           If built with a newer libcurl, but running with an older libcurl:
           curl_easy_setopt() will fail in run-time trying to set the new
           callback, making the older callback get used.

           New libcurls will prefer the new callback and instead use that one even
           if both callbacks are set. */

        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        /* pass the struct pointer into the xferinfo function, note that this is
           an alias to CURLOPT_PROGRESSDATA */
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
        #endif
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        /*Funcion para escribir en memoria o en el disco*/
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        /*pasamos la informacion de las cabeceras*/
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, (void *)&header);
        /* we pass our 'postResult' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        /* we get the headers response*/
        readBufferHeader.clear();
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, handleHeader);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        /* Check for errors */
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            Traza::print("curl_easy_perform() failed", http_code, W_ERROR);
        } else {
            downState = true;
            parserCabeceras();
        }

        double speed_upload, total_time;
        curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n", speed_upload, total_time);
        //Traza::print("speed_upload", speed_upload, W_INFO);
        /* always cleanup */
        curl_easy_cleanup(curl);
        if (hd_src != NULL) fclose(hd_src); /* close the local file */
    }
    curl_global_cleanup();
    return downState;
}

/**
*
*/
void HttpUtil::parserCabeceras(){
    cabecerasResp.clear();
    size_t pos;
    vector<string> v = Constant::split(readBufferHeader, "\n");
    for (int i=0; i < v.size(); i++){
        pos = v.at(i).find(":");
        if (pos != string::npos){
//            cout << i << ":" << v.at(i) << endl;
//            cout << "****" << v.at(i).substr(pos+2, v.at(i).length() - pos - 3) << "*****" << endl;
            cabecerasResp.insert( make_pair(v.at(i).substr(0,pos), v.at(i).substr(pos+2, v.at(i).length() - pos - 3)));
        }
    }
}

/**
*
*/
size_t HttpUtil::handleHeader(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    readBufferHeader.append((char *)contents, realsize);
    return realsize;
}

/**
* Se utiliza para enviar datos al servidor. Por ejemplo en el PUT
*/
size_t HttpUtil::read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream){
    size_t retcode = 0;
    if (nmemb*size > 0){
        char *buffer = new char[nmemb*size];
        retcode = fread(buffer, size, nmemb, stream);
        memcpy(ptr, buffer, nmemb*size);
        delete [] buffer;
    }
    return retcode;
}

/**
* Se utiliza para recibir los datos del servidor.
* Dependiendo de si el campo userp --> MemoryStruct.filepath es nulo, cargara el contenido
* de la descarga en memoria o en el disco duro
*/
size_t HttpUtil::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

//    cout << mem->size << endl;
//    if (mem->size > 500)
//        return 0;

    if (mem->filepath == NULL){
        //Escribimos el fichero descargado en la memoria
        mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
        if(mem->memory == NULL) {
            /* out of memory! */
            Traza::print("not enough memory (realloc returned NULL)", W_DEBUG);
            return 0;
        }
        memcpy(&(mem->memory[mem->size]), contents, realsize);
        mem->size = mem->size + realsize;
        mem->memory[mem->size] = 0;
    } else {
        //Escribimos el fichero descargado en un fichero del disco duro
        ofstream file;
        file.open(mem->filepath, ios::out | ios::binary | ios::app);
        if (file.is_open()){
            char *contenido = new char[realsize];
            memcpy(contenido, contents, realsize);
            file.write(contenido, realsize);
            delete [] contenido;
            file.close();
        } else {
            file.close();//Cerramos el fichero
            Traza::print("HttpUtil: file: " +  string(mem->filepath) +  " not found or could not be opened", W_DEBUG);
        }
    }
    return realsize;
}

/**
*
*/
bool HttpUtil::writeToFile(string path){
    return writeToFile(path.c_str(), chunk.memory, chunk.size, false);
}

/**
* operaciones de escritura
*
* ios::in	Open for input operations.
* ios::out	Open for output operations.
* ios::binary	Open in binary mode.
* ios::ate	Set the initial position at the end of the file. If this flag is not set to any value, the initial position is the beginning of the file.
* ios::app	All output operations are performed at the end of the file, appending the content to the current content of the file. This flag can only be used in streams open for output-only operations.
* ios::trunc	If the file opened for output operations already existed before, its previous content is deleted and replaced by the new one.
*/
bool HttpUtil::writeToFile(const char *uri, char * memblocktowrite, ifstream::pos_type tam, bool append){
    bool ret = false;
    ofstream file;

    if (append){
        file.open(uri,ios::out | ios::binary | ios::app);
    } else {
        file.open(uri,ios::out | ios::binary | ios::trunc);
    }

    if (file.is_open()){
        file.write(memblocktowrite, tam);
        file.close();
        ret = true;
        Traza::print("HttpUtil: downloaded file in: " + string(uri), W_DEBUG);
    } else {
        file.close();//Cerramos el fichero
        Traza::print("HttpUtil: file: " + string(uri) + " not found or could not be opened", W_DEBUG);
    }
    #ifdef GP2X
        sync();
    #endif
    return ret;
}

/**
* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION)
*/
int HttpUtil::older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow)
{
  return xferinfo(p,
                  (curl_off_t)dltotal,
                  (curl_off_t)dlnow,
                  (curl_off_t)ultotal,
                  (curl_off_t)ulnow);
}

/**
* Funcion para controlar cuantos bytes descargar o para
* abortar la descarga en cualquier momento
* this is how the CURLOPT_XFERINFOFUNCTION callback works
*/
int HttpUtil::xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
  struct myprogress *myp = (struct myprogress *)p;
  CURL *curl = myp->curl;
  double curtime = 0;

  curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);

  /* under certain circumstances it may be desirable for certain functionality
     to only run every N seconds, in order to do this the transaction time can
     be used */
  if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
    myp->lastruntime = curtime;
    //Traza::print("TOTAL TIME: " + Constant::TipoToStr(curtime), W_DEBUG);
  }

  downloadProgress = dltotal > 0.0 ? (dlnow / (float)dltotal * 100) : 0.0;
//  Traza::print(" Progress " + Constant::TipoToStr(downloadProgress) + "%", W_DEBUG);
//  Traza::print(Constant::TipoToStr(ulnow) + ";" + Constant::TipoToStr(ultotal) + ";" + Constant::TipoToStr(dlnow)
//               + ";" + Constant::TipoToStr(dltotal) + ";" + Constant::TipoToStr(downloadProgress), W_DEBUG);

  if( (maxBytesDownload > 0 && dlnow > maxBytesDownload) || aborted)
    return 1;
  return 0;
}