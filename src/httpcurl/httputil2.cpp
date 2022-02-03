/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   httputil2.cpp
 * Author: Dani
 * 
 * Created on 2 de febrero de 2022, 13:59
 */


#include "httputil2.h"

uint8_t Httputil2::aborted = 0;

Httputil2::Httputil2() {
}

Httputil2::Httputil2(const Httputil2& orig) {
}

Httputil2::~Httputil2() {
}

void Httputil2::initProgress(Progress2 *p){
    p->progress = 0.0;
    p->speedDown = 0.0;
    p->lastProgress = 0.0;
    p->bytesDown = 0;
    p->lastruntime = 0.0;
    p->lastBytesDownloaded = 0;
    p->lastruntimeBytesDown = 0.0;
    p->timeNoProgress = 0.0;
}


void Httputil2::cleanData(MemoryStruct *p){
    
    if (p == NULL)
        return;
    
    if (p->memory != NULL && p->size > 0) 
        free(p->memory);

    p->memory = NULL;
    p->size = 0;

    if (p->readBufferHeader != NULL) 
        free(p->readBufferHeader);
    p->readBufferHeader = NULL;

    //if (p->cabecerasResp != NULL) 
        p->cabecerasResp.clear(); 

    p->curlDownloadLimit = 0;
    if (p->filepath != NULL)
        free(p->filepath);
    p->filepath = NULL;

    
    initProgress(&p->prog);
}


MemoryStruct * Httputil2::initDownload(){
    //aborted = 0;
    MemoryStruct *chunk = (MemoryStruct *) calloc (1, sizeof(MemoryStruct));
    if (chunk != NULL){
        cleanData(chunk);
        chunk->connProps.timeout = SECONDS_TO_ABORT_STUCK_DOWNLOAD; //Timeout of 5 seconds
        chunk->connProps.connectionRetries = 1;
        chunk->connProps.sendContentLength = 1;
    } else {
        cerr << "initDownload error al reservar memoria en initDownload" << endl;
    }
    return chunk;
}

void Httputil2::cleanChunkData(MemoryStruct *chunk){
    cleanData(chunk);
}

void Httputil2::endDownload(MemoryStruct **chunk){
    cleanChunkData(*chunk);
    free(*chunk);
    *chunk = NULL;
    //SDL_DestroyMutex( conn_mutex );
}


/**
 * Obtains the response headers and saves a map with its values
 */
void Httputil2::parserCabeceras(MemoryStruct *p){
    p->cabecerasResp.clear();
    size_t pos;
    vector<string> v = Constant::split(p->readBufferHeader, "\n");
    for (int i=0; i < v.size(); i++){
        pos = v.at(i).find(":");
        if (pos != string::npos){
//            cout << i << ":" << v.at(i) << endl;
//            cout << "****" << v.at(i).substr(pos+2, v.at(i).length() - pos - 3) << "*****" << endl;
            p->cabecerasResp.insert( make_pair(v.at(i).substr(0,pos), v.at(i).substr(pos+2, v.at(i).length() - pos - 3)));
        }
    }
}

size_t Httputil2::handleHeader(void *contents, size_t size, size_t nmemb, void *userp)
{
    MemoryStruct *mem = (MemoryStruct *)userp;
    size_t realsize = size * nmemb;
    if (realsize <= 0)
        return 0;
    
    if (mem->readBufferHeader != NULL){
        size_t tam = strlen(mem->readBufferHeader);
        char* tmp;
        if ((tmp = (char*)realloc(mem->readBufferHeader, tam + realsize + 1)) != NULL) {
            mem->readBufferHeader = tmp;
            strcat_s(mem->readBufferHeader, tam + realsize + 1, (char*)contents);
        } else 
            cerr << "error al redimensionar memoria en handleHeader" << endl;
    } else {
        mem->readBufferHeader = (char* )calloc(realsize + 1, 1);
        if (mem->readBufferHeader != NULL)
            strcpy_s(mem->readBufferHeader, realsize + 1, (char *)contents);
        else 
            cerr << "error al reservar memoria en handleHeader" << endl;
    }
    //printf("Header: %s",mem->readBufferHeader);
    return realsize;
}


/**
 * Add the chunk of data to the memory buffer with a defined size of MAX_FILE_BUFFER 
 * @param contents
 * @param realsize
 * @param mem
 */
int Httputil2::addDataToMem(void *contents, size_t realsize, MemoryStruct *mem){
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
 * Writes the received chunk of data to memory or a file in the disk
 * @param contents
 * @param size
 * @param nmemb
 * @param userp
 * @return 
 */
size_t Httputil2::writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;
    size_t totalDown = 0;
    
    //We start counting the total size of the buffered memory
    totalDown = mem->size + realsize;
    
    //We add the total file size until now
    if (mem->filepath != NULL) {
        FILE* f = NULL;
        errno_t err;
        err = fopen_s(&f, mem->filepath, "rb");
        size_t tam = 0;
        if (err == 0 && f != NULL && fseek(f, 0, SEEK_END) == 0 && (tam = ftell(f)) > 0){
            totalDown += tam;
        }
        if (f) 
            fclose(f);
    }
    
    if (mem->curlDownloadLimit > 0 && totalDown > mem->curlDownloadLimit){
        cout << "curlDownloadLimit. Size of download exceeded " << totalDown << endl;
        mem->curlDownloadLimit = 0;
        return 0; 
    }
    
    //Adding the data to the buffer
    if (mem->filepath == NULL){
        if (!addDataToMem(contents, realsize, mem)){
            return 0;
        }
    } else if (realsize > 0){
        //Check if we must write the contentent of the buffer to disk
        //cerr << "downloading file: " << size << " - " <<  nmemb << endl;
        if (addDataToMem(contents, realsize, mem)){
            if (mem->size > MAX_FILE_BUFFER || realsize < CURL_MAX_WRITE_SIZE ){
                //cerr << "writing file with size: " << mem->size << " | " << realsize << endl;
                checkWriteMemToFile(mem->memory, mem->size, mem->filepath);
                mem->size = 0;
                free(mem->memory);
                mem->memory = NULL;
            }
        }
    }
    return realsize;
}

/**
 * Check if we must write the contents of the buffered memory to a file
 * 
 * @param mem
 */
void Httputil2::checkWriteMemToFile(void *contents, size_t sizeToWrite, char *filepath){
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
int Httputil2::xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
      //struct myprogress *myp = (struct myprogress *)p;
      Progress2 *myp = (Progress2 *)p;
      //auto myp = static_cast<Progress2 *>(p);
  
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
      if((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
        myp->lastruntime = curtime ;
        //Traza::print("TOTAL TIME: " + Constant::TipoToStr(curtime), W_DEBUG);
      }
  
      if ((curtime - myp->lastruntimeBytesDown) >= 1.0){
          myp->lastruntimeBytesDown = curtime;
          myp->speedDown = (dltotal > MIN_PROGRESS_CHUNK_OF_BYTES ? dlnow - myp->lastBytesDownloaded : 0.0);
          myp->lastBytesDownloaded = dlnow;
      }
  
      //We expect to download at least 512 bytes. this is to avoid errors in the progress count
      if (dltotal > MIN_PROGRESS_CHUNK_OF_BYTES){ 
            myp->bytesDown = dltotal;
            myp->progress = (dltotal > 0.0 ? (dlnow / (double)dltotal) * 100.0 : 0.0);
    //        Traza::print(" Progress " + Constant::TipoToStr(myp->getProgress()) + "%", W_DEBUG);
    //        Traza::print(Constant::TipoToStr(ulnow) + ";" + Constant::TipoToStr(ultotal) + ";" + Constant::TipoToStr(dlnow)
    //                     + ";" + Constant::TipoToStr(dltotal) + ";" + Constant::TipoToStr(myp->getProgress()), W_DEBUG);
      } else {
          myp->bytesDown = 0;
          myp->progress = 0.0;
      }
  
      //Traza::print("dltotal: " + Constant::TipoToStr(dltotal) + ", Progress " + Constant::TipoToStr(myp->getProgress()) + ", last: " + Constant::TipoToStr(myp->getLastProgress()), W_DEBUG);
      if (myp->progress != myp->lastProgress || myp->timeNoProgress == 0.0){
          myp->timeNoProgress  = curtime;
          myp->lastProgress  = myp->progress;
      } 
  
      if (curtime - myp->timeNoProgress > myp->timeout && myp->timeout > 0){
          printf("Aborting download: %f seconds with no activity. Curtime: %f\n", myp->timeout, curtime);
          return 1;
      }
  
      if( (myp->maxBytesDownload > 0 && dlnow > myp->maxBytesDownload)){
          //printf("Aborting download: %llu Max. bytes of download", myp->maxBytesDownload);
          cout << "Aborting download: " << myp->maxBytesDownload << " Max. bytes of download" << endl;
          return 1;
      }
    
      if(aborted){
          printf("Aborting download: ending all threads signal received");
          return 1;
      }
  
  
      return 0;
}

/**
 * Se utiliza para enviar datos al servidor. Por ejemplo en el PUT
 * @param ptr
 * @param size
 * @param nmemb
 * @param stream
 * @return 
 */
size_t Httputil2::read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream){
    size_t retcode = 0;
    if (nmemb*size > 0){
        char *buffer = (char *)calloc(nmemb, size);
        if (buffer != NULL) {
            retcode = fread(buffer, size, nmemb, stream);
            memcpy(ptr, buffer, nmemb * size);
            free(buffer);
        } else 
            cerr << "error al crear memoria en read_callback" << endl;
    }
    return retcode;
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
int Httputil2::sendHttp(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, long httpType, MemoryStruct *chunk){
    CURL *curl = NULL;
    CURLcode res;
    int downState = 0;
    FILE * hd_src = NULL;
    aborted = 0;
    
    if (chunk == NULL)
        return 0;
    
    // SDL_LockMutex(conn_mutex);
    //Traza::print("HttpUtil::sendHttp " + string(httpType == 0 ? "POST" : httpType == 1 ? "GET" : "PUT") + ", " +  url, W_INFO);
    cleanChunkData(chunk);
    chunk->memory = (char *) calloc(data != NULL && httpType == HTTP_GET ? MAX_FILE_BUFFER : 1, 1);  /* will be grown as needed by the realloc above */
    
    
    if (chunk->memory == NULL || data == NULL){
        printf("sendHttp: Could not allocate memory\n");
        return 0;
    }

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */
    
    curl = curl_easy_init();
    // SDL_UnlockMutex(conn_mutex);
    char *auth = NULL;

    /* specify proxy*/
    
    if (chunk->connProps.proxyIP && chunk->connProps.proxyUser && chunk->connProps.proxyPass){
        printf("Setting proxy settings\n");
        size_t len = strlen(chunk->connProps.proxyUser) + strlen(chunk->connProps.proxyPass) + 2;
        auth = (char *) calloc(len, 1);
        if (auth != NULL){
            strcpy_s(auth, len, chunk->connProps.proxyUser);
            strcat_s(auth, len, ":");
            strcat_s(auth, len, chunk->connProps.proxyPass);
            curl_easy_setopt(curl, CURLOPT_PROXY, chunk->connProps.proxyIP);
            curl_easy_setopt(curl, CURLOPT_PROXYPORT, chunk->connProps.proxyPort);
            curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, auth);
        }
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

        #if !defined  SKIP_PEER_VERIFICATION || !defined SKIP_HOSTNAME_VERIFICATION
            curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem"); //pacman -S ca-certificates
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
        size_t retcode = 0;
        errno_t err;
        
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        
        switch (httpType){
            case HTTP_POST:
                curl_easy_setopt(curl, CURLOPT_POST, 1);
                /* Now specify the POST data */
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
//                if (chunk->connProps.sendContentLength > 0){
//                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, tam);
//                } else {
//                    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
//                    curl_easy_setopt(curl, CURLOPT_IGNORE_CONTENT_LENGTH, 1L);
//                }
                    
                break;
            case HTTP_POST2: 
                //To upload files. Needed by dropbox
                curl_easy_setopt(curl, CURLOPT_POST, 1);

                err = fopen_s(&hd_src, data, "rb");
                if (err == 0 && hd_src != NULL && tam > 0) {
                    fseek(hd_src, offset, SEEK_SET);
                    if ((buffer = (char*) calloc(tam, 1)) != NULL) {
                        retcode = fread(buffer, 1, tam, hd_src);
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
                    if (chunk->connProps.sendContentLength)
                        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)tam);
                } else {
                    cerr << "HTTP_PUT: file: " << data << "not found or could not be opened" << endl;
                    decodeError(err);
                }
                break;
            case HTTP_DELETE:
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
                curl_easy_setopt(curl, CURLOPT_HEADER, 0);
//                curl_easy_setopt(curl, CURLOPT_RETURNTRANSFER, true);
                break;
            default:
                if (httpType == HTTP_POST3){
                    //To download files by post and without need to wait to finish
                    //Needed by dropbox
//                    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                    curl_easy_setopt(curl, CURLOPT_POST, 1L);
                    /* Now specify the POST data */
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ""); //no need to pass postData
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0);
                } else if (httpType == HTTP_GET){
                    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
                }
                
                if (data != NULL){
                    size_t len = strlen(data) + 1;
                    if (len > 1){
                        chunk->filepath = (char *) calloc(len, 1);
                        if (chunk->filepath != NULL){
                            strcpy_s(chunk->filepath, len, data);
                        } else {
                            cerr << "No se pudo crear memoria para inicializar chunk->filepath" << endl;
                        }
                    }
                }
            break;
        }

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        
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
        
        chunk->prog.lastruntime = 0;
        chunk->prog.lastBytesDownloaded = 0;
        chunk->prog.timeout = chunk->connProps.timeout;
        chunk->prog.curl = curl;
        

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
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &chunk->prog);
        #else
        /*Funcion para el progreso de la descarga o para abortarla*/
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, prog);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
        #endif
        /* ask libcurl to allocate a larger receive buffer */
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        /*Funcion para escribir en memoria o en el disco*/
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
        /*pasamos la informacion de las cabeceras*/
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, chunk);
        /* we pass our 'postResult' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, chunk);
        /* we get the headers response*/
        if (chunk->readBufferHeader){
            free(chunk->readBufferHeader);
            chunk->readBufferHeader = NULL;
        }

        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, handleHeader);
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        //Important to call this at the end to write the memory to the file if specified
        //checkWriteMemToFile(chunk);
        //Obtaining the response code
        chunk->connProps.http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &chunk->connProps.http_code);
        /* Check for errors */
        if(res != CURLE_OK){
            printf("curl_easy_perform() failed: %s. Url: %s\n", curl_easy_strerror(res), url);
        } else {
            downState = 1;
            parserCabeceras(chunk);
        }
//        curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);

        double speed_upload, total_time;
        curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        //fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n", speed_upload, total_time);
        //Traza::print("speed_upload", speed_upload, W_INFO);
        /* always cleanup */
        curl_easy_cleanup(curl);
        if (hd_src) fclose(hd_src); /* close the local file */
        if (buffer) free(buffer);
        if (auth) free(auth);
    }
    curl_global_cleanup();
    return downState;
}

/**
 * It does some tries if something in the connection fails
 * */
int Httputil2::sendHttpWithRetries(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, long httpType, MemoryStruct *chunk){
    int success = 0;
    int nTry = 0;
    
    if (chunk == NULL)
        return 0;
    
    do{
        success = sendHttp(url, data, tam, offset, headers, httpType, chunk);
        nTry++;
        if (!success){
            printf("Reintentando envio de %s, %d",url, nTry);
        }
    } while (nTry < chunk->connProps.connectionRetries && !success);
    return success;
}

/**
* Solo descarga una url en memoria
*/
int Httputil2::download(std::string url, MemoryStruct *chunk){
    return sendHttpWithRetries(url, "", 0, 0, NULL, HTTP_GET, chunk);
}

/**
* Descarga una url al disco duro
*/
int Httputil2::downloadToDisk(std::string url, std::string pathfilename, MemoryStruct *chunk){
    return sendHttpWithRetries(url, pathfilename.c_str(), 0, 0, NULL, HTTP_GET, chunk);
}

/**
* Descarga una url al disco duro pero utilizando cabeceras para la peticion http
*/
int Httputil2::downloadToDiskHeaders(std::string url, std::string pathfilename, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, pathfilename.c_str(), 0, 0, headers, HTTP_GET, chunk);
}


/**
* Realiza una peticion http get con las cabeceras especificadas
*/
int Httputil2::httpGet(std::string url, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, "", 0, 0, headers, HTTP_GET, chunk);
}

/**
* Este post se utiliza principalmente para subir ficheros en lugar de hacerlo por un PUT.
* Es requerido por el api de dropbox
*/
int Httputil2::httpPostWithOffset(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, data, tam, offset, headers, HTTP_POST2, chunk);
}

/**
*
*   Este post se utiliza principalmente para enviar datos de un array que contiene principalmente
*   datos binarios de un fichero (ya sea .exe, .gif...) al cual es necesario pasarle el tamanyo
*   del array y que se interprete correctamente en el post
*/
int Httputil2::httpPost(std::string url, const char* data, size_t tam, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, data, tam, 0, headers, HTTP_POST, chunk);
}

/**
* data = "name=daniel&project=curl"
* http://streamcloud.eu/512zjmlchbr7
*/
int Httputil2::httpPostStrlen(std::string url, const char *data, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, data, strlen(data), 0, headers, HTTP_POST, chunk);
}

/**
 * To download by post
*/
int Httputil2::httpPostDownload(std::string url, const char *data, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, data, strlen(data), 0, headers, HTTP_POST3, chunk);
}


/**
* Realiza una peticion http put con las cabeceras especificadas para subir un fichero
*/
int Httputil2::httpPut(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, data, tam, offset, headers, HTTP_PUT, chunk);
}

/**
*
*/
int Httputil2::httpDel(std::string url, map<string, string> * headers, MemoryStruct *chunk){
    return sendHttpWithRetries(url, "", 0, 0, headers, HTTP_DELETE, chunk);
}

/**
 * Writes all the memory buffer to disk
 * @param path
 * @return 
 */
int Httputil2::writeMemToFile(char* path, MemoryStruct *chunk){
    Fileio fileio;
    return fileio.writeToFile(path, chunk->memory, chunk->size, 0);
}

/**
 * Returns the same data obtained by the http response. It can be gzipped though
 * */
char * Httputil2::getRawData(MemoryStruct *chunk){
    return chunk->memory;
}

/**
 * It performs a gunzip before extracting data
 * */
char * Httputil2::getData(MemoryStruct *chunk){
    size_t tamData = 0;
    
    if (chunk->size > 0 && chunk->memory != NULL){
        CGZIP2A a((unsigned char *)chunk->memory, chunk->size);
        tamData = strlen(a.psz);
        
        if (tamData > 0){
            if(chunk->memory){
                free(chunk->memory);
                chunk->memory = NULL;
            }
            chunk->memory = (char *) calloc(tamData, 1);  /* will be grown as needed by the realloc above */
            if (chunk->memory != NULL){
                memcpy(chunk->memory, a.psz, tamData);
            } else {
                cerr << "No se pudo crear la memoria con tamanyo " << tamData <<endl;
            }
        } else {
            cerr << "No se ha podido descomprimir los datos" <<endl;
        }
    }   
    chunk->size = tamData;
    return chunk->memory;
}

