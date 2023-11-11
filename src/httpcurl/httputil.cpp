#include "httputil.h"
#include "util/ConstantHttp.h"
#include "uiobjects/Constant.h"

bool HttpUtil::aborted;

/**
 * Constructor
 */
HttpUtil::HttpUtil(){
    init();
}

/**
 *
 */
void HttpUtil::init(){
    aborted = false;
    chunk.memory = NULL;
    chunk.filepath = NULL;
    chunk.size = 0;

    prog = new Progress();
    this->setProxyIP(Constant::getPROXYIP());
    this->setProxyPort(Constant::strToTipo<int>(Constant::getPROXYPORT()));
    this->setProxyUser(Constant::getPROXYUSER());
    this->setProxyPass(Constant::getPROXYPASS());
    this->setTimeout(SECONDS_TO_ABORT_STUCK_DOWNLOAD); //Timeout of 5 seconds
    this->setMaxBytesDownload(0);
    this->setConnectionRetries(1);
    sendContentLength = true;
}

/**
* Destructor
*/
HttpUtil::~HttpUtil(){
    cleanChunkData();
    if (prog != NULL){
       delete prog;
       prog = NULL;
    }
}

void HttpUtil::cleanChunkData(){
    if(chunk.memory != NULL && chunk.size > 0){
        free(chunk.memory);
        chunk.memory = NULL;
    }
    if(chunk.filepath != NULL){
        free(chunk.filepath);
        chunk.filepath = NULL;
    }
    chunk.size = 0;
    chunk.filepath = NULL;
    cabecerasResp.clear();
}

/**
* Solo descarga una url en memoria
*/
bool HttpUtil::download(string url){
    return sendHttpWithRetries(url, "", 0, 0, NULL, HTTP_GET);
}

/**
* Descarga una url al disco duro
*/
bool HttpUtil::download(string url, string pathfilename){
    return sendHttpWithRetries(url, pathfilename.c_str(), 0, 0, NULL, HTTP_GET);
}

/**
* Descarga una url al disco duro pero utilizando cabeceras para la peticion http
*/
bool HttpUtil::download(string url, string pathfilename, map <string, string> *headers){
    return sendHttpWithRetries(url, pathfilename.c_str(), 0, 0, headers, HTTP_GET);
}


/**
* Realiza una peticion http get con las cabeceras especificadas
*/
bool HttpUtil::get(string url, map <string, string> *headers){
    return sendHttpWithRetries(url, "", 0, 0, headers, HTTP_GET);
}

/**
* Este post se utiliza principalmente para subir ficheros en lugar de hacerlo por un PUT.
* Es requerido por el api de dropbox
*/
bool HttpUtil::post(string url, const char* data, size_t tam, size_t offset, map <string, string> *headers){
    return sendHttpWithRetries(url, data, tam, offset, headers, HTTP_POST2);
}

/**
*
*   Este post se utiliza principalmente para enviar datos de un array que contiene principalmente
*   datos binarios de un fichero (ya sea .exe, .gif...) al cual es necesario pasarle el tamanyo
*   del array y que se interprete correctamente en el post
*/
bool HttpUtil::post(string url, const char* data, size_t tam, map <string, string> *headers){
    return sendHttpWithRetries(url, data, tam, 0, headers, HTTP_POST);
}

/**
* data = "name=daniel&project=curl"
* http://streamcloud.eu/512zjmlchbr7
*/
bool HttpUtil::post(string url, string data, map <string, string> *headers){
    return sendHttpWithRetries(url, data.c_str(), data.length(), 0, headers, HTTP_POST);
}

/**
 * To download by post
*/
bool HttpUtil::postDownload(string url, string data, map <string, string> *headers){
    return sendHttpWithRetries(url, data.c_str(), data.length(), 0, headers, HTTP_POST3);
}


/**
* Realiza una peticion http put con las cabeceras especificadas para subir un fichero
*/
bool HttpUtil::put(string url, const char* data, size_t tam, size_t offset, map <string, string> *headers){
    return sendHttpWithRetries(url, data, tam, offset, headers, HTTP_PUT);
}

/**
*
*/
bool HttpUtil::del(string url, map <string, string> *headers){
    return sendHttpWithRetries(url, "", 0, 0, headers, HTTP_DELETE);
}


bool HttpUtil::sendHttpWithRetries(string url, const char* data, size_t tam, size_t offset, map <string, string> *headers, long httpType){
    bool success = false;
    int nTry = 0;

    do{
        success = sendHttp(url, data, tam, offset, headers, httpType);
        nTry++;
        if (!success){
            Traza::print("Reintentando envio de " + url, nTry, W_ERROR);
        }
    } while (nTry < this->getConnectionRetries() && !success);
    return success;
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
    aborted = false;


    mtx.lock();
    cleanChunkData();
    chunk.memory = (char *) calloc(data != NULL && httpType == HTTP_GET ? MAX_FILE_BUFFER : 1, 1);
    if (chunk.memory == NULL){
        printf("sendHttp: Could not allocate memory\n");
        return 0;
    }
    readBufferHeader.clear();


    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    curl = curl_easy_init();
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    mtx.unlock();

    /* specify proxy*/
    if (!proxyIP.empty()){
        Traza::print("Setting proxy settings", W_ERROR);
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

        #ifdef SSL_VER_3
            /**
             * This is needed for dropbox api V2
             */
            curl_easy_setopt(curl, CURLOPT_SSLVERSION, 3L); // Force SSLv3 to fix Unknown SSL Protocol error
        #endif

        /*Especificamos la url a la que conectarse*/
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        /*Sigue las redirecciones de los sitios*/
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        /* some servers don't like requests that are made without a user-agent
         field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT.c_str());
        /* ask libcurl to allocate a larger receive buffer */
        curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, CURL_MAX_WRITE_SIZE);

        char *buffer = NULL;
        errno_t err;


        switch (httpType){
            case HTTP_POST:
                curl_easy_setopt(curl, CURLOPT_POST, 1);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, tam);
                break;
            case HTTP_POST2:
                //To upload files. Needed by dropbox
                curl_easy_setopt(curl, CURLOPT_POST, 1);
                err = fopen_s(&hd_src, data, "rb");
                if (err == 0 && hd_src != NULL && tam > 0) {
                    fseek(hd_src, offset, SEEK_SET);
                    if ((buffer = (char*) calloc(tam, 1)) != NULL) {
                        fread(buffer, 1, tam, hd_src);
                        /* Now specify the POST data */
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, tam);
                    } else
                        cerr << "error al crear memoria en HTTP_POST2" << endl;
                } else {
                    cerr << "HTTP_POST2: file: " << data << "not found or could not be opened" << endl;
                    decodeError(err);
                }
                break;
            case HTTP_PUT:
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                curl_easy_setopt(curl, CURLOPT_PUT, 1L);
                err = fopen_s(&hd_src, data, "rb");
                if (err == 0 && hd_src != NULL) {
                    fseek(hd_src, offset, SEEK_SET);
                    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
                    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
                    if (sendContentLength)
                        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)tam);
                } else {
                    cerr << "HTTP_PUT: file: " << data << "not found or could not be opened" << endl;
                    decodeError(err);
                }
                break;
            case HTTP_DELETE:
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
                curl_easy_setopt(curl, CURLOPT_HEADER, false);
//                curl_easy_setopt(curl, CURLOPT_RETURNTRANSFER, true);
                break;
            default:
                if (httpType == HTTP_POST3){
                    curl_easy_setopt(curl, CURLOPT_POST, 1L);
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ""); //no need to pass postData
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0);
                } else if (httpType == HTTP_GET){
                    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
                }

                if (data != NULL){
                    size_t len = strlen(data) + 1;
                    if (len > 1){
                        chunk.filepath = (char *) calloc(len, 1);
                        if (chunk.filepath != NULL){
                            strcpy_s(chunk.filepath, len, data);
                        } else {
                            cerr << "No se pudo crear memoria para inicializar chunk->filepath" << endl;
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

        if (prog != NULL){
            prog->init();
            prog->setTimeout(getTimeout());
            prog->curl = curl;
        }


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
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
        #else
        /*Funcion para el progreso de la descarga o para abortarla*/
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, prog);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
        #endif
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        /*Funcion para escribir en memoria o en el disco*/
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        /* we pass our 'postResult' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        /* we get the headers response*/
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, handleHeader);
        /*pasamos la informacion de las cabeceras*/
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
        //curl_easy_setopt(curl, CURLOPT_WRITEHEADER, (void *)&header);



//        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); /* start cookie engine */
//        char nline[5000] = {""};
//        curl_easy_setopt(curl, CURLOPT_COOKIELIST, cookies); /* start cookie engine */
//        char nline[256];
//        snprintf(nline, sizeof(nline),
//        "Set-Cookie: OLD_PREF=3d141414bf4209321; "
//        "expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.example.com");
//        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        //Obtaining the response code
        http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        /* Check for errors */
        if(res != CURLE_OK){
            Traza::print("curl_easy_perform() failed: " + string(curl_easy_strerror(res)) + ". Url: " + url, http_code, W_ERROR);
//            cout << "curl_easy_perform() failed: " + string(curl_easy_strerror(res)) + ". Url: " + url << ":" << http_code << endl;
        } else {
            downState = true;
            parserCabeceras();
            if (chunk.memory != NULL && chunk.filepath != NULL){
                checkWriteMemToFile(chunk.memory, chunk.size, chunk.filepath);
                chunk.size = 0;
                free(chunk.memory);
                chunk.memory = NULL;
            }
        }
//        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);

        double speed_upload, total_time;
        curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n", speed_upload, total_time);
        //Traza::print("speed_upload", speed_upload, W_INFO);
        /* always cleanup */
        curl_easy_cleanup(curl);
        if (hd_src != NULL) fclose(hd_src); /* close the local file */
        if (buffer != NULL) free(buffer);
    }
    curl_global_cleanup();
    return downState;
}

/**
 * Obtains the response headers and saves a map with its values
 */
void HttpUtil::parserCabeceras(){
    cabecerasResp.clear();
    size_t pos;
    vector<string> v = Constant::split(readBufferHeader, "\n");
    for (size_t i=0; i < v.size(); i++){
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
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return
 */
size_t HttpUtil::handleHeader(void *contents, size_t size, size_t nmemb, void *f){

    // Call non-static member function.
    size_t realsize = size * nmemb;
    static_cast<HttpUtil*>(f)->getReadBufferHeader()->append((char *)contents, realsize);
    return realsize;
}

/**
 * Se utiliza para enviar datos al servidor. Por ejemplo en el PUT
 * @param ptr
 * @param size
 * @param nmemb
 * @param stream
 * @return
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
 * Writes the received chunk of data to memory or a file in the disk
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return
 */
size_t HttpUtil::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *f){
    size_t realsize = size * nmemb;

    MemoryStruct *mem = static_cast<HttpUtil*>(f)->getChunk();
    size_t totalDown = 0;

    //We start counting the total size of the buffered memory
    totalDown = mem->size + realsize;

    //We add the total file size until now
    if (mem->filepath != NULL) {
        ifstream file (mem->filepath, ios::in|ios::binary|ios::ate);
        if (file.is_open()){
            totalDown += (size_t)file.tellg();
        }
        file.close();
    }

    if (Constant::getCURL_DOWNLOAD_LIMIT() > 0 && totalDown > Constant::getCURL_DOWNLOAD_LIMIT()){
        Traza::print("CURL_DOWNLOAD_LIMIT. Size of download exceeded", totalDown, W_DEBUG);
        Constant::setCURL_DOWNLOAD_LIMIT(0); //We don't want to forget to set to 0 again
        return 0;
    }

    //Adding the data to the buffer
    if (mem->filepath == NULL){
        if (!static_cast<HttpUtil*>(f)->addDataToMem(contents, realsize, mem)){
            return 0;
        }
    } else if (realsize > 0){
        //Check if we must write the contentent of the buffer to disk
        //cerr << "downloading file: " << size << " - " <<  nmemb << endl;
        if (static_cast<HttpUtil*>(f)->addDataToMem(contents, realsize, mem)){
            if (mem->size > MAX_FILE_BUFFER){
                //cerr << "writing file with size: " << mem->size << " | " << realsize << endl;
                static_cast<HttpUtil*>(f)->checkWriteMemToFile(mem->memory, mem->size, mem->filepath);
                mem->size = 0;
                free(mem->memory);
                mem->memory = NULL;
            }
        }
    }
    return realsize;
}

/**
 * Add the chunk of data to the memory buffer with a defined size of MAX_FILE_BUFFER
 * @param contents
 * @param realsize
 * @param mem
 */
int HttpUtil::addDataToMem(void *contents, size_t realsize, MemoryStruct *mem){

    if (contents == NULL){
        cerr << "addDataToMem data is null" << endl;
        return 0;
    }

    if (mem->size == 0 && mem->memory == NULL){
        mem->memory = (char *)calloc(mem->size + realsize + 1, 1);
        if (mem->memory == NULL){
            cerr << "addDataToMem not enough memory (calloc returned NULL)" << endl;
            return 0;
        }
    } else {
        char* tmp = (char *)realloc(mem->memory, mem->size + realsize + 1);
        if (tmp != NULL) {
            mem->memory = tmp;
        } else {
            /* out of memory! */
            cerr << "addDataToMem not enough memory (realloc returned NULL)" << endl;
            return 0;
        }
    }
    memcpy(&(mem->memory[mem->size]), (char *)contents, realsize);
    mem->size = mem->size + realsize;
    mem->memory[mem->size] = 0;
    //printf("Adding data to memory %d\n", mem->size);
    return 1;
}

/**
 * Check if we must write the contents of the buffered memory to a file
 *
 * @param mem
 */
void HttpUtil::checkWriteMemToFile(void *contents, size_t sizeToWrite, char *filepath){
    if (contents != NULL && sizeToWrite != 0 && filepath != NULL){
        //Escribimos el fichero descargado en un fichero del disco duro
        errno_t err;
        FILE* file = NULL;
        err = fopen_s(&file, filepath, "ab");
        if (err == 0 && file != NULL){
            fwrite((char *)contents, 1, sizeToWrite, file);
            fclose(file);
        } else {
            printf("HttpUtil::checkWriteMemToFile file: %s not found or could not be opened", filepath);
            decodeError(err);
        }
    }
}

/**
 * Writes all the memory buffer to disk
 * @param path
 * @return
 */
bool HttpUtil::writeToFile(string path){
    return writeToFile(path.c_str(), chunk.memory, chunk.size, false);
}

/**
* Writes the specified amount of data of the memory buffer to disk
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
 * @param p
 * @param dltotal
 * @param dlnow
 * @param ultotal
 * @param ulnow
 * @return
 */
int HttpUtil::xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
  //struct myprogress *myp = (struct myprogress *)p;
  //Progress *myp = (Progress *)p;
  //auto myp = static_cast<Progress *>(p);
  Progress *myp = static_cast<HttpUtil*>(p)->getProgress();

  if (myp == NULL)
      return 0;

  if (myp->curl == NULL)
      return 0;

  CURL *curl = myp->curl;
  double curtime = 0;

  curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &curtime);
  /* under certain circumstances it may be desirable for certain functionality
     to only run every N seconds, in order to do this the transaction time can
     be used */
  if((curtime - myp->getLastruntime()) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
    myp->setLastruntime(curtime) ;
    //Traza::print("TOTAL TIME: " + Constant::TipoToStr(curtime), W_DEBUG);
  }

  if ((curtime - myp->getLastruntimeBytesDown()) >= 1.0){
      myp->setLastruntimeBytesDown(curtime);
      myp->setDlSpeed(dltotal > MIN_PROGRESS_CHUNK_OF_BYTES ? dlnow - myp->getLastBytesDownloaded() : 0.0);
      myp->setLastBytesDownloaded(dlnow);
  }

  //We expect to download at least 512 bytes. this is to avoid errors in the progress count
  if (dltotal > MIN_PROGRESS_CHUNK_OF_BYTES){
        myp->setDlSizeBytes(dltotal);
        myp->setProgress(dltotal > 0.0 ? (dlnow / (float)dltotal * 100) : 0.0);
//        Traza::print(" Progress " + Constant::TipoToStr(myp->getProgress()) + "%", W_DEBUG);
//        Traza::print(Constant::TipoToStr(ulnow) + ";" + Constant::TipoToStr(ultotal) + ";" + Constant::TipoToStr(dlnow)
//                     + ";" + Constant::TipoToStr(dltotal) + ";" + Constant::TipoToStr(myp->getProgress()), W_DEBUG);
  } else {
      myp->setDlSizeBytes(0.0);
      myp->setProgress(0.0);
  }

  //Traza::print("dltotal: " + Constant::TipoToStr(dltotal) + ", Progress " + Constant::TipoToStr(myp->getProgress()) + ", last: " + Constant::TipoToStr(myp->getLastProgress()), W_DEBUG);
  if (myp->getProgress() != myp->getLastProgress() || myp->getTimeNoProgress() == 0.0){
      myp->setTimeNoProgress(curtime);
      myp->setLastProgress(myp->getProgress());
  }

  if (curtime - myp->getTimeNoProgress() > myp->getTimeout() && myp->getTimeout() > 0){
      Traza::print("Aborting download: " + Constant::TipoToStr(myp->getTimeout()) + " seconds with no activity. Curtime: " + Constant::TipoToStr(curtime), W_DEBUG);
      return 1;
  }

  if( (myp->getMaxBytesDownload() > 0 && dlnow > myp->getMaxBytesDownload())){
      Traza::print("Aborting download: " + Constant::TipoToStr(myp->getMaxBytesDownload()) + " Max. bytes of download", W_DEBUG);
      return 1;
  }

  if(aborted){
      Traza::print("Aborting download: ending all threads signal received", W_DEBUG);
      return 1;
  }


  return 0;
}

void HttpUtil::setConnectionRetries(int connectionRetries) {
    this->connectionRetries = connectionRetries;
}

int HttpUtil::getConnectionRetries() const {
    return connectionRetries;
}

void HttpUtil::setCookies(curl_slist* cookies) {
    this->cookies = cookies;
}

curl_slist* HttpUtil::getCookies() {
    return cookies;
}

bool HttpUtil::isValidURL(string url)
{

  // Regex to check valid URL
  const regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");

  // If the URL
  // is empty return false
  if (url.empty())
  {
     return false;
  }

  // Return true if the URL
  // matched the ReGex
  if(regex_match(url, pattern))
  {
    return true;
  }
  else
  {
    return false;
  }
}
