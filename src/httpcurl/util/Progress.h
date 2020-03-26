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
        
        Progress(int posListThreads, int nThreads);
        
        void setProgress(float progress) { 
            if (this->arrProgress != NULL){
                this->arrProgress[this->posListThreads % this->nThreads] = progress; 
            }
        }
        
        float getProgress(){
            if (this->arrProgress != NULL){
                return this->arrProgress[this->posListThreads % this->nThreads];
            } else {
                return 0.0;
            }
        }
        
        void setDlSizeBytes(double bytes) { 
            if (this->arrBytesDown != NULL){
                this->arrBytesDown[this->posListThreads % this->nThreads] = bytes; 
            }
        }
        
        double getDlSizeBytes(){
            if (this->arrBytesDown != NULL){
                return this->arrBytesDown[this->posListThreads % this->nThreads];
            } else {
                return 0.0;
            }
        }
                
        void setDlSpeed(double speed) { 
            if (this->arrSpeedDown != NULL){
                this->arrSpeedDown[this->posListThreads % this->nThreads] = speed; 
            }
        }
        
        double getDlSpeed(){
            if (this->arrSpeedDown != NULL){
                return this->arrSpeedDown[this->posListThreads % this->nThreads];
            } else {
                return 0.0;
            }
        }
        double lastruntime;

        double lastruntimeBytesDown;
        double lastBytesDownloaded;
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
    static double *arrBytesDown;
    static double *arrSpeedDown;
    int nThreads;
    int posListThreads;
    GMutex mutex;
};

#endif /* PROGRESS_H */

