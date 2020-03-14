#ifndef HTTPUTIL_H
#define HTTPUTIL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "filecipher.h"
#include "Traza.h"
#include "unzip/GZipHelper.h"
#include <curl/curl.h>
#include <gmutex.h>

#define MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL     3

using namespace std;


class Progress {
 public:
        Progress(){ 
            Progress(0,1);
        }
        ~Progress(){
            mutex.Lock();
            if (this->arrProgress != NULL){
                delete[] this->arrProgress;
                this->arrProgress = NULL;
            }
            mutex.Unlock();
        }
        
        Progress(int posListThreads, int nThreads){ 
            mutex.Lock();
            this->posListThreads = posListThreads;
            this->nThreads = nThreads;
            this->arrProgress = new float[nThreads]();
            
            for (int i=0; i < nThreads; i++){
                this->arrProgress[i] = 0.0;
            }
            mutex.Unlock();
        }
        
        void setProgress(float progress) { 
            if (this->arrProgress != NULL){
                this->arrProgress[this->posListThreads] = progress; 
            }
        }
        
        float getProgress(){
            if (this->arrProgress != NULL){
                return this->arrProgress[this->posListThreads];
            } else {
                return 0.0;
            }
        }
        
        double lastruntime;
        std::ifstream::pos_type maxBytesDownload;
        CURL *curl;
        
        void setPosListThreads(int posListThreads){this->posListThreads = posListThreads;}
        int getPosListThreads(){
            return this->posListThreads;
        }
        void setNThreads(int nThreads){this->nThreads = nThreads;}
        int getNThreads(){
            return this->nThreads;
        };
 private:
    static float *arrProgress;
    int nThreads;
    int posListThreads;
    GMutex mutex;
};

class HttpUtil
{
    public:
        HttpUtil();
        HttpUtil(int posListThreads, int nThreads);
        virtual ~HttpUtil();

        bool download(string);
        bool download(string, string);
        bool download(string, string, map <string, string> *headers);
        bool writeToFile(string);
        bool post(string, const char *,size_t, size_t, map <string, string> *);
        bool post(string, const char *,size_t, map <string, string> *);
        bool post(string, string, map <string, string> *);
        bool postDownload(string url, string data, map <string, string> *headers);
        bool get (string, map <string, string> *);
        bool put(string, const char*, size_t, size_t, map <string, string> *);
        
        bool del(string url, map <string, string> *headers);

        void setProxyIP(string var){proxyIP = var;}
        void setProxyPort(unsigned short var){proxyPort = var;}
        void setProxyUser(string var){proxyUser = var;}
        void setProxyPass(string var){proxyPass = var;}
        string getProxyIP(){return proxyIP;}
        unsigned short getProxyPort(){return proxyPort;}
        string getProxyUser(){return proxyUser;}
        string getProxyPass(){return proxyPass;}
        char * getRawData(){return chunk.memory;}

        char * getData(){
            CGZIP2A a((unsigned char *)chunk.memory, chunk.size);
            if(chunk.memory)
                free(chunk.memory);

            chunk.memory = (char *) malloc(strlen(a.psz));  /* will be grown as needed by the realloc above */
            chunk.size = strlen(a.psz);

            memcpy(chunk.memory, a.psz, strlen(a.psz));
            return chunk.memory;
        }

        size_t getDataLength(){return chunk.size;}
        int getTimeout(){return timeout;}
        void setTimeout(int var){timeout = var;}
        void abort(){aborted = true;}
        void setMaxBytesDownload(std::ifstream::pos_type var){prog->maxBytesDownload = var;}
        //float getDownloadProgress(){return prog.getProgress();}
        float getDownloadProgress(){return prog->getProgress();}
        long getHttp_code(){return http_code;}
        map<string, string> *getResponseHeaders(){return &this->cabecerasResp;}

        void setSendContentLength(bool var){sendContentLength = var;}
        bool getSendContentLength(){return sendContentLength;}
        
    protected:
    private:
        string proxyIP;
        unsigned short proxyPort;
        string proxyUser;
        string proxyPass;
        struct MemoryStruct {
          char *memory;
          size_t size;
          char *filepath;
        };
        

        void parserCabeceras();
        map<string, string> cabecerasResp;

        struct MemoryStruct chunk;
        struct MemoryStruct header;
        int timeout;

//        struct myprogress {
//          double lastruntime;
//          float downloadProgress;
//          std::ifstream::pos_type maxBytesDownload;
//          CURL *curl;
//        } prog;
        
        Progress *prog;

        static std::string readBufferHeader;
        static size_t handleHeader(void *contents, size_t size, size_t nmemb, void *userp);
        static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
        static size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream);
        bool writeToFile(const char *, char *, ifstream::pos_type, bool);
        bool sendHttp(string url, const char* data, size_t, size_t, map <string, string> *headers, long httpType);

        static int older_progress(void *p,
                          double dltotal, double dlnow,
                          double ultotal, double ulnow);
        
        static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);

        static bool aborted;
//        static std::ifstream::pos_type maxBytesDownload;
        long http_code;
        bool sendContentLength;
};

#endif // HTTPUTIL_H
