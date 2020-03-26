/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Progress.cpp
 * Author: Ryuk
 * 
 * Created on 20 de marzo de 2020, 11:13
 */

#include "Progress.h"

float * Progress::arrProgress;
double * Progress::arrBytesDown;
double * Progress::arrSpeedDown;


Progress::Progress(){ 
    Progress(0,1);
}
Progress::~Progress(){
    mutex.Lock();
    if (this->arrProgress != NULL){
        delete[] this->arrProgress;
        this->arrProgress = NULL;
    }
    if (this->arrBytesDown != NULL){
        delete[] this->arrBytesDown;
        this->arrBytesDown = NULL;
    }
    if (this->arrSpeedDown != NULL){
        delete[] this->arrSpeedDown;
        this->arrSpeedDown = NULL;
    }
    mutex.Unlock();
}

Progress::Progress(int posListThreads, int nThreads){ 
    mutex.Lock();
    this->posListThreads = posListThreads;
    this->nThreads = nThreads;
    this->arrProgress = new float[nThreads];
    this->arrBytesDown = new double[nThreads];
    this->arrSpeedDown = new double[nThreads];

    this->lastruntime = 0.0;
    this->lastBytesDownloaded = 0.0;
    this->lastruntimeBytesDown = 0.0;

    for (int i=0; i < nThreads; i++){
        this->arrProgress[i] = 0.0;
        this->arrBytesDown[i] = 0.0;
        this->arrSpeedDown[i] = 0.0;
    }
    mutex.Unlock();
}

