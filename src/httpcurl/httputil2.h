/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   httputil2.h
 * Author: Dani
 *
 * Created on 2 de febrero de 2022, 13:59
 */

#ifndef HTTPUTIL2_H
#define HTTPUTIL2_H

#include <curl/curl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#include "../uiobjects/Fileio.h"
#include "httpcurl/util/ConstantHttp.h"
#include "../uiobjects/unzip/GZipHelper.h"
#include "uiobjects/gmutex.h"
//#include <mutex>          // std::mutex

typedef struct ConnectionProps{
    char* proxyIP;
    unsigned short proxyPort;
    char* proxyUser;
    char* proxyPass;
    int connectionRetries;
    int timeout;
    int http_code;
    uint8_t sendContentLength;
} ConnectionProps;

typedef struct Progress2{
    double progress;
    double speedDown;
    double lastruntime;
    double lastProgress;
    double timeNoProgress;
    double timeout;
    double lastruntimeBytesDown;

    curl_off_t bytesDown;
    curl_off_t lastBytesDownloaded;
    curl_off_t maxBytesDownload;
    CURL *curl;
} Progress2;

typedef struct MemoryStruct {
    char *memory;
    size_t size;
    char *filepath;
    char *readBufferHeader;

    size_t curlDownloadLimit;
    ConnectionProps connProps;
    Progress2 prog;
    map<string, string> cabecerasResp;
} MemoryStruct;

class Httputil2 {
public:
    Httputil2();
    Httputil2(const Httputil2& orig);
    virtual ~Httputil2();

    static uint8_t aborted;
    size_t getDataLength(MemoryStruct *chunk){return chunk->size;}
    double getTimeout(MemoryStruct *chunk){return chunk->prog.timeout;}
    void setTimeout(MemoryStruct *chunk, int var){chunk->prog.timeout = var;}
    static void abortDownload(){aborted = 1;}
    static void setAborted(uint8_t pAborted){aborted = pAborted;}
    void setMaxBytesDownload(MemoryStruct *chunk, curl_off_t var){chunk->prog.maxBytesDownload = var;}
    double getDownloadProgress(MemoryStruct *chunk){return chunk->prog.progress;}
    double getDownloadSpeed(MemoryStruct *chunk){return chunk->prog.speedDown;}
    curl_off_t getDownloadTotal(MemoryStruct *chunk){return chunk->prog.bytesDown;}
    long getHttp_code(MemoryStruct *chunk){return chunk->connProps.http_code;}
    void setSendContentLength(MemoryStruct *chunk, uint8_t var){chunk->connProps.sendContentLength = var;}
    uint8_t getSendContentLength(MemoryStruct *chunk){return chunk->connProps.sendContentLength;}
    void setConnectionRetries(MemoryStruct *chunk, int connectionRetries){chunk->connProps.connectionRetries = connectionRetries;}
    int getConnectionRetries(MemoryStruct *chunk) {return chunk->connProps.connectionRetries;}

    void initProgress(Progress2 *p);
    void cleanData(MemoryStruct *p);
    MemoryStruct * initDownload();
    void cleanChunkData(MemoryStruct *chunk);
    void endDownload(MemoryStruct **chunk);
    void parserCabeceras(MemoryStruct *p);
    static int addDataToMem(void *contents, size_t realsize, MemoryStruct *mem);
    static size_t handleHeader(void *contents, size_t size, size_t nmemb, void *userp);
    static void checkWriteMemToFile(void *contents, size_t sizeToWrite, char *filepath);
    static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow);
    static size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream);
    int sendHttp(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, long httpType, MemoryStruct *chunk);
    int sendHttpWithRetries(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, long httpType, MemoryStruct *chunk);
    int download(std::string url, MemoryStruct *chunk);
    int downloadToDisk(std::string url, std::string pathfilename, MemoryStruct *chunk);
    int downloadToDiskHeaders(std::string url, std::string pathfilename, map<string, string> * headers, MemoryStruct *chunk);
    int httpGet(std::string url, map<string, string> * headers, MemoryStruct *chunk);
    int httpPostWithOffset(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, MemoryStruct *chunk);
    int httpPost(std::string url, const char* data, size_t tam, map<string, string> * headers, MemoryStruct *chunk);
    int httpPostStrlen(std::string url, const char *data, map<string, string> * headers, MemoryStruct *chunk);
    int httpPostDownload(std::string url, const char *data, map<string, string> * headers, MemoryStruct *chunk);
    int httpPut(std::string url, const char* data, size_t tam, size_t offset, map<string, string> * headers, MemoryStruct *chunk);
    int httpDel(std::string url, map<string, string> * headers, MemoryStruct *chunk);
    int writeMemToFile(char* path, MemoryStruct *chunk);
    char * getRawData(MemoryStruct *chunk);
    char * getData(MemoryStruct *chunk);
    void writeChunkToDisk(MemoryStruct *chunk);


private:

    static void decodeError(int r){
        char buff[100];
        strerror_s(buff, 100, r);
        printf("str_trim_left.error: %d %s\n", r, buff);
    }

    //std::mutex mutex;
    GMutex mutex;

};

#endif /* HTTPUTIL2_H */

