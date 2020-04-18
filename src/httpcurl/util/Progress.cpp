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

Progress::Progress(){ 
    init(0,1);
}

Progress::Progress(int posListThreads, int nThreads){ 
    init(posListThreads, nThreads);
}

Progress::~Progress(){
}


/**
 * 
 * @param posListThreads
 * @param nThreads
 */
void Progress::init(int posListThreads, int nThreads){ 
    this->posListThreads = posListThreads;
    this->nThreads = nThreads;
    progress = 0.0;
    bytesDown = 0;
    speedDown = 0;
    lastruntime = 0.0;
    lastBytesDownloaded = 0.0;
    lastruntimeBytesDown = 0.0;
    timeNoProgress = 0.0;
    lastProgress = 0.0;
}

void Progress::setMaxBytesDownload(std::ifstream::pos_type maxBytesDownload) {
    this->maxBytesDownload = maxBytesDownload;
}

std::ifstream::pos_type Progress::getMaxBytesDownload() {
    return maxBytesDownload;
}

void Progress::setLastBytesDownloaded(double lastBytesDownloaded) {
    this->lastBytesDownloaded = lastBytesDownloaded;
}

double Progress::getLastBytesDownloaded() {
    return lastBytesDownloaded;
}

void Progress::setLastruntimeBytesDown(double lastruntimeBytesDown) {
    this->lastruntimeBytesDown = lastruntimeBytesDown;
}

double Progress::getLastruntimeBytesDown() {
    return lastruntimeBytesDown;
}

void Progress::setLastruntime(double lastruntime) {
    this->lastruntime = lastruntime;
}

double Progress::getLastruntime() {
    return lastruntime;
}

void Progress::setTimeNoProgress(double timeNoProgress) {
    this->timeNoProgress = timeNoProgress;
}

double Progress::getTimeNoProgress() {
    return timeNoProgress;
}

void Progress::setLastProgress(double lastProgress) {
    this->lastProgress = lastProgress;
}

double Progress::getLastProgress() {
    return lastProgress;
}

void Progress::setTimeout(int timeout) {
    this->timeout = timeout;
}

int Progress::getTimeout() {
    return timeout;
}



