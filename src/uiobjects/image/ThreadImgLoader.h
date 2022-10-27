/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ThreadImgLoader.h
 * Author: Ryuk
 *
 * Created on 6 de abril de 2020, 16:53
 */

#ifndef THREADIMGLOADER_H
#define THREADIMGLOADER_H

#include "uiobjects/uiimglist.h"
#include "uiobjects/image/ThreadImgWorker.h"
#include "uiobjects/thread.h"

class ThreadImgLoader {
public:
    ThreadImgLoader();
    virtual ~ThreadImgLoader();
    bool startLoading(UIImgList *listImages, SDL_Surface *screen);
    bool isRunning();
private:
    ThreadImgWorker *imageWorker;
    Thread<ThreadImgWorker> *thread;
};

#endif /* THREADIMGLOADER_H */

