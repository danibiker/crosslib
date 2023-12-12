/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ThreadImgWorker.h
 * Author: Ryuk
 *
 * Created on 7 de abril de 2020, 10:53
 */

#ifndef THREADIMGWORKER_H
#define THREADIMGWORKER_H

#include "uiobjects/uiimglist.h"
#include "uiobjects/image/ImageLoader.h"
#include "uiobjects/thread.h"
#include <SDL/SDL.h>
#include <thread>
#include <chrono>
#include <csignal>

class ThreadImgWorker {
public:
    ThreadImgWorker();
    virtual ~ThreadImgWorker();
    uint32_t startLoading();
    static void my_handler(int sig);
    void setScreen(SDL_Surface* screen);
    SDL_Surface* getScreen();
    void setListImages(UIImgList* listImages);
    UIImgList* getListImages();
    void setNumThreads(int numThreads);
    int getNumThreads() const;
    bool isRunning();
    bool stopLoading();

private:
    UIImgList *listImages;
    SDL_Surface *screen;
    int numThreads;
    static volatile bool running;
    static volatile sig_atomic_t s_interrupted ;
    void iniciarCtrlCHandler();
    GMutex mutex;
    IcoGestor *gestorIconos;

};

#endif /* THREADIMGWORKER_H */

