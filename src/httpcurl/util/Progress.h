/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Progress.h
 * Author: Ryuk
 *
 * Created on 20 de marzo de 2020, 11:13
 */

#ifndef PROGRESS_H
#define PROGRESS_H

using namespace std;

#include <curl/curl.h>
#include <gmutex.h>
#include <fstream>

class Progress {
 public:
        Progress();
        ~Progress();

        void setProgress(float progress) { 
                this->progress = progress; 
        }
        
        float getProgress(){
                return this->progress;
        }
        
        void setDlSizeBytes(double bytes) { 
            this->bytesDown = bytes; 
        }
        
        double getDlSizeBytes(){
                return this->bytesDown;
        }
                
        void setDlSpeed(double speed) { 
            this->speedDown = speed; 
        }
        
        double getDlSpeed(){
                return this->speedDown;
        }
        CURL *curl;
        void setMaxBytesDownload(std::ifstream::pos_type maxBytesDownload);
        std::ifstream::pos_type getMaxBytesDownload();
        void setLastBytesDownloaded(double lastBytesDownloaded);
        double getLastBytesDownloaded();
        void setLastruntimeBytesDown(double lastruntimeBytesDown);
        double getLastruntimeBytesDown();
        void setLastruntime(double lastruntime);
        double getLastruntime();
        void setTimeNoProgress(double timeNoProgress);
        double getTimeNoProgress();
        void setLastProgress(double lastProgress);
        double getLastProgress();
        void setTimeout(int timeout);
        int getTimeout();
        
        
        
        
 private:
    float progress;
    double bytesDown;
    double speedDown;
    double lastruntime;
    double lastruntimeBytesDown;
    double lastBytesDownloaded;
    double timeNoProgress;
    double lastProgress;
    int timeout;
    std::ifstream::pos_type maxBytesDownload;
    void init();
};

#endif /* PROGRESS_H */

