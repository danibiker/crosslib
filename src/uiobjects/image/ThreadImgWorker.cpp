/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ThreadImgWorker.cpp
 * Author: Ryuk
 *
 * Created on 7 de abril de 2020, 10:53
 */

#include <mutex>

#include "ThreadImgWorker.h"

volatile sig_atomic_t ThreadImgWorker::s_interrupted;
volatile bool ThreadImgWorker::running;

ThreadImgWorker::ThreadImgWorker() {
    mutex.Lock();
    numThreads = 4;
    running = false;
    s_interrupted  = 0;
    gestorIconos = new IcoGestor();
    mutex.Unlock();
}

ThreadImgWorker::~ThreadImgWorker() {
    s_interrupted = 1;
}

void ThreadImgWorker::my_handler(int sig){
    s_interrupted = 1;
    /* Reset handler to catch SIGINT next time.
    Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, ThreadImgWorker::my_handler);
}

void ThreadImgWorker::iniciarCtrlCHandler(){
    signal (SIGINT, ThreadImgWorker::my_handler);
}

bool ThreadImgWorker::stopLoading(){
    s_interrupted = 1;
    return true;
}

/**
 *
 * @return
 */
bool ThreadImgWorker::isRunning(){
    return running;
}

/**
 *
 * @param screen
 */
void ThreadImgWorker::setScreen(SDL_Surface* screen) {
    this->screen = screen;
}

/**
 *
 * @return
 */
SDL_Surface* ThreadImgWorker::getScreen() {
    return screen;
}

/**
 *
 * @param listImages
 */
void ThreadImgWorker::setListImages(UIImgList* listImages) {
    this->listImages = listImages;
}

/**
 *
 * @return
 */
UIImgList* ThreadImgWorker::getListImages() {
    return listImages;
}

/**
 *
 * @param numThreads
 */
void ThreadImgWorker::setNumThreads(int numThreads) {
    this->numThreads = numThreads;
}

/**
 *
 * @return
 */
int ThreadImgWorker::getNumThreads() const {
    return numThreads;
}

/**
 *
 * @param listImages
 * @param screen
 */
uint32_t ThreadImgWorker::startLoading(){
    mutex.Lock();
    bool ret = !isRunning();
    bool salir = false;
    int i=0;

    if(ret && listImages != NULL){
        running = true;
        s_interrupted  = 0;
//        Traza::print("Lanzando Thread de carga de imagenes", W_PARANOIC);

        Thread<ImageLoader> **arrThread = new Thread<ImageLoader>* [numThreads];
        ImageLoader **arrImgLoader = new ImageLoader* [numThreads];
        int contadorReintentos[numThreads];

        for (int i=0; i < numThreads; i++){
            arrImgLoader[i] = NULL;
            arrThread[i] = new Thread<ImageLoader>(NULL, &ImageLoader::loadImage);
            contadorReintentos[i] = 0;
        }

        int imgToLoad = listImages->getPosIniLista();
        int imgToEnd = listImages->getPosFinLista();
//        int imgToLoad = 0;
//        int imgToEnd = listImages->getSize() - 1;

//        Traza::print("loading from: " + Constant::TipoToStr(imgToLoad) + " to: " + Constant::TipoToStr(imgToEnd), W_PARANOIC);
        string pruebaImageStuck;

        while (!salir && imgToLoad >= 0){
            Traza::print("imagen a cargar",imgToLoad,  W_PARANOIC);
            if (!arrThread[i]->isRunning() || ( arrImgLoader[i] != NULL && arrImgLoader[i]->getEstado() == 0)){
            //if (arrImgLoader[i] == NULL || (arrImgLoader[i] != NULL && arrImgLoader[i]->getEstado() >= 0) ){
                if (imgToLoad <= imgToEnd && imgToLoad < listImages->getSize() && !s_interrupted){

                    if (arrImgLoader[i] != NULL){
                        delete arrImgLoader[i];
                        arrImgLoader[i] = NULL;
                        delete arrThread[i];
                    }
                    arrImgLoader[i] = new ImageLoader();
                    arrThread[i] = new Thread<ImageLoader>(NULL, &ImageLoader::loadImage);

                    arrImgLoader[i]->setListImages(listImages);
                    arrImgLoader[i]->setScreen(&screen);
                    arrImgLoader[i]->setImagetoload(imgToLoad);
                    arrImgLoader[i]->setGestorIconos(&gestorIconos);

                    arrThread[i]->setObject(arrImgLoader[i]);

                    if (arrImgLoader[i] != NULL){
                        arrThread[i]->start();
                        //arrThread[i]->join();
                    }
//                    else {
//                        Traza::print("Imagen no cargada para", imgToLoad, W_PARANOIC);
//                    }
                    imgToLoad++;
                }
                else {
                    salir = true;
                    for (int j=0; j < numThreads; j++){
                        if (arrImgLoader[j] != NULL)
                        if (arrImgLoader[j]->getEstado() == 1){
                            salir = false;
                            if (imgToLoad > imgToEnd || imgToLoad >= listImages->getSize() || s_interrupted){
                                pruebaImageStuck = listImages->getValue(arrImgLoader[j]->getImagetoload());;
                                Traza::print("arrThread: " + pruebaImageStuck + " still running", W_PARANOIC);
                                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                contadorReintentos[j]=contadorReintentos[j] + 1;
                            } else {
                                contadorReintentos[j] = 0;
                            }
                            if (contadorReintentos[j] >= 500){
                                salir = true;
                                Traza::print("arrThread: " + pruebaImageStuck + " Salida por reintentos", W_PARANOIC);
                            }
                        }
                    }
                }
            }
//                else {
//                Traza::print("thread ocupado", i,  W_PARANOIC);
//            }
            i = (i + 1) % getNumThreads();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
//            Traza::print("Bucle: " + Constant::TipoToStr(imgToLoad) + ":" + Constant::TipoToStr(imgToEnd), W_PARANOIC);
        }

//        Traza::print("startDownload: Destruyendo threads principales", W_PARANOIC);
        //destrucción del array
        for (int i=0; i<numThreads; i++){
            delete arrImgLoader[i];
            delete arrThread[i];
        }
//        Traza::print("startDownload: Destruyendo ok", W_PARANOIC);
//        Traza::print("startDownload: Destruyendo arrays principales", W_PARANOIC);
        delete[] arrImgLoader;
        delete[] arrThread;
    }
//        else {
//        Traza::print("ThreadImgWorker: Thread no ejecutado por estar en ejecucion", W_PARANOIC);
//    }

    running = false;
    s_interrupted = 0;
    mutex.Unlock();
//    Traza::print("ThreadImgWorker startDownload: Saliendo", W_PARANOIC);
    return 0;
}
