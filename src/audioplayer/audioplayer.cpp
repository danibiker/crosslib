#include "audioplayer.h"


int AudioPlayer::musicplaying;
SDL_Surface * AudioPlayer::screen;
int AudioPlayer::len;
int AudioPlayer::done;
int AudioPlayer::need_refresh;
int AudioPlayer::bits;
//int AudioPlayer::streamMusicPlayingwhich=0;
int AudioPlayer::sample_size;
int AudioPlayer::position;
int AudioPlayer::rate;
//Sint16 AudioPlayer::stream[2][BUFFER*2*2];
//bool AudioPlayer::paused;
long AudioPlayer::initialPlayTime;
long AudioPlayer::pauseMoment;
long AudioPlayer::avance;
long AudioPlayer::actualPlayTime;
int AudioPlayer::volume;
struct AudioPlayer::TStreamMusicPlaying AudioPlayer::streamMusicPlaying;
long AudioPlayer::posicionInicial;
int AudioPlayer::status = STOPED;
bool AudioPlayer::viewSpectrum;
bool AudioPlayer::songDownloaded;
string AudioPlayer::filename;
FILE *AudioPlayer::fp;
SDL_RWops *AudioPlayer::rw2;
Music AudioPlayer::musica;
bool AudioPlayer::equalizerOn;
bool AudioPlayer::equalizerVisible;
const long BUFFERSIZE = 100*1024; //Esperamos a que se carguen 100kbytes
const long MAXTIEMPOESPERABUFFER = 20000;
Biquad *AudioPlayer::lpFilterLeft[NBIQUADFILTERS];
Biquad *AudioPlayer::lpFilterRight[NBIQUADFILTERS];
Uint16 AudioPlayer::audio_format;
int AudioPlayer::audio_channels;
bool AudioPlayer::TStreamMusicPlaying::canUpdateBuffer;
Fft *AudioPlayer::fastFourier;

/**
* Constructor
*/
AudioPlayer::AudioPlayer(){
    audio_rate = 44100.0;
    audio_format = AUDIO_S16SYS;
    audio_channels = 2;
    audio_buffers = 4096;
    volume = SDL_MIX_MAXVOLUME;
    len=BUFFERSPECTRUMVIS*2*2;
    screen=NULL;
    viewSpectrum = true;
    actualPlayTime = 0;
    initialPlayTime = 0;
    pauseMoment = 0;
    avance = 0;
    need_refresh=0;
    bits=0;
    done=0;
    sample_size=0;
    position=0;
    rate=0;
    musicplaying = 1;
    posicionInicial = 0;
    status = STOPED;
    equalizerOn = true;
    equalizerVisible = false;
    //Por defecto suponemos que la cancion esta descargada correctamente
    songDownloaded = true;
    streamMusicPlaying.canUpdateBuffer = true;

    if (screen != NULL){
        Traza::print("CAUTION: Screen is null", W_ERROR);
        cerr << "CAUTION: Screen is null" << endl;
    }

    //http://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/
    for (int i=0; i < NBIQUADFILTERS; i++){
        lpFilterLeft[i] = new Biquad();	 // create a Biquad, lpFilter;
        lpFilterRight[i] = new Biquad(); // create a Biquad, lpFilter;
        setFilter(i, 0.0);
    }

    fastFourier = new Fft(audio_buffers, audio_rate);
    for (int i = 0; i < NBIQUADFILTERS; i++){
        streamMusicPlaying.arrFreqVis[i] = 0;
    }

}

/**
*
*/
void AudioPlayer::setFilter(int nFilter, double gain){
    Traza::print("setFilter" + Constant::TipoToStr(nFilter) + " gain: ", gain, W_DEBUG);
    if (nFilter > 0 && nFilter < NBIQUADFILTERS){
        lpFilterLeft[nFilter]->setBiquad(bq_type_peak, frecsEQ[nFilter] / audio_rate, QFrecsEQ[nFilter], gain);
        lpFilterRight[nFilter]->setBiquad(bq_type_peak, frecsEQ[nFilter] / audio_rate, QFrecsEQ[nFilter], gain);
    }
}


/**
* Destructor
*/
AudioPlayer::~AudioPlayer(){
//    Mix_HaltMusic();
//    Mix_CloseAudio();
}

/**
*
*/
uint32_t AudioPlayer::loadFile(){
    loadFile(filename);
    return 0;
}

/**
*
*/
void AudioPlayer::musicLengthCallback(int chan, void * _stream, int _len, void *udata){
    Music *music = static_cast<Music *>(udata);
    music->currentPosition += len;

    /**
    * you open initialize SDL_mixer with 2 channels and signed 16 bit audio format.
    *
    * This is very important to know, because that will be the format of the stream
    * pointer in your function callback.
    *
    * The data that comes in stream is pointer of size len bytes, but your data is
    * 16 bit (2 bytes), then cast to a short pointer, and the size will be len/2.
    *
    * Since you initialized with 2 channels (stereo), given i a positive number
    * multiple of 2 (0,2,4,6,8,10,...), then p[i] is the sound data for left channel,
    * and p[i+1] is data for right channel.
    */

    short* p = (short*) _stream;
    int length = len/2;

    // filter a buffer of input samples, in-place
    if (equalizerOn){
        for (int i = 0; i < length/2; i++) {
            //Procesando los canales mediante filtros biquad en cascada
            for (int j = 0; j < NBIQUADFILTERS; j++){
                p[i*2] = lpFilterLeft[j]->process(p[i*2]);
                p[i*2+1] = lpFilterRight[j]->process(p[i*2+1]);
            }
        }
    }

    analizarEspectro(_stream, _len);

    position += _len/sample_size;

    if (viewSpectrum && !equalizerVisible && streamMusicPlaying.canUpdateBuffer){
        if(need_refresh)
            return;
        /* save the stream buffer and indicate that we need a redraw */
        len=_len;
        if (screen != NULL){
            //len indica el numero de bytes en el stream. Resulta que en el stream hay 2 canales
            //y en cada canal las muestras son de 16 bits. Por tanto, si tenemos un len de 16384
            //--> 16384 = 4096 muestras/canal * 2 canales * 2 bytes
            long bytesPorMuestra = ((audio_format & 0x0FFF) >> 3);
            long nMuestras = len / audio_channels / bytesPorMuestra;
//            Traza::print("len : " + Constant::TipoToStr(len) + ", screen->w*4 : " + Constant::TipoToStr(screen->w*4)
//                         + ", nMuestras: " + Constant::TipoToStr(nMuestras), W_INFO);
//
//            Traza::print("audio_rate_obtained: " + Constant::TipoToStr(rate)
//                         + ", sample_size: " + Constant::TipoToStr(sample_size), W_INFO);
            //Se copia un canal cada vez que se pasa por este metodo.
            //Cuidado porque el tamanyo tiene que pasarse en bytes y nuestro stream es de 16bits
            memcpy(streamMusicPlaying.stream[(streamMusicPlaying.which+1)%2],_stream,
                   nMuestras >= BUFFERSPECTRUMVIS ? BUFFERSPECTRUMVIS * bytesPorMuestra : nMuestras * bytesPorMuestra);
            streamMusicPlaying.which=(streamMusicPlaying.which+1)%2;
        }
        need_refresh=1;
    }
}

/**
*
*/
void AudioPlayer::analizarEspectro(void * _stream, int _len){
    fastFourier->CopyIn(_stream, _len);
    fastFourier->Transform();

    double freqL;
    double freqR;

    for (int i=0; i < NBIQUADFILTERS; i++){
        freqL = i > 0 ? ((frecsEQ[i] - frecsEQ[i - 1]) / 2.0 + frecsEQ[i - 1]) : 0;
        freqR = (i < NBIQUADFILTERS - 1 && i > 0) ? ((frecsEQ[i + 1] - frecsEQ[i]) / 2.0 + frecsEQ[i]) :
                                         (frecsEQ[i] - frecsEQ[i - 1]) / 2.0 + frecsEQ[i];

        //cout << "freqL: " << freqL << ", freqR: " << freqR << endl;
        streamMusicPlaying.arrFreqVis[i] = 0;
        //Sumamos a cada 100Hz
        for (int hz=freqL; hz < freqR; hz+=100){
            streamMusicPlaying.arrFreqVis[i] += static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(hz))) / 128;
        }

        //cout << frecsEQ[i] << ": " << streamMusicPlaying.arrFreqVis[i] << "; ";
    }
    //cout << endl;

    /**
    cout << "1000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(1000))) / 128
    << ", 2000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(2000))) / 128
    << ", 4000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(4000))) / 128
    << ", 8000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(8000))) / 128
    << ", 10000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(10000))) / 128
    << ", 1100: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(1100))) / 128
    << ", 900: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(900))) / 128
    << ", 20000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(20000))) / 128
    << ", 30000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(30000))) / 128
    << ", 40000: " << static_cast<int> (fastFourier->GetIntensity(fastFourier->HzToPoint(40000))) / 128
    << endl;
    */


}

/**
*
*/
void AudioPlayer::pause(){
    if(status == PLAYING){
        pauseMoment = SDL_GetTicks();
        //Mix_UnregisterEffect(MIX_CHANNEL_POST, &AudioPlayer::musicLengthCallback);
        Mix_PauseMusic();
        status = PAUSED;
    } else {
        initialPlayTime += SDL_GetTicks() - pauseMoment;
        Mix_ResumeMusic();
        status = PLAYING;
    }
}

/**
*
*/
void AudioPlayer::stop(){
    if (musicplaying == 0){
        status = STOPED;
    }
    musicplaying = 0;
    posicionInicial = 0;
}

/**
*
*/
void AudioPlayer::rewind(){
    Mix_RewindMusic();
}

/**
*
*/
void AudioPlayer::setPosicionCancion(long millis){
    Mix_RewindMusic();
    double pos = millis / (double)1000;
    if (Mix_SetMusicPosition(pos) == -1) {
        Traza::print("Error AudioPlayer::setPosicionCancion", pos, W_ERROR);
    } else {
        this->avance = millis - (SDL_GetTicks() - this->initialPlayTime);
    }
}

/**
*
*/
void AudioPlayer::rewind(long millis){
    if (actualPlayTime > millis){
        double pos = (actualPlayTime - millis)/(double)1000;
        Mix_RewindMusic();
        if (Mix_SetMusicPosition(pos) != -1)
            avance -= millis;
    } else {
        Mix_RewindMusic();
        if (Mix_SetMusicPosition(0) != -1)
            avance = -(SDL_GetTicks() - initialPlayTime);
    }
}

/**
*
*/
void AudioPlayer::forward(long millis){
    long salto = actualPlayTime + millis;
    Mix_RewindMusic();
    double pos = ceil(salto / (float)1000);
    if (Mix_SetMusicPosition(pos) != -1){
        avance += millis;
    } else {
        Traza::print("error avanzando", W_ERROR);
    }
}

/**
*
*/
void AudioPlayer::volUp(){
    volume=(volume+1)<<1;
    if(volume>SDL_MIX_MAXVOLUME)
        volume=SDL_MIX_MAXVOLUME;
    Mix_VolumeMusic(volume);
}

/**
*
*/
void AudioPlayer::volDown(){
    volume>>=1;
    Mix_VolumeMusic(volume);
}

void AudioPlayer::setVol(int vol){
    volume=vol;
    if(volume>SDL_MIX_MAXVOLUME)
        volume=SDL_MIX_MAXVOLUME;
    Mix_VolumeMusic(volume);
}

/**
*
*/
void AudioPlayer::resumeMusic( Music * music ){
    music->currentPosition = 0;
    Mix_RegisterEffect(MIX_CHANNEL_POST, &musicLengthCallback,0, music);
}

/**
*
*/
void AudioPlayer::musicfinished() {
    Traza::print("AudioPlayer::musicfinished: " + string(songDownloaded ? "S":"N"), W_DEBUG);
    if (songDownloaded == true){
        musicplaying = 0;
        status = FINISHEDSONG;
    } else if(status == PLAYING){
        pause();
        status = PAUSEDTOLOADBUFFER;
    }
}

/**
*
*/
void AudioPlayer::initAudio(){
     if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        Traza::print("No se puede inicializar el dispositivo de audio: " + string(Mix_GetError()), W_ERROR);
        exit (1);
    }
}

/**
* Carga un fichero de musica y empieza a reproducirlo
*/
int AudioPlayer::loadFile(string filename){
//    mutex.Lock();
    musicplaying = 1;
    this->status = PLAYING;
    this->avance = 0;
    this->actualPlayTime = 0;
    this->initialPlayTime = SDL_GetTicks();

    Traza::print("AudioPlayer::loadFile: " + filename, W_DEBUG);

    //Cargamos la cancion y empezamos la reproduccion
    reloadSong();

    //Esto se utiliza cuando recargamos la cancion para que no se pierda el lugar actual
    //de la reproduccion
    if (posicionInicial > 0){
        Traza::print("Recargando cancion en la posicion", posicionInicial,W_DEBUG);
        forward(posicionInicial);
        posicionInicial = 0;
    }

    //Hacemos el OR para que la transicion al recargar la cancion cuando esta
    //totalmente descargada no se note. Esto ocurre porque el metodo Mix_SetMusicPosition
    //solo acepta segundos, por lo que solo abortamos la reproduccion en los multiplos
    //de mil que representan un segundo.
    //EDIT: Esto ya no es del todo necesario porque no se recarga la cancion cuando
    //se ha terminado la descarga, se hace ahora cuando se pulsa en la barra de desplazamiento
    //si es que la cancion se habia descargado ya
    while(musicplaying == 1 || (posicionInicial == 0 && musicplaying == 0 && this->actualPlayTime % 1000 != 0)) {
        if (status == PAUSEDTOLOADBUFFER){
            reloadSong();
        } else if (status == PAUSED){
            SDL_Delay(200);
        } else {
            this->actualPlayTime = SDL_GetTicks() - this->initialPlayTime + this->avance;
            SDL_Delay(1);
        }
    }

    Traza::print("Closing resources in function loadFile", W_DEBUG);
    Mix_PauseMusic();
    Mix_HaltMusic();
    Mix_UnregisterEffect(MIX_CHANNEL_POST, &musicLengthCallback);
    SDL_RWclose(rw2); /* Automatically does an fclose(fp) in this case */
    musica.music = NULL; // so we know we freed it...
//    mutex.Unlock();
    Traza::print("Exiting audio player in function loadFile", W_DEBUG);
    if (musicplaying == 0){
        status = STOPED;
    }
    return 0;
}

/**
*
*/
int AudioPlayer::reloadSong(){

    long tiempoActual = 0;
    //Si habiamos parado la reproduccion porque no se ha bajado del todo la cancion,
    //paramos la musica y reseteamos variables
    if (status == PAUSEDTOLOADBUFFER){
        Dirutil dir;
        std::ifstream::pos_type tam_loaded = dir.filesize(filename.c_str());
        Traza::print("AudioPlayer::musicfinished. Detectado fin prematuro del fichero con tam: ", tam_loaded, W_DEBUG);
        tiempoActual = floor(this->actualPlayTime / (double)1000)*1000;
        long inicioPausa = SDL_GetTicks();

        //Esperamos a que se carguen BUFFERSIZE bytes durante un
        //tiempo maximo de MAXTIEMPOESPERABUFFER milisegundos
        std::ifstream::pos_type bufferFilled = 0;

        string labelStatusAnt = ObjectsMenu->getObjByName("statusMessage")->getLabel();

        while ((bufferFilled = dir.filesize(filename.c_str()) - tam_loaded) < BUFFERSIZE &&
                SDL_GetTicks() - inicioPausa < MAXTIEMPOESPERABUFFER &&
                !songDownloaded)
        {
            ObjectsMenu->getObjByName("statusMessage")->setLabel("Cargando... "
                            + Constant::TipoToStr(ceil(bufferFilled /(double)BUFFERSIZE * 100)) + "% "
                            + " " + labelStatusAnt);
            SDL_Delay(200);
        }

        ObjectsMenu->getObjByName("statusMessage")->setLabel(labelStatusAnt);

        Traza::print("AudioPlayer::loadFile. Buffer llenado", BUFFERSIZE, W_DEBUG);
        pause();

        Mix_HaltMusic();
        Mix_UnregisterEffect(MIX_CHANNEL_POST, &musicLengthCallback);
        SDL_RWclose(rw2); /* Automatically does an fclose(fp) in this case */
    }

    //Aqui abrimos el fichero descargado parcialmente
    fp = fopen(filename.c_str(), "rb");
    rw2 = SDL_RWFromFP(fp, SDL_TRUE);
    musica.music = Mix_LoadMUS_RW(rw2);

    //Realizamos comprobaciones de que se puede reproducir el archivo
    if (musica.music == NULL) {
        Traza::print("No se puede cargar el archivo: " + string(Mix_GetError()), W_ERROR);
        return 1;
    }


    if (Mix_PlayMusic(musica.music, 0) == -1) {
        Traza::print("No se puede reproducir el archivo: " + string(Mix_GetError()), W_ERROR);
        return 1;
    }
    //Es necesario hacer un rewind inmediato despues de cargar, si es un mp3. Sino se producen
    //errores de SDL abortando la reproduccion. Esto pasa al reproducir ficheros en local.
    //Los ficheros ogg no estan afectados
    Mix_RewindMusic();

    /**
    * Temas para el dibujado del audio por el espectrografo
    */
    int audio_rate_obtained,audio_channels_obtained;
    Uint16 audio_format_obtained;
    /* print out some info on the audio device and stream */
    Mix_QuerySpec(&audio_rate_obtained, &audio_format_obtained, &audio_channels_obtained);
    bits=audio_format_obtained&0xFF;
    sample_size=bits/8+audio_channels_obtained;
    rate=audio_rate_obtained;
    //Iniciamos la reproduccion
    resumeMusic(&musica);
    //Cuando termine, actualiza el estado del reproductor a parado
    Mix_HookMusicFinished(musicfinished);

    if (tiempoActual > 0){
        //Especificamos el tiempo en el que estaba la cancion antes de detenerse
        setPosicionCancion(tiempoActual);
    }

    return 0;
}
