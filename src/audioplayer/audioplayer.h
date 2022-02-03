#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "Constant.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <cstdlib>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "SDL/SDL_ttf.h"
#include "math.h"
#include "Traza.h"
#include "gmutex.h"
#include "Dirutil.h"
#include "Menuobject.h"
#include "biquad.h"
#include "fft/Fft.h"

using namespace std;

 typedef enum { PLAYING,        //0
            PAUSED,             //1
            STOPED,             //2
            FINISHEDSONG,       //3
            FINISHEDALBUM,      //4
            PAUSEDTOLOADBUFFER, //5
            MAXPLAYSTATUS       //6
    } enumPlayerStatus;

struct Music {
       int currentPosition;
       Mix_Music *music;
};

class AudioPlayer
{
    public:
        /** Default constructor */
        AudioPlayer();
        /** Default destructor */
        virtual ~AudioPlayer();

        static struct TStreamMusicPlaying{
            Sint16 stream[2][BUFFERSPECTRUMVIS];
            int which;
            static const int bufSize = BUFFERSPECTRUMVIS;
            static bool canUpdateBuffer;
            int arrFreqVis[NBIQUADFILTERS];
        }streamMusicPlaying;

        void initAudio();
        uint32_t loadFile();
        static Fft *fastFourier;

        int loadFile(string filename);
        static void pause();
        static void stop();
        static void rewind();
        static void rewind(long millis);
        static void forward(long millis);
        static void volUp();
        static void volDown();

        static void analizarEspectro(void * _stream, int _len);

        void setLen(int len) { this->len = len; }
        int getLen() { return this->len; }
        void setBits(int bits) { this->bits = bits; }
        int getBits() { return this->bits; }
        void setSample_size(int sample_size) { this->sample_size = sample_size; }
        int getSample_size() { return this->sample_size; }
        void setPosition(int position) { this->position = position; }
        int getPosition() { return this->position; }
        void setRate(int rate) { this->rate = rate; }
        int getRate() { return this->rate; }
        void setDone(int done) { this->done = done; }
        int getDone() { return this->done; }
        void setNeed_refresh(int need_refresh) { this->need_refresh = need_refresh; }
        int getNeed_refresh() { return this->need_refresh; }
        void setMusicplaying(int musicplaying) { this->musicplaying = musicplaying; }
        int getMusicplaying() { return this->musicplaying; }
        void setScreen(SDL_Surface * screen) { this->screen = screen; }
        SDL_Surface * getScreen() { return this->screen; }
        void setFilename(string filename) { this->filename = filename; }
        string getFilename() { return this->filename; }
//        void setPaused(bool paused) { this->paused = paused; }
//        bool getPaused() { return this->paused; }
        void setActualPlayTime(long actualPlayTime) { this->actualPlayTime = actualPlayTime; }
        long getActualPlayTime() { return this->actualPlayTime; }
        void setInitialPlayTime(long initialPlayTime) { this->initialPlayTime = initialPlayTime; }
        long getInitialPlayTime() { return this->initialPlayTime; }
        TStreamMusicPlaying * getStreamMusicPlaying(){return &streamMusicPlaying;}
        void setPosicionInicial(long var){posicionInicial = var;}
        int getStatus(){return status;}
        void setStatus(int var){status = var;}
        void setPosicionCancion(long millis);
        void setViewSpectrum(bool var){viewSpectrum = var;}
        bool isViewSpectrum(){return viewSpectrum;}
        void setVol(int vol);
        void setSongDownloaded(bool var){songDownloaded = var;}
        bool isSongDownloaded(){return songDownloaded;}
        void setObjectsMenu(tmenu_gestor_objects *var){ObjectsMenu = var;}
        bool isEqualizerOn(){return equalizerOn;}
        void setEqualizerOn(bool var){equalizerOn = var;}
        bool isEqualizerVisible(){return equalizerVisible;}
        void setEqualizerVisible(bool var){equalizerVisible = var;}
        void setFilter(int nFilter, double gain);

    protected:
    private:
        static Biquad *lpFilterLeft[NBIQUADFILTERS];
        static Biquad *lpFilterRight[NBIQUADFILTERS];

        static void musicfinished();
        static void resumeMusic( Music * music );
        static void musicLengthCallback(int chan, void * _stream, int _len, void *udata);
        static int startSDLMusicPlayback(FILE *fp, SDL_RWops *rw2, Music * musica);
        int reloadSong();

        /* set this to any of 512,1024,2048,4096              */
        /* the lower it is, the more FPS shown and CPU needed */
        double audio_rate;
        static Uint16 audio_format;
        static int audio_channels;
        int audio_buffers;

//        static bool paused;
        static long actualPlayTime;
        static long initialPlayTime;
        static long pauseMoment;
        static long avance;
        static int volume;
        static int musicplaying;
        static SDL_Surface *screen;
        static int len, done, need_refresh, bits,sample_size, position, rate;
        static bool playFinished;
        static long posicionInicial;
        static string filename;
        static int status;
        static bool viewSpectrum;
        GMutex mutex;
        static bool songDownloaded;
        static bool equalizerOn;
        static bool equalizerVisible;

        static FILE *fp;
        static SDL_RWops *rw2;
        static Music musica;

        tmenu_gestor_objects *ObjectsMenu;
};

#endif // AUDIOPLAYER_H
