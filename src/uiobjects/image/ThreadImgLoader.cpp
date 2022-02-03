/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreadImgLoader.cpp
 * Author: Ryuk
 * 
 * Created on 6 de abril de 2020, 16:53
 */

#include "ThreadImgLoader.h"

/**
 * 
 */
ThreadImgLoader::ThreadImgLoader() {
    imageWorker = new ThreadImgWorker();
    thread = new Thread<ThreadImgWorker>(NULL, &ThreadImgWorker::startLoading);
}

/**
 * 
 */
ThreadImgLoader::~ThreadImgLoader() {
    if (imageWorker != NULL)
        delete imageWorker;
    if (thread != NULL)
        delete thread;
}

/**
 * 
 * @return 
 */
bool ThreadImgLoader::isRunning(){
    bool ret = false;
    if(thread != NULL){
        ret = imageWorker->isRunning();
    }
    return ret;
}

/**
 * 
 * @param listImages
 * @param screen
 */
bool ThreadImgLoader::startLoading(UIImgList *listImages, SDL_Surface *screen){
    bool ret = false;
    ret = !isRunning();
    
    if(ret){
        listImages->clearPrevImageCache();
//        Traza::print("Lanzando Thread de carga de imagenes", W_PARANOIC);
        imageWorker->setListImages(listImages);
        imageWorker->setScreen(screen);
        thread->setObject(imageWorker);
        thread->start();
        
        listImages->setLastIni(listImages->getPosIniLista());
        listImages->setLastEnd(listImages->getPosFinLista());
    } else {
//        Traza::print("ThreadImgLoader: Thread no ejecutado por estar en ejecucion", W_PARANOIC);
        imageWorker->stopLoading();
        
        int sumTime = 0;
        while (imageWorker->isRunning() && sumTime < 10){
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            sumTime += 1;
        }

        if (!imageWorker->isRunning()){
//            Traza::print("Relanzando Thread de carga de imagenes", W_PARANOIC);
            thread->start();
        } 
//        else {
//            Traza::print("Thread no lanzado", W_PARANOIC);
//        }
    }
    return ret;
}
