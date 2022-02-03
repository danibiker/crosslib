#include "Ioutil.h"
#include "font/Arimo_Regular.ttf.h"
#include "uiimglist.h"


#ifdef WIN
    const int SCREEN_MODE = SDL_SWSURFACE|SDL_RESIZABLE; // SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_SWSURFACE|SDL_RESIZABLE
#elif UNIX
    //const int SCREEN_MODE = SDL_SWSURFACE|SDL_FULLSCREEN; // SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_SWSURFACE|SDL_RESIZABLE
    const int SCREEN_MODE = SDL_SWSURFACE|SDL_RESIZABLE; // SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN|SDL_SWSURFACE|SDL_RESIZABLE
    //const int SCREEN_MODE = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
#endif // UNIX
const int SURFACE_MODE = SDL_SWSURFACE;
#define Y(sample) (((sample)*H)/4/0x7fff)

/**
*
*/
Ioutil::Ioutil(){
    canFlip = true;
    autosize = true;
    frames = 0;
    mediaFps = 0;
    time = 0;
    totalMuestreo = 0;
    lastTime = SDL_GetTicks();
    font = NULL;
    RWFont = NULL;
    screenShotSurface = NULL;
    screenShotThumbSurface = NULL;
    fontAscent = 0;
    fontDescent = 0;
    fontHeight = 0;
    moveSurface = NULL;
    mensajeAuto = "";
    fontfile = new Fileio();
    gestorIconos = new IcoGestor();
    loadFontFromFile(FONT_TYPE);
    WINDOW_WIDTH_FULLSCREEN = 0;
    WINDOW_HEIGHT_FULLSCREEN = 0;
    SCREEN_BITS_FULLSCREEN = 0;
    this->w = 0;
    this->h = 0;
    this->fullsflags = SCREEN_MODE;
    ignoreButtonRepeats = false;
    clearEvento(&evento);
    lastEvento.keyjoydown = false;
    initSDL(true);
    dirInicial = Constant::getAppDir();

    int corte = ceil((ALBUMWIDTH/4)/(float)3);
    color1Spectrum.calcDegradation(cAzulTotal, cVerde, corte);
    color2Spectrum.calcDegradation(cVerde, cAmarillo, corte);
    color3Spectrum.calcDegradation(cAmarillo, cRojo , corte);
    
    imgLoader = new ThreadImgLoader();
    Traza::print("Dir Inicial: " + dirInicial, W_DEBUG);
}

/**
*
*/
Ioutil::~Ioutil(){
    Traza::print("Destructor de Ioutil", W_DEBUG);
    Traza::print("Liberando capturas de pantalla", W_DEBUG);
    if (screenShotSurface != NULL) SDL_FreeSurface( screenShotSurface );
    Traza::print("Liberando capturas de thumbs", W_DEBUG);
    if (screenShotThumbSurface != NULL) SDL_FreeSurface( screenShotThumbSurface );
    Traza::print("Liberando iconos", W_DEBUG);
    delete gestorIconos;
    Traza::print("Liberando Joysticks", W_DEBUG);
    if(mJoysticks != NULL){
		delete[] mJoysticks;
		mJoysticks = NULL;

		delete[] mPrevAxisValues;
		mPrevAxisValues = NULL;

		delete [] mPrevHatValues;
		mPrevHatValues = NULL;
	}
	Traza::print("Cerrando recursos de SDL", W_DEBUG);
    killSDL();
    Traza::print("Liberando fuentes de texto", W_DEBUG);
    delete fontfile;
    delete imgLoader;
    Traza::print("Destructor de ioutil FIN", W_DEBUG);

}

/**
*
*/
void Ioutil::initSDL(bool calcFS){
    Traza::print("Iniciando SDL", W_DEBUG);
    #ifdef WIN
        dpi();
    #endif

    SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);			// Initialize SDL

    //Audio Stuff
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
    int volume = SDL_MIX_MAXVOLUME;
    int bufLen = 100*1024; //Son 100KB
    char *buffer;
    size_t FileLenght = 0;
    //Audio Stuff
    //Iniciamos la rutina para buscar Joysticks
    Traza::print("Buscando joysticks", W_DEBUG);
    mNumJoysticks = SDL_NumJoysticks();
    mJoysticks = new SDL_Joystick*[mNumJoysticks];
    mPrevAxisValues = new std::map<int, int>[mNumJoysticks];
    mPrevHatValues = new std::map<int, int>[mNumJoysticks];
    int axis = 0;
    int hats = 0;

    for(int i = 0; i < mNumJoysticks; i++){
        mJoysticks[i] = SDL_JoystickOpen(i);
        axis = SDL_JoystickNumAxes(mJoysticks[i]);
        hats = SDL_JoystickNumHats(mJoysticks[i]);
        //cout << "hay " + Constant::TipoToStr(axis) + " axis en el joystick: " + Constant::TipoToStr(i) << endl;
        for(int k = 0; k < axis; k++){
            mPrevAxisValues[i][k] = 0;
        }
        for(int k = 0; k < hats; k++){
            mPrevHatValues[i][k] = 0;
        }
    }
    JoyMapper::initJoyMapper();
    Traza::print("Joystick abierto", W_DEBUG);

    screen = NULL;
    //Iniciamos en modo Fullscreen para saber la resolucion del escritorio
    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
    WINDOW_WIDTH_FULLSCREEN = videoInfo->current_w;
    WINDOW_HEIGHT_FULLSCREEN = videoInfo->current_h;
    SCREEN_BITS_FULLSCREEN = videoInfo->vfmt->BitsPerPixel ;

    if(screen == NULL){
        //Comprobamos si es la primera vez que arrancamos la aplicacion y cargamos los valores definidos en el
        //fichero de configuracion
        try{
            if (this->w == 0 || this->h == 0){
                string configIniFile = Constant::getAppDir() + Constant::getFileSep() + "config.ini";
                ListaIni<Data> *config = new ListaIni<Data>();
                Traza::print("Cargando configuracion", W_DEBUG);
                config->loadFromFile(configIniFile);
                config->sort();
                this->w = Constant::strToTipo<int>(config->get(config->find("width")).getValue());
                this->h = Constant::strToTipo<int>(config->get(config->find("height")).getValue());
                if (Constant::strToTipo<int>(config->get(config->find("fullscreen")).getValue()) == 1)
                    this->fullsflags = this->fullsflags | SDL_FULLSCREEN;
                Constant::setTrazaLevel(Constant::strToTipo<int>(config->get(config->find("loglevel")).getValue()));
                Constant::setExecMethod(Constant::strToTipo<int>(config->get(config->find("execMethod")).getValue()));

                string pip = config->find("proxyip") >= 0 ? config->get(config->find("proxyip")).getValue() : "";
                string ppt = config->find("proxyport") >= 0 ? config->get(config->find("proxyport")).getValue() : "";
                string pu = config->find("proxyuser") >= 0 ? config->get(config->find("proxyuser")).getValue() : "";
                string pp = config->find("proxypass") >= 0 ? config->get(config->find("proxypass")).getValue() : "";
                Constant::setPROXYIP(pip);
                Constant::setPROXYPORT(ppt);
                Constant::setPROXYUSER(pu);
                Constant::setPROXYPASS(pp);
                delete config;
            }
        } catch (Excepcion &e){
            Traza::print("Error al cargar la configuracion", W_ERROR);
            this->w = 640;
            this->h = 480;
        }

        //En caso de que no se especifique una resolucion de pantalla, obtenemos la del sistema operativo
        Uint8 bpp = SCREEN_BITS;
        if (this->w == 0 || this->h == 0){
            bpp = SCREEN_BITS_FULLSCREEN;
            this->w = WINDOW_WIDTH_FULLSCREEN;
            this->h = WINDOW_HEIGHT_FULLSCREEN;
        }
        Traza::print("Ancho", this->w, W_DEBUG);
        Traza::print("Alto", this->h, W_DEBUG);

        //Si finalmente no hemos encontrado un alto y ancho adecuados, lo ponemos por defecto
        if (this->w == 0 || this->h == 0){
            this->w = 640;
            this->h = 480;
            bpp = 16;
        }
        
        SDL_putenv("SDL_VIDEO_CENTERED=center"); //Center the game Window
        //Finalmente establecemos el modo del video
        screen = SDL_SetVideoMode(this->w, this->h, bpp, this->fullsflags);
        if(!screen){
            cerr << "Error iniciando la pantalla width: " << this->w << " height: " << this->h << ". Saliendo de la aplicacion" << endl;
            exit(0);
        }
        //SDL_("SDL_VIDEO_CENTERED=0"); //Center the game Window
        Constant::setWINDOW_HEIGHT(this->h);
        Constant::setWINDOW_WIDTH(this->w);
        SDL_putenv( "SDL_VIDEO_CENTERED="); //Reset the centering when the screen resizes
    }

    Traza::print("Mostrando cursor", W_DEBUG);
	SDL_ShowCursor(CURSORVISIBLE);	// Disable mouse cursor on gp2x
	SDL_WM_SetCaption( getWindowTitle().c_str(), 0 );	// Sets the window title (not needed for gp2x)
	Traza::print("Iniciando TTF", W_DEBUG);
	TTF_Init();		// Initialize SDL_TTF
	Traza::print("Cargando fuente", W_DEBUG);
	loadFont(FONTSIZE);
    //Creamos un cursor vacio para poder ocultar el cursor y evitar el problema de
    //llamar a SDL_ShowCursor(SDL_DISABLE); para ocultarlo. Resultaba que se movia el cursor
    //a una posicion no deseada
    gestorCursor = new CursorGestor();
    setCursor(cursor_arrow);

    //Audio stuff
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        Traza::print("No se ha podido abrir el dispositivo de audio " +  string(Mix_GetError()), W_DEBUG);
        exit (1);
    }

    

	Traza::print("FIN initSDL", W_DEBUG);
}

/**
*
*/
void Ioutil::killSDL(){
    Traza::print("SDL Cerrando fuente", W_DEBUG);
	TTF_CloseFont(font);
	Traza::print("SDL Cerrando TTF", W_DEBUG);
	TTF_Quit();
	Traza::print("SDL Cerrando Joysticks", W_DEBUG);
    SDL_JoystickEventState(SDL_DISABLE);
    //Traza::print("Liberando cursores", W_DEBUG);
    //delete gestorCursor;
    Traza::print("SDL Cerrando subsistemas", W_DEBUG);
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Traza::print("SDL Cerrando SDL", W_DEBUG);
    SDL_Quit();
    font = NULL;
    Traza::print("clearJoyMapper", W_DEBUG);
    JoyMapper::clearJoyMapper();
    //Mix_CloseAudio();
    Traza::print("SDL CLOSED!!", W_DEBUG);
}

/**
*
*/
void Ioutil::drawObject(Object *obj, tEvento *evento){
    switch(obj->getObjectType()){
        case GUIPOPUPMENU:
            drawUIPopupMenu(obj);
            break;
        case GUILABEL:
            drawUILabel(obj);
            break;
        case GUIBUTTON:
            drawUIButton(obj);
            break;
        case GUILISTBOX:
            drawUIListBox(obj);
            break;
        case GUILISTGROUPBOX:
        case GUILISTIMG:
            drawUIListGroupBox(obj);
            break;
        case GUITREELISTBOX:
            drawUITreeListBox(obj);
            break;
        case GUICOMBOBOX:
            drawUIComboBox(obj);
            break;
        case GUIPICTURE:
            drawUIPicture(obj);
            break;
        case GUIARTSURFACE:
            drawUIArt(obj);
            break;
        case GUIINPUTWIDE:
            drawUIInputWide(obj);
            break;
        case GUITEXTELEMENTSAREA:
            drawUITextElementsArea(obj);
            break;
        case GUICHECK:
            showCheck(obj);
            break;
        case GUIPANELBORDER:
            drawUITitleBorder(obj->getLabel().c_str());
            break;
        case GUIPANEL:
            drawUIPanel(obj);
            break;
        case GUIPROGRESSBAR:
            //Traza::print("evento.mouse_x: ",evento->mouse_x, W_DEBUG);
            drawUIProgressBar(obj, evento);
            break;
        case GUISPECTRUM:
            if (((UISpectrum *)obj)->getSpectrumVisualizer() == WAVESPECTRUM){
                drawUISpectrum(obj);
            } else if (((UISpectrum *)obj)->getSpectrumVisualizer() == BARSPECTRUM){
                drawUISpectrumFft(obj);
            }

            break;
        case GUISLIDER:
            //Traza::print("evento.mouse_x: ",evento->mouse_x, W_DEBUG);
            drawUISlider(obj, evento);
            break;
        default:
            break;
    }
}


/**
*
*/
void Ioutil::toggleFullScreen(){
    //SDL_WM_ToggleFullScreen(screen);
    Uint32 flags = SCREEN_MODE; /* Start with whatever flags you prefer */

    /* -- Portable Fullscreen Toggling --
    As of SDL 1.2.10, if width and height are both 0, SDL_SetVideoMode will use the
    width and height of the current video mode (or the desktop mode, if no mode has been set).
    Use 0 for Height, Width, and Color Depth to keep the current values. */
    int tempw = 0, temph = 0, tempbpp = 0;

    flags = screen->flags; /* Save the current flags in case toggling fails */
    if (screen->flags & SDL_FULLSCREEN){
        tempw = Constant::getWINDOW_WIDTH();
        temph = Constant::getWINDOW_HEIGHT();
        tempbpp = SCREEN_BITS;
        screen->flags |= SDL_RESIZABLE;
    } else {
        tempw = WINDOW_WIDTH_FULLSCREEN;
        temph = WINDOW_HEIGHT_FULLSCREEN;
        tempbpp = SCREEN_BITS_FULLSCREEN;
    }

    /*Toggles FullScreen Mode */
    screen = SDL_SetVideoMode(tempw, temph, tempbpp, screen->flags ^ SDL_FULLSCREEN);
    this->fullsflags = screen->flags;
    this->w = tempw;
    this->h = temph;
    /* If toggle FullScreen failed, then switch back */
    if(screen == NULL) screen = SDL_SetVideoMode(Constant::getWINDOW_WIDTH(), Constant::getWINDOW_HEIGHT(), SCREEN_BITS, flags);
    /* If you can't switch back for some reason, then epic fail */
    if(screen == NULL) exit(1);
}

/**
 * Disables dpi scaling in windows 7 and later. It makes impossible to
 * set the correct fullscreen size
 */
void Ioutil::dpi(){
    void* userDLL;
    void* shcoreDLL;
    
    userDLL = SDL_LoadObject("USER32.DLL");
    if (userDLL) {
        SetProcessDPIAware = (BOOL(WINAPI *)(void)) SDL_LoadFunction(userDLL, "SetProcessDPIAware");
    }
    shcoreDLL = SDL_LoadObject("SHCORE.DLL");
    if (shcoreDLL) {
        SetProcessDpiAwareness = (HRESULT(WINAPI *)(PROCESS_DPI_AWARENESS)) SDL_LoadFunction(shcoreDLL, "SetProcessDpiAwareness");
    }

    if (SetProcessDpiAwareness) {
        /* Try Windows 8.1+ version */
        HRESULT result = SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
        Traza::print("called SetProcessDpiAwareness",(result == S_OK) ? 1 : 0, W_DEBUG);
    }
    else if (SetProcessDPIAware) {
        /* Try Vista - Windows 8 version.
        This has a constant scale factor for all monitors.
        */
        BOOL success = SetProcessDPIAware();
        Traza::print("called SetProcessDpiAwareness",(int)success, W_DEBUG);
    }
}

/**
 * 
 * @param mode
 */
int Ioutil::setFullscreenMode(int mode){
    
    SDL_Rect **modes;
    int i;
    int selMode = -1;
    
    /* Get available fullscreen/hardware modes */
    modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

    /* Check is there are any modes available */
    if(modes == (SDL_Rect **)0){
        printf("No modes available!\n");
        return 1;
    }

    /* Check if our resolution is restricted */
    if(modes == (SDL_Rect **)-1){
        printf("All resolutions available.\n");
    } else {
        /* Print valid modes */
        printf("Available Modes\n");
        for(i=0;modes[i];++i){
            printf("%d:  %d x %d\n",i, modes[i]->w, modes[i]->h);
            if (i == mode){
                printf("Setting mode to: %d x %d\n", modes[i]->w, modes[i]->h);
                /*Toggles FullScreen Mode */
                screen = SDL_SetVideoMode(modes[i]->w, modes[i]->h, SCREEN_BITS_FULLSCREEN, screen->flags | SDL_FULLSCREEN);
                this->fullsflags = screen->flags;
                this->w = modes[i]->w;
                this->h = modes[i]->h;
                selMode = i;
            }
        }
    }
    return selMode;
}

/**
* Mostramos un cursor vacio para poder ocultar el cursor y evitar el problema de
* llamar a SDL_ShowCursor(SDL_DISABLE); para ocultarlo. Resultaba que se movia el cursor
* a una posicion no deseada
*/
void Ioutil::setCursor(int cursor){
    SDL_SetCursor(gestorCursor->getCursor(cursor));
    actualCursor = cursor;
}

/**
*
*/
void Ioutil::loadFontFromFile(const char *filename){
    string dir = Constant::getAppDir() + Constant::getFileSep() + filename;
    try{
        //La llamada al loadFromFile ya libera recursos en caso de necesitarlos. No nos preocupamos por ello
        Traza::print("Abriendo fuente: " + dir, W_DEBUG);
        fontfile->loadFromFile(dir.c_str());
    } catch (Excepcion &e){
        Traza::print("loadFontFromFile: No se encuentra el fichero de la fuente" + string(dir), W_ERROR);
        //Lo cargamos del array que tenemos definido
        fontfile->loadFromMem(Arimo_Regular_ttf, Arimo_Regular_ttf_size);
    }
}


/**
*
*/
void Ioutil::loadFont(unsigned char size){
    if(font != NULL){
        Traza::print("Cerrando la fuente", W_PARANOIC);
        TTF_CloseFont(font);
        Traza::print("Asignando a null", W_PARANOIC);
        font = NULL;
    }
    Traza::print("Cargando de memoria", W_PARANOIC);
    SDL_RWops *RWOps = SDL_RWFromMem(fontfile->getFile(), fontfile->getFileSize());

    Traza::print("Abriendo la fuente", W_PARANOIC);
    if (RWOps != NULL){
        font = TTF_OpenFontRW(RWOps,1,size);
        fontAscent = TTF_FontAscent(font);
        fontDescent = TTF_FontDescent(font);
        fontHeight = TTF_FontHeight(font);
        //Establecemos el tam de los menus a un 50% mas que el tam de la fuente
        Constant::setMENUSPACE(ceil(fontHeight * 1.5));
        Constant::setINPUTH(ceil(fontHeight * 1.2));
    } else {
        Traza::print("No se puede cargar la fuente", W_ERROR);
    }
}

/**
*
*/
void Ioutil::clearScr(){
    //SDL_BlitSurface(empty, NULL, screen, NULL);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, cBgScreen.r,cBgScreen.g,cBgScreen.b));
}

/**
*/
void Ioutil::clearScr(t_color color){
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, color.r,color.g,color.b));
}

/**
*/
void Ioutil::clearScr(t_color color, t_region region){
    SDL_Rect location = {(short int)region.selX, (short int)region.selY, (short unsigned int)region.selW, (short unsigned int)region.selH};
    SDL_FillRect(screen, &location, SDL_MapRGB(screen->format, color.r,color.g,color.b));
}


/**
*/
void Ioutil::flipScr(){
    if (canFlip){
//        if ( fbdev >= 0 ) {
//            int arg = 0;
//            ioctl( fbdev, FBIO_WAITFORVSYNC, &arg );
//        }
        SDL_Flip(screen);
        //updateScr(NULL);
    }
}

/**
*/
void Ioutil::updateScr(t_region *region){
    //SDL_Flip(screen);
    if (region != NULL){
        SDL_UpdateRect(screen, region->selX, region->selY, region->selW, region->selH);
    } else {
        SDL_UpdateRect(screen, 0,0,0,0);
    }

}


/**
*
*/
void Ioutil::getMousePos(int *mouse_x, int *mouse_y){
    SDL_GetMouseState(mouse_x, mouse_y);
}

void Ioutil::clearEvento(tEvento *evento){
    evento->key = INT_MIN;
    evento->joy = INT_MIN;
    evento->isJoy = false;
    evento->isKey = false;
    evento->isMouse = false;
    evento->keyMod = INT_MIN;
    evento->unicode = INT_MIN;
    evento->resize = false;
    evento->quit = false;
    evento->isMousedblClick = false;
    evento->isMouseMove = false;
    evento->isRegionSelected = false;
    evento->mouse_state = INT_MIN;
    evento->mouse = INT_MIN;
    evento->mouse_x = 0;
    evento->mouse_y = 0;
    evento->keyjoydown = false;
    evento->width = 0;
    evento->height = 0;
    //t_region region;
}

void Ioutil::desactivarEventos(){
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
}

void Ioutil::activarEventos(){
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
    SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
    SDL_EventState(SDL_KEYUP, SDL_ENABLE);


}

/**
*/
tEvento Ioutil::WaitForKey(){

    static unsigned long lastClick = 0;
    static unsigned long lastKeyDown = 0;
    static unsigned long retrasoTecla = KEYRETRASO;
    static unsigned long lastMouseMove = 0;
    static t_region mouseRegion = {0,0,0,0};
    
    clearEvento(&evento);
    SDL_Event event;
#ifdef UNIX
    while( SDL_PollEvent( &event ) ){
#else
    if( SDL_PollEvent( &event ) ){
#endif
    //if (SDL_WaitEvent (&event)){
        switch( event.type ){
            case SDL_VIDEORESIZE:
                if (!(screen->flags & SDL_FULLSCREEN)){
                    screen = SDL_SetVideoMode (event.resize.w, event.resize.h, SCREEN_BITS, SCREEN_MODE);
                }
                this->w = event.resize.w;
                this->h = event.resize.h;
                evento.resize = true;
                evento.width = event.resize.w;
                evento.height = event.resize.h;
                break;
            case SDL_JOYBUTTONDOWN: // JOYSTICK/GP2X buttons
                if (event.jbutton.button >= 0 && event.jbutton.button < MAXJOYBUTTONS){
//                    evento.joy = Constant::getJoyMapper(event.jbutton.button);
                    evento.joy = event.jbutton.button;
                    evento.isJoy = true;
                    evento.keyjoydown = true;
                    lastEvento = evento;    //Guardamos el ultimo evento que hemos lanzado desde el teclado
                    lastKeyDown = SDL_GetTicks();  //reseteo del keydown
                }
                break;
            case SDL_JOYBUTTONUP:
                lastEvento = evento;
                evento.keyjoydown = false;
                break;
            case SDL_JOYHATMOTION:
                mPrevHatValues[event.jhat.which][event.jhat.hat] = event.jhat.value;
                evento.isJoy = true;


                if (event.jhat.value & SDL_HAT_UP){
                    evento.joy = JOYHATOFFSET + event.jhat.value;
                } else if (event.jhat.value & SDL_HAT_DOWN){
                    evento.joy = JOYHATOFFSET + event.jhat.value;
                } else if (event.jhat.value & SDL_HAT_LEFT){
                    evento.joy = JOYHATOFFSET + event.jhat.value;
                } else if (event.jhat.value & SDL_HAT_RIGHT){
                    evento.joy = JOYHATOFFSET + event.jhat.value;
                }

                if (event.jhat.value == 0) evento.keyjoydown = false;
                else {
                    evento.keyjoydown = true;
                    lastKeyDown = SDL_GetTicks();  //reseteo del keydown
                }
                lastEvento = evento;    //Guardamos el ultimo evento que hemos lanzado

                break;
            case SDL_JOYAXISMOTION:
                if((abs(event.jaxis.value) > DEADZONE) != (abs(mPrevAxisValues[event.jaxis.which][event.jaxis.axis]) > DEADZONE))
                {
                    int normValue;
                    evento.isJoy = true;

                    if(abs(event.jaxis.value) <= DEADZONE){
                        normValue = 0;
                        evento.keyjoydown = false;
                    } else {
                        if(event.jaxis.value > 0)
                            normValue = 1;
                        else
                            normValue = -1;

                        evento.keyjoydown = true;
                        lastKeyDown = SDL_GetTicks();  //reseteo del keydown
                    }

                    int valor = (abs(normValue) << 4 | event.jaxis.axis) * normValue;
                    evento.joy = JOYAXISOFFSET + valor;
                    lastEvento = evento;    //Guardamos el ultimo evento que hemos lanzado desde el teclado

                }

                mPrevAxisValues[event.jaxis.which][event.jaxis.axis] = event.jaxis.value;

                break;
            case SDL_KEYDOWN: // PC buttons
                evento.key = event.key.keysym.sym;
                evento.keyMod = event.key.keysym.mod;
                evento.unicode = event.key.keysym.unicode;
                evento.isKey = true;
                evento.keyjoydown = true;
//                if (evento.keyMod & KMOD_LCTRL && evento.key == SDLK_c) evento.quit = true;
                lastEvento = evento;    //Guardamos el ultimo evento que hemos lanzado desde el teclado
                lastKeyDown = SDL_GetTicks();  //reseteo del keydown
                break;
            case SDL_KEYUP: // PC button keyup
                lastEvento = evento;
                break;
            case SDL_MOUSEBUTTONDOWN: // Mouse buttons SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
                evento.mouse = event.button.button;
                evento.mouse_x = event.button.x;
                evento.mouse_y = event.button.y;
                evento.mouse_state = event.button.state;
                evento.isMouse = true;
                if (SDL_GetTicks() - lastClick < DBLCLICKSPEED){
                   evento.isMousedblClick = true;
                   lastClick = SDL_GetTicks() - DBLCLICKSPEED;  //reseteo del dobleclick
                } else {
                    lastClick = SDL_GetTicks();
                }
                mouseRegion.selX = event.button.x;
                mouseRegion.selY = event.button.y;
                evento.region = mouseRegion;
                break;
            case SDL_MOUSEBUTTONUP:
                evento.mouse = event.button.button;
                evento.mouse_x = event.button.x;
                evento.mouse_y = event.button.y;
                evento.mouse_state = event.button.state;
                evento.isMouse = true;
                evento.isRegionSelected = false;
                break;
            case SDL_MOUSEMOTION:
                evento.mouse = event.button.button;
                evento.mouse_x = event.button.x;
                evento.mouse_y = event.button.y;
                evento.mouse_state = event.button.state;
                evento.isMouseMove = true;
                lastMouseMove = SDL_GetTicks();
                if (actualCursor == cursor_hidden && evento.mouse_state != SDL_PRESSED
                    && evento.mouse != MOUSE_BUTTON_LEFT){
                     setCursor(cursor_arrow);
                }
                
                if (evento.mouse == MOUSE_BUTTON_LEFT){
                    mouseRegion.selW = event.button.x - mouseRegion.selX;
                    mouseRegion.selH = event.button.y - mouseRegion.selY;
                    evento.isRegionSelected = true;
                } else {
                    mouseRegion.selW = 0;
                    mouseRegion.selH = 0;
                    evento.isRegionSelected = false;
                }
                evento.region = mouseRegion;
//                Traza::print("mouse motion. selected: " + string(evento.isRegionSelected ? "S" : "N")
//                + " mouse state: " + Constant::TipoToStr(evento.mouse_state)
//                + " evt mouse: " + Constant::TipoToStr(evento.mouse) 
//                + " selW: " + Constant::TipoToStr(mouseRegion.selW) 
//                + " selH: " + Constant::TipoToStr(mouseRegion.selH), 
//                        W_DEBUG);
                break;
            case SDL_QUIT:
                evento.quit = true;
                Traza::print("SDL_QUIT Detected", W_DEBUG);
                break;
            default :
                break;
        }
    } else {
        clearEvento(&lastEvento);
    }

    unsigned long now = SDL_GetTicks();

    //En algunas ocasiones se repiten eventos. Con este flag los controlamos
    if (ignoreButtonRepeats){
        lastEvento.keyjoydown = false;
        ignoreButtonRepeats = false;
        Traza::print("Ignorando retraso de teclas", W_DEBUG);
    }

    if (lastEvento.keyjoydown == true){
        if (now > lastKeyDown + KEYDOWNSPEED + retrasoTecla){
            Traza::print("Repitiendo tecla", W_DEBUG);
            lastKeyDown = SDL_GetTicks();
            evento = lastEvento;
            retrasoTecla = 0;
        }
    } else {
        retrasoTecla = KEYRETRASO;
    }

    if (now > lastMouseMove + MOUSEVISIBLE && actualCursor != cursor_hidden){
        setCursor(cursor_hidden);
    }

    return evento;
}

/**
*
*/
void Ioutil::printGrafica(double fase, int altura){
    int funcion = 0;
    int periodo = 80;
    int amplitud = 15;

    for (int i=0;i < this->getWidth(); i++){
        funcion = (int)(amplitud*sin(2*PI/periodo*i + fase) + altura);
        if (funcion > altura){
            funcion = altura - (funcion - altura);
        }

        for (int j=0; j < amplitud; j++){
           if (funcion + j >= 0 && funcion + j < this->getHeight())
            putpixelSafe(screen,i, funcion + j,SDL_MapRGB(screen->format, 255, 255, 255));
        }
    }
    drawRect(0, altura, this->getWidth(), this->getHeight()-altura, cRojo);
}


/**
*
*/
void Ioutil::fps(){
    drawTextInt(mediaFps,this->getWidth() - 60,0,cRojo);
    if (SDL_GetTicks() - lastTime > FRAMEPERIOD){
        mediaFps = frames;
        frames = 0;
        lastTime = SDL_GetTicks();
    } else{
        frames++;
    }
}

/**
*
*/
void Ioutil::printContImg(const char *dato){
    int pixelDato = 0;
    TTF_SizeText(this->font,dato,&pixelDato,NULL );
    SDL_Rect textLocation = {(short int)(this->getWidth() - pixelDato), (short int)(this->getHeight() - Constant::getMENUSPACE()), (short unsigned int)pixelDato, (short unsigned int)Constant::getMENUSPACE()};
    SDL_FillRect(screen, &textLocation, SDL_MapRGB(screen->format, 0,0,0));
    drawText(dato,this->getWidth() - pixelDato,this->getHeight() - Constant::getMENUSPACE(),cBlanco);
}

/**
*
*/
void Ioutil::drawTextCent( const char* dato, int x, int y, bool centx, bool centy, t_color color){
    if (font != NULL){
        int pixelDato = 0;
        TTF_SizeText(this->font,dato,&pixelDato,NULL );
        int posDatox = x;
        int posDatoy = y;

        if (centx){
            posDatox = (screen->w - pixelDato)/2;
            posDatox += x;
        }
        if (centy){
            posDatoy = (screen->h)/2;
            posDatoy += y;
        }
        drawText(dato,posDatox,posDatoy,color);
    } else {
        Traza::print("Fallo en drawTextCent: La fuente es NULL", W_ERROR);
    }
}

/**
*
*/
void Ioutil::drawText( const char* dato, int x, int y, t_color color){
        drawTextInArea(dato, x, y, color, NULL);
}

/**
*
*/
void Ioutil::drawTextInArea( const char* dato, int x, int y, t_color color, SDL_Rect *textLocation){
    drawTextInAreaSrf(dato, x, y, color, textLocation, screen);
}

/**
*
*/
void Ioutil::drawTextInAreaSrf( const char* dato, int x, int y, t_color color, SDL_Rect *textLocation , SDL_Surface* dstSrf, bool isAlpha){
    if (font != NULL){
        SDL_Color foregroundColor = { (unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b };
        SDL_Surface* textSurface =  TTF_RenderText_Blended(font,dato, foregroundColor);
        if (isAlpha){
            SDL_SetColorKey(textSurface, textSurface->format->colorkey, SDL_ALPHA_OPAQUE);
            SDL_SetAlpha( textSurface, 0, SDL_ALPHA_OPAQUE );
        }
        SDL_Rect screenLocation = { (short int)x, (short int)y, 0, 0 };
        SDL_BlitSurface(textSurface, textLocation, dstSrf, &screenLocation);
        SDL_FreeSurface(textSurface);
    } else {
        Traza::print("Fallo en drawTextInAreaSrf: La fuente es NULL", W_ERROR);
    }
}


/**
*/
void Ioutil::drawTextInt(int number, int x, int y, t_color color){
    if (font != NULL){
        char tmp_num[16];
        sprintf((char*)tmp_num, "%d", number);
        SDL_Color foregroundColor = { (unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b };
        SDL_Surface* textSurface =  TTF_RenderText_Blended(font,tmp_num, foregroundColor);
        SDL_Rect screenLocation = { (short int)x, (short int)y, 0, 0 };
        SDL_BlitSurface(textSurface, NULL, screen, &screenLocation);
        SDL_FreeSurface(textSurface);
    } else {
        Traza::print("Fallo en drawTextInt: La fuente es NULL", W_ERROR);
    }
}

bool Ioutil::drawRectAlpha(int x, int y, int w, int h, t_color color, int colorAlpha){
    return drawRectAlpha(x, y, w, h, color, colorAlpha, screen);
}
/**
*
*/
bool Ioutil::drawRectAlpha(int x, int y, int w, int h, t_color color, int colorAlpha, SDL_Surface *destSurf){
    bool ret = true;
    SDL_Surface *mySurface = NULL;
    SDL_Rect dstrect = { (short int)x, (short int)y, (short unsigned int)w, (short unsigned int)h };

    if (w == 0)
        dstrect.w = destSurf->w;

    if (h == 0)
        dstrect.h = destSurf->h;

    if (w > 0 && h > 0){
        //Si tenemos que generar una surface con alpha, va a ralentizar a la aplicacion dependiendo mucho del tamanyo de la pantalla
        if (colorAlpha > 0 && colorAlpha <= 255){
            mySurface = SDL_CreateRGBSurface(SURFACE_MODE, dstrect.w, dstrect.h, destSurf->format->BitsPerPixel,0, 0, 0, 0);
            SDL_FillRect( mySurface, NULL, SDL_MapRGB(destSurf->format, color.r, color.g, color.b) );
            SDL_SetColorKey(mySurface, SDL_SRCCOLORKEY, 128);
            SDL_SetAlpha(mySurface, SDL_SRCALPHA, colorAlpha);
            SDL_BlitSurface(mySurface, NULL, destSurf, &dstrect);
            SDL_FreeSurface( mySurface );
        } else {
             SDL_FillRect( destSurf, &dstrect, SDL_MapRGB(destSurf->format, color.r, color.g, color.b) );
        }

    } else {
        Traza::print("Error de ancho y alto en drawRectAlpha" + Constant::TipoToStr(x) + ";"+ Constant::TipoToStr(y) + ";"
                     + Constant::TipoToStr(w) + ";"+ Constant::TipoToStr(h) + ";", W_ERROR);
        ret = false;
        //throw(Excepcion(ERANGO));
    }
    
    return ret;
}

/**
*
*/
bool Ioutil::drawRect(int x, int y, int w, int h, t_color color){
        return drawRectAlpha(x,y,w,h,color,-1);
}
/**
*
*/
bool Ioutil::drawRect(int x, int y, int w, int h, t_color color, SDL_Surface *destSurf){
        return drawRectAlpha(x,y,w,h,color,-1, destSurf);
}



/**
* Necesito pasar un puntero al puntero porque sino el valor del puntero se parasaria por valor. De esta forma el puntero
* queda con el valor almacenado.
*/
void Ioutil::takeScreenShot(SDL_Surface **pointerSurface){
    if (*pointerSurface != NULL) SDL_FreeSurface( *pointerSurface );
        *pointerSurface = SDL_CreateRGBSurface(SURFACE_MODE, screen->w, screen->h, screen->format->BitsPerPixel,0, 0, 0, 0);
    SDL_BlitSurface(screen, NULL, *pointerSurface, NULL);
}

/**
*
*/
void Ioutil::takeScreenShot(SDL_Surface **pointerSurface, SDL_Rect dstrect){
    if (*pointerSurface != NULL) SDL_FreeSurface( *pointerSurface );
        *pointerSurface = SDL_CreateRGBSurface(SURFACE_MODE, dstrect.w, dstrect.h, screen->format->BitsPerPixel,0, 0, 0, 0);
    SDL_BlitSurface(screen, &dstrect, *pointerSurface, NULL);
}

/**
*
*/
void Ioutil::printScreenShot(SDL_Surface **pointerSurface){
    SDL_BlitSurface(*pointerSurface, NULL,screen , NULL);
}

/**
*
*/
void Ioutil::printScreenShot(SDL_Surface **pointerSurface, SDL_Rect dstrect){
    SDL_BlitSurface(*pointerSurface, NULL,screen , &dstrect);
}

/**
*
*/
//Los campos x0, x1, y0, y1 representan las esquinas
void Ioutil::drawRectLine(int x0, int y0, int w0, int h0, int lineWidth, t_color color){

    int x1 = (x0 + w0) < this->w ? (x0 + w0) : this->w-1 ;
    int y1 = (y0 + h0) < this->h ? (y0 + h0) : this->h-1 ;

    if (y0+lineWidth < this->w)
        for (int i=0;i<lineWidth;i++){
            pintarLinea(x0,y0+i, x1, y0+i, color); //Superior
            pintarLinea(x0,y1-i, x1, y1-i, color); //Inferior
            pintarLinea(x0+i,y0, x0+i, y1, color); //Izquierda
            pintarLinea(x1-i,y0, x1-i, y1, color);//Derecha
        }
}

/**
*
*/
void Ioutil::pintarContenedor(int x1, int y1, int w1, int h1, bool selected, Object *obj, t_color color){
    if (w1 >= 0 && h1 >= 0 && w1 >= INPUTBORDER && h1 >= INPUTBORDER){
        
        int border = INPUTBORDER;
        if (obj->getImgGestor()->isFillBackgroundColour() && obj->isVerContenedor()  && obj->getObjectType() == GUIPICTURE){
            border = 0;
        }
        
        if (obj->isVerContenedor() || obj->getImgGestor()->isFillBackgroundColour()){
            if ( (selected && obj == NULL) || (selected && obj != NULL && obj->showShadow()) ){
                drawRectLine(x1+border, y1 + border, w1, h1, BORDERSELECTION, cSelection);//Dibujo la sombra del elemento seleccionado
            }
            if (obj->getAlpha() >= 0){
                drawRectAlpha(x1+border,y1+border,w1-border,h1-border, color, obj->getAlpha()); // Dibujo el contenedor
            } else {
                drawRect(x1+border,y1+border,w1-border,h1-border, color); // Dibujo el contenedor
            }
            
            if (border > 0){
                drawRectLine(x1,y1,w1,h1,border,cInputBorder);//Dibujo el borde
            }
        }
    } else {
        Traza::print("Error del objeto " + obj->getLabel() + " con ancho y alto en pintarContenedor: " + Constant::TipoToStr(x1) + ";"+ Constant::TipoToStr(y1) + ";"
                     + Constant::TipoToStr(w1) + ";"+ Constant::TipoToStr(h1) + ";", W_ERROR);
    }
}

/**
*
*/
void Ioutil::pintarHint(int x1, int y1, int w1, int h1, string text, t_color color){
    w1 += 4;
    drawRect(x1+INPUTBORDER,y1+INPUTBORDER,w1-INPUTBORDER,h1-INPUTBORDER, color); // Dibujo el contenedor
    drawRectLine(x1,y1,w1,h1,INPUTBORDER,cInputBorder);//Dibujo el borde
    drawText(text.c_str(), x1 + 2, y1 + 2, cNegro);
}

/**
*
*/
void Ioutil::pintarLineaV (int x1, int y1, int h, t_color color){
    int dif = abs(h);
    for (int i=0; i < dif; i++){
        putpixelSafe(screen, x1, h > 0 ? y1 + i : y1 - i, SDL_MapRGB(screen->format, color.r, color.g, color.b));
    }
}

void Ioutil::pintarLineaSpectrum (int x1, int y1, int h, int maxh, t_color color){
    int dif = abs(h);
    int corte = ceil(maxh/(float)3);

    for (int i=0; i < dif; i++){
        if (dif < corte){
//            color1Spectrum.getDegradedColor(dif, &degColorSpectrum);
            degColorSpectrum = cVerde;
        } else if (dif < corte*2){
//            color2Spectrum.getDegradedColor(dif, &degColorSpectrum);
            degColorSpectrum = cAmarillo;
        } else {
//            color3Spectrum.getDegradedColor(dif, &degColorSpectrum);
            degColorSpectrum = cRojo;
        }

        putpixelSafe(screen, x1, h > 0 ? y1 + i : y1 - i,
            SDL_MapRGB(screen->format, degColorSpectrum.r, degColorSpectrum.g, degColorSpectrum.b));
    }
}


/**
*
*/
void Ioutil::pintarLinea (int x1, int y1, int x2, int y2 , t_color color)
{
    if (x1 >= 0 && y1 < this->getWidth()-1 && x2 >= 0 && y2 < this->getHeight()-1){
        int tempInt = 0;
        int i = x1;
        int j = y1;
        int tempj = 0;
        int numerador = (y1-y2);
        int denominador = (x1-x2);
        float pendiente = 0;
        int dif = 0;
        int contj = 0;
        int conti = 0;
        int difx = (x2 > x1) ? x2-x1+1 : x1-x2+1;
        int dify = (y2 > y1) ? y2-y1+1 : y1-y2+1;

        if (denominador != 0){
            pendiente = numerador/(float)denominador;
            while (conti < difx && conti < screen->w){
                //Funcion de la recta -> j = (y1-y2)/(x1-x2) * (i-x1) + y1
                j = (int)(pendiente * (i-x1)) + y1;
                tempj = (int)(pendiente * (i+1-x1)) + y1;
                dif = (j > tempj) ? j-tempj : tempj-j;

                if (dif > 1){
                    contj = 0;
                    while (contj < dif && (conti+1)*dif < dify){
                        putpixelSafe(screen, i, (j > tempj) ? j-contj : j + contj, SDL_MapRGB(screen->format, color.r, color.g, color.b));
                        contj++;
                    }
                } else {
                    putpixelSafe(screen,i,j,SDL_MapRGB(screen->format, color.r, color.g, color.b));
                }

                if ((pendiente > 0. && denominador < 0.) ||
                    (pendiente < 0. && denominador < 0.) ||
                    (pendiente == 0 && denominador < 0.)){
                        i++;
                } else if((pendiente < 0. && denominador > 0.) ||
                        (pendiente > 0. && denominador > 0.) ||
                        (pendiente == 0 && denominador > 0.)) {
                        i--;
                }

                conti++;
            }
        } else {
            //    //Comprobamos que el x1 sea siempre el menor punto
            if (x1 > x2) {
                tempInt = x1;
                x1 = x2;
                x2 = tempInt;
            }

            if (y1 > y2) {
                tempInt = y1;
                y1 = y2;
                y2 = tempInt;
            }
            i = x1;
            j = y1;

            while (j <= y2 && j < screen->h){
                putpixelSafe(screen,i,j,SDL_MapRGB(screen->format, color.r, color.g, color.b));
                j++;
            }
        }
    }
}

/**
*
*/
void Ioutil::drawUIArt(Object *obj){

    SDL_Rect imgLocation = { (short int)obj->getX() , (short int)obj->getY(), (short unsigned int)obj->getW(), (short unsigned int)obj->getH() };

    if (!obj->getImgDrawed()){
        int fontStyle = TTF_GetFontStyle(font);
        if (!(fontStyle & TTF_STYLE_BOLD)) {
            TTF_SetFontStyle(font, fontStyle | TTF_STYLE_BOLD);
        }

        int tamLabel = fontStrLen(obj->getLabel().c_str());

        drawRect(obj->getX(), obj->getY(), obj->getW(), obj->getH(), cTitleScreen);
        drawRectLine(obj->getX(), obj->getY(), obj->getX() + obj->getW()-1, obj->getY() + obj->getH()-1,1, cTitleScreen);
        int posx = obj->getW() - tamLabel;
        int posy = (obj->getH() - FONTSIZE)/2;

        //Si el tamanyo del texto a pintar es menor que el disponible, centro el texto
        if (tamLabel < obj->getW()){
            posx = posx / 2;
        }

        drawText(obj->getLabel().c_str(),posx, posy, cBlanco);
        TTF_SetFontStyle(font, fontStyle);

        //Reseteamos el surface
        obj->getImgGestor()->setSurface(NULL);
        //Guardamos la imagen en el objeto
        takeScreenShot(&obj->getImgGestor()->surface, imgLocation);
        //Indicamos que ya hemos pintado la imagen
        obj->setImgDrawed(true);
    } else {
        //SDL_Surface *pointerSurface = obj->getImgGestor()->getSurface();
        printScreenShot(&obj->getImgGestor()->surface, imgLocation);
    }
}

/**
*
*/
bool Ioutil::drawImgObj(Object *obj){
    bool salida = false;
    int bordeAdd = 0;
    
    if (!obj->isVisible()){
        return false;
    }
    
    if (obj->getImgGestor()->isFillBackgroundColour() && obj->isVerContenedor() && obj->getObjectType() == GUIPICTURE){
        bordeAdd = 0;
    } else if (obj->isVerContenedor()){
        bordeAdd = INPUTBORDER;
    }
    SDL_Rect imgLocation = { (Sint16)obj->getX() , (Sint16)obj->getY(), (Uint16) (obj->getW() - bordeAdd), (Uint16)(obj->getH() - bordeAdd)};

    if (!obj->getImgDrawed()){
        Traza::print("Drawing image from object", W_PARANOIC);
        t_region regionPantalla = {obj->getX() + bordeAdd, obj->getY() + bordeAdd, obj->getW(), obj->getH()};
        Traza::print("Region generated", W_PARANOIC);
        salida = obj->getImgGestor()->drawImgMem(-1, obj->getW(), obj->getH(), regionPantalla, screen);
        Traza::print(string("Image Drawed with result") + string(salida ? "Ok" : "Error"), W_PARANOIC);
    } else {
        SDL_Rect imgRect = { (short int)obj->getImgGestor()->getLeftDif() ,
                             (short int)obj->getImgGestor()->getTopDif(),
                             (short unsigned int)(obj->getImgGestor()->getLeftDif()  + obj->getW()),
                             (short unsigned int)(obj->getImgGestor()->getTopDif() + obj->getH()) };
        Traza::print("getLeftDif",obj->getImgGestor()->getLeftDif(),W_PARANOIC);
        SDL_BlitSurface(obj->getImgGestor()->getSurface(), &imgRect, screen, &imgLocation);
        salida = true;
    }
    return salida;
}

/**
*
*/
void Ioutil::drawIco(int numIco, int x, int y){
    gestorIconos->drawIcono(numIco, screen, x, y);
}

/**
*
*/
void Ioutil::drawIco(int numIco, int x, int y, int w, int h){
    gestorIconos->drawIcono(numIco, screen, 0, x, y, w, h);
}

/**
*
*/
void Ioutil::drawIco(int numIco, int angle, int x, int y, int w, int h){
    gestorIconos->drawIcono(numIco, screen, angle, x, y, w, h);
}

/**
* En base al surface temporal creado en la llamada a drawZoomImgMem mueve el punto de
* vista para situarnos donde sea necesario
*/
bool Ioutil::drawImgMoved(ImagenGestor *imgGestor){
    SDL_Rect dstRect = { 0,0,0,0 };
    bool salida = false;
    if (moveSurface != NULL){
        clearScr(cBgImageBin);
        imgGestor->calcRectCent(&dstRect, moveSurface->w, moveSurface->h, screen->w, screen->h);
        salida = imgGestor->blitImage(moveSurface, screen,&dstRect, false);
        flipScr();
    }
    //objTraza->print("Con resize",(int)(SDL_GetTicks()-now));
    return salida;
}

/**
* Redimensionamos una superficie al tamanyo que queramos en base a la dimension
* relacion y la almacena en destino
*/
void Ioutil::ResizeSurface(float relacion, SDL_Surface *varBitmap, SDL_Surface **destino)
{
    if (varBitmap != NULL && relacion != 0.){
        const int newWidth =  (int)(varBitmap->w / relacion);
        const int newHeight = (int)(varBitmap->h / relacion);
        const int maxi = newHeight-1;
        const int maxj = newWidth-1;
        *destino = SDL_CreateRGBSurface( SURFACE_MODE, newWidth, newHeight, varBitmap->format->BitsPerPixel,
                                        varBitmap->format->Rmask,varBitmap->format->Gmask,varBitmap->format->Bmask,
                                        varBitmap->format->Amask);

        if (*destino != NULL){
            //Calculamos el array de los pixeles
            for (int ti = 0; ti <= maxi; ti++)
                for (int tj = 0; tj <= maxj; tj++)
                    putpixel(*destino, tj, ti,  getpixel(varBitmap,(int)(tj * relacion),(int)(ti * relacion)));
        } else {
            Traza::print("Ioutil::ResizeSurface:" + string(IMG_GetError()), W_ERROR);
        }
    }
}

/**
*
*/
void Ioutil::drawUITitleBorder(const char *title, int sizeBorder, t_color colorBorder){
    //Dibujando el titulo
    loadFont(TITLEFONTSIZE);
    drawText(title,sizeBorder + TITLESPACE, sizeBorder - TITLEFONTSIZE/2, cBlanco);

    int pixelTitle = 0;
    TTF_SizeText(this->font,title,&pixelTitle,NULL );
    const int tamTitle = sizeBorder + 10 + pixelTitle;

    pintarLinea(sizeBorder,sizeBorder,sizeBorder + TITLESPACE/2,sizeBorder,colorBorder);
    pintarLinea(sizeBorder+1,sizeBorder+1,sizeBorder + TITLESPACE/2,sizeBorder+1,cBlanco);

    pintarLinea(sizeBorder,sizeBorder,sizeBorder,screen->h -sizeBorder,colorBorder);
    pintarLinea(sizeBorder+1,sizeBorder+1,sizeBorder+1,screen->h -sizeBorder -1,cBlanco);

    pintarLinea(sizeBorder,screen->h -sizeBorder,screen->w -sizeBorder,screen->h -sizeBorder,colorBorder);
    pintarLinea(sizeBorder,screen->h -sizeBorder+1,screen->w -sizeBorder,screen->h -sizeBorder+1,cBlanco);

    pintarLinea(screen->w -sizeBorder,sizeBorder,screen->w -sizeBorder,screen->h -sizeBorder,colorBorder);
    pintarLinea(screen->w -sizeBorder+1,sizeBorder,screen->w -sizeBorder+1,screen->h -sizeBorder+1,cBlanco);

    pintarLinea(tamTitle + sizeBorder,sizeBorder,screen->w -sizeBorder,sizeBorder,colorBorder);
    pintarLinea(tamTitle + sizeBorder,sizeBorder+1,screen->w -sizeBorder-1,sizeBorder+1,cBlanco);

    loadFont(FONTSIZE);
}

/**
*
*/
void Ioutil::drawUITitleBorder(const char *title){
    t_color color = {128,128,128};
    drawUITitleBorder(title,TITLEBORDER, color);
}

/**
*
*/
void Ioutil::drawUIPanel(Object *obj){
    if (obj->isVisible() && obj->getObjectType() == GUIPANEL){
        drawRectAlpha(obj->getX(), obj->getY(), obj->getW(), obj->getH() , ((UIPanel *)obj)->getColor(), obj->getAlpha());
    }
}

/**
*
*/
void Ioutil::drawUIPicture(Object *obj){
    if (obj->isVisible()){
        int x_ = obj->getX();
        int y_ = obj->getY();
        int w_ = obj->getW();
        int h_ = obj->getH();
        boolean imgFound = false;
        
        if (!obj->getImgDrawed()){
            t_color fondo = obj->getImgGestor()->isFillBackgroundColour() ?
                                obj->getImgGestor()->getColorBackground() : cInputContent;

            pintarContenedor(x_,y_,w_,h_,obj->isFocus() && obj->isEnabled(), obj, fondo);
            if (obj->getImgGestor() != NULL){
                imgFound = drawImgObj(obj);
                if (imgFound && obj->getAlpha() >= 0){
                    drawRectAlpha(x_+INPUTBORDER,y_+INPUTBORDER,w_-INPUTBORDER,h_-INPUTBORDER,cInputContent, obj->getAlpha()); // Difumino la imagen
                }
            }
            if (imgFound) 
                cachearObjeto(obj);
        } else {
            cachearObjeto(obj);
        }
    }
}

/**
*
*/
void Ioutil::drawUISpectrum(Object *obj){
    int x;
    if (obj->isVisible()){
        if (!obj->getImgDrawed()){
            int x_ = obj->getX();
            int y_ = obj->getY();
            int w_ = obj->getW();
            int h_ = obj->getH();
            int W=w_-2*INPUTBORDER;
            int H=h_-2*INPUTBORDER;
            int H2=(H/2);
            int H4=(H/4);

            pintarContenedor(x_,y_,w_,h_,obj->isFocus() && obj->isEnabled(), obj, obj->getColor());
            UISpectrum *objspectrum = (UISpectrum *)obj;

            if (objspectrum->isEnabled()){
                SDL_LockSurface(screen);
                Uint32 colorFondo = SDL_MapRGB(screen->format, obj->getColor().r, obj->getColor().g, obj->getColor().b);
                Uint32 colorSpectrum = SDL_MapRGB(screen->format, cBlanco.r, cBlanco.g, cBlanco.b);
                /* clear the screen */
                /* SDL_FillRect(s,NULL,black); */
                /* draw the wav from the saved stream buffer */
                if (objspectrum->buf != NULL){
                    x_+=INPUTBORDER;
                    y_+=INPUTBORDER;
                    int lenX = W*2 > objspectrum->getBuffSize() ? objspectrum->getBuffSize() : W*2;
//
                    if (lenX > 0){
//                        int inc = floor(objspectrum->getBuffSize() / (float)lenX);
//                        if (inc < 1) inc = 1;
                        //inc = 1;
                        int ejex = 0;

                        for(x=0;x<lenX;x++){
                            const int X=x>>1, b=x&1 ,t=H4+H2*b;
                            int y1,h1;
                            if(objspectrum->buf[ejex]<0){
                                h1=-Y(objspectrum->buf[ejex]);
                                y1=t-h1;
                            } else {
                                y1=t;
                                h1=Y(objspectrum->buf[ejex]);
                            }
                            pintarLineaV(X + x_, y1 + y_, h1, cBlanco);
                            //pintarLineaSpectrum(X + x_, y1 + y_, h1, ALBUMWIDTH/4, cBlanco);
                            ejex+=objspectrum->getZoom();
                        }
                    }

                }
            }
            SDL_UnlockSurface(screen);
            cachearObjeto(obj);
        } else {
            cachearObjeto(obj);
        }
    }
}

t_color Ioutil::MapColor (int s){

    int totalColores = sizeof (ColorScaleFFT) / sizeof(t_color);

    s = (s / (double)MAXVALUEFFT) * totalColores;

	if (s > totalColores - 1)
		s = totalColores - 1;
	return ColorScaleFFT [s];
}

/**
*
*/
void Ioutil::drawUISpectrumFft(Object *obj){
    const int margenBarras = 20;
    const int sepBarras = 3;
    const int numCeils = 10;
    const int x_ = obj->getX();
    const int y_ = obj->getY();
    const int w_ = obj->getW();
    const int h_ = obj->getH();
    const int W=w_-2*INPUTBORDER;
    const int H=h_-2*INPUTBORDER;
    const int hBarCeil = floor( (H - margenBarras) / (double)numCeils);
    const int barWidth = W /(double)NBIQUADFILTERS;
    int valueFreq = 0;
    double percent = 0;
    int barHeight = 0;
    t_color barColor;
    SDL_Rect labelLocation;
    labelLocation.x = x_;
    labelLocation.y = y_ + H - margenBarras;
    labelLocation.w = x_ + W;
    labelLocation.h = fontHeight;
    int tmpFontSize = FONTSIZE;

    if (obj->isVisible()){
        if (!obj->getImgDrawed()){
            pintarContenedor(x_,y_,w_,h_,obj->isFocus() && obj->isEnabled(), obj, obj->getColor());
            UISpectrum *objspectrum = (UISpectrum *)obj;

            if (objspectrum->isEnabled() && objspectrum->arrFreqVis != NULL){
                SDL_LockSurface(screen);
                for (int i=0; i < NBIQUADFILTERS; i++){
                    //Obtenemos el valor de la frecuencia
                    valueFreq = objspectrum->arrFreqVis[i] > MAXVALUEFFT ? MAXVALUEFFT : objspectrum->arrFreqVis[i];
                    //Obtenemos el color y el tamanyo de la barra a pintar
                    barColor = MapColor(valueFreq);
                    percent = valueFreq / (double) MAXVALUEFFT;
                    barHeight = (H - margenBarras) * percent;

                    if (barHeight > hBarCeil){
                        int ceils = barHeight / hBarCeil;
                        barHeight = hBarCeil * ceils - sepBarras;
                    }
                    //Pintamos las barras que indican las componentes en frecuencia
                    SDL_Rect r={x_ + i * barWidth, y_ + H - barHeight - margenBarras, x_ + barWidth, barHeight};
                    SDL_FillRect(screen, &r, SDL_MapRGB(screen->format,barColor.r,barColor.g,barColor.b));
                    //Pintamos las barras verticales de separacion entre bandas
                    SDL_Rect sepRect={x_ + i * barWidth, y_ + INPUTBORDER, sepBarras, H};
                    SDL_FillRect(screen, &sepRect, SDL_MapRGB(screen->format, obj->getColor().r,obj->getColor().g,obj->getColor().b));
                }

                SDL_Rect sepRect={x_ + INPUTBORDER,
                                  y_ + H - margenBarras,
                                  x_ + W,
                                  sepBarras};

                for (int i=0; i < numCeils; i++){
                    sepRect.y -= hBarCeil;
                    SDL_FillRect(screen, &sepRect, SDL_MapRGB(screen->format, obj->getColor().r,obj->getColor().g,obj->getColor().b));
                }

                SDL_UnlockSurface(screen);

                if (!obj->isOtherDrawed()){
                    loadFont(9);
                    for (int i=0; i < NBIQUADFILTERS; i++){
                        drawText(frecsEQStr[i], x_ + i * barWidth + barWidth / 4, labelLocation.y + 5, cBlanco);
                    }
                    cachearPosicion(obj,&labelLocation);
                    loadFont(tmpFontSize);
                } else {
                    cachearPosicion(obj,&labelLocation);
                }
                cachearObjeto(obj);
            }
        } else {
            cachearObjeto(obj);
        }
    }
}


/**
*
*/
void Ioutil::cachearPosicion(Object *obj, SDL_Rect *imgLocation){
    if (obj->isVisible()){
        if (!obj->isOtherDrawed()){
            //Reseteamos el surface
            obj->getOtherCache()->setSurface(NULL);
            //Guardamos la imagen en el objeto
            takeScreenShot(&obj->getOtherCache()->surface, *imgLocation);
            //Indicamos que ya hemos pintado la imagen
            obj->setOtherDrawed(true);
        } else {
            printScreenShot(&obj->getOtherCache()->surface, *imgLocation);
        }
    }
}

/**
*
*/
void Ioutil::cachearObjeto(Object *obj){
    if (obj->isVisible()){
        int borde = 0;

        if (obj->showShadow() && obj->isVerContenedor()){
            borde += BORDERSELECTION;
        } else if (obj->isVerContenedor()){
            borde += INPUTBORDER;
        }

        SDL_Rect imgLocation = { (short int)obj->getX() - borde, (short int)obj->getY() - borde,
            (short unsigned int)(obj->getW() + borde*2), (short unsigned int)(obj->getH() + borde*2) };

        if (obj->getObjectType() == GUICOMBOBOX){
            if (obj->isChecked()){
                //Si se ha pulsado el combo, tenemos que cachear el alto de la lista desplegada tambien
                imgLocation.h = COMBOHEIGHT + Constant::getCOMBOLISTHEIGHT() + borde*2;
            } else {
                imgLocation.h = COMBOHEIGHT + borde*2;
            }
        }


        if (!obj->getImgDrawed()){
            //Reseteamos el surface
            obj->setSurfaceCache(NULL);
            //Guardamos la imagen en el objeto
            takeScreenShot(&obj->surfaceCache, imgLocation);
            //Indicamos que ya hemos pintado la imagen
            obj->setImgDrawed(true);
        } else {
            printScreenShot(&obj->surfaceCache, imgLocation);
        }
    }
}

/**
*
*/
void Ioutil::drawUIPopupFondo(Object *obj, int borde){
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();
        if (w > borde*2 && h > borde*2)
            drawRectAlpha(x + borde, y + borde, w - borde*2, h-borde*2, cNegro, 128);
    }
}

/**
*
*/
void Ioutil::drawUILetraPopup(Object *obj){

    //UIListGroup *listObj = (UIListGroup *)dynamic_cast<UIListGroup*>(obj);
    UIListGroup *listObj = (UIListGroup *)(obj);

    if (listObj->isVisible()){

        int x = listObj->getX();
        int y = listObj->getY();
        int w = listObj->getW();
        int h = listObj->getH();
        const int borde = 20;
        int xCent = (x + listObj->getW()) / 2;
        int yCent = (y + listObj->getH()) / 2;
        int radio = h/2 - borde;
        if (w/2 - borde < radio)
            radio = w/2 - borde;
        int bordeLetra = 40;

        radio = radio - bordeLetra;

        const unsigned int numAngles = 26;
        const int sepLetras = 10;
        float minAngle = 2 * PI / (float)numAngles;
        float angle = 0;
        int xCircle = xCent + x/2;
        int yCircle = yCent + y/2;
        int h1 = fontHeight/2;
        int w1 = fontStrLen("M")/2;
        const char *letra ;
        //Uint32 azulOscuro = SDL_MapRGB(screen->format, 50,104,202);

        for (unsigned int i = 0; i < numAngles; i++){
            angle = minAngle * i;
            //Draw_Line(screen, xCircle, yCircle, xCircle + radio * cos(angle), yCircle - radio * sin(angle), gris);

            if (i + 65 == listObj->getLetraPopup()){
                for (float alpha = angle; alpha < minAngle * (i+1); alpha += 0.0001 ){
                    pintarLinea(xCircle, yCircle, xCircle + radio * cos(alpha), yCircle - radio * sin(alpha), cAzulOscuro);
                }
            }

            letra = Constant::TipoToStr((char)(65+i)).c_str();
            drawText(letra,
                     xCircle - w1 + (radio + sepLetras) * cos(angle + minAngle/2),
                     yCircle - h1 - (radio + sepLetras) * sin(angle + minAngle/2),
                     cBlanco);
        }

        letra = Constant::TipoToStr((char)listObj->getLetraPopup()).c_str();
        loadFont(FONTSIZE*5);
        int xCentChar = xCent - (fontStrLen(letra) / 2);
        int yCentChar = yCent - (fontHeight / 2);
        //Draw_FillCircle(screen, xCent + x/2, yCent + y/2, fontHeight / 2, SDL_MapRGB(screen->format, 0,0,0));
        //pintarSemiCirculo(xCent + x/2, yCent + y/2, fontHeight / 2, cNegro, minAngle * (listObj->getLetraPopup() - 65));
        //pintarCirculo(xCent + x/2, yCent + y/2, radio, cNegro);
        drawText(letra, xCentChar + x/2 , yCentChar , cBlanco);
        loadFont(FONTSIZE);
    }
}

/**
*
*/
void Ioutil::drawUIPopupMenu(Object *obj){
     if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();

        UIPopupMenu *listObj = (UIPopupMenu *)obj;

        if (!listObj->getImgDrawed()){
            int centeredY = (Constant::getMENUSPACE() - fontHeight) / 2;
            Traza::print("Repintando popup: " + listObj->getLabel(), W_PARANOIC);
            Traza::print("Alto del popup: ", h, W_PARANOIC);
            pintarContenedor(x,y,w,h, listObj->isFocus() && listObj->isEnabled(), obj, cInputContent);
            x += INPUTCONTENT;
            y += INPUTCONTENT;

            t_color colorText = listObj->isEnabled() && listObj->isFocus() ? cNegro : cGris;

            Traza::print("listObj->isEnabled()",listObj->isEnabled(), W_PARANOIC);
            Traza::print("listObj->isFocus()",listObj->isFocus(), W_PARANOIC);

            int cont = 0;
            if (listObj->getObjectType() == GUIPOPUPMENU && listObj->getSize() > 0){
                int icono = -1;
                //Ponemos el color por defecto para las listas
                colorText = listObj->isEnabled() && listObj->isFocus() ? cNegro : cGris;
                TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
                Traza::print("listObj->getPosIniLista(): ", listObj->getPosIniLista(), W_PARANOIC);
                Traza::print("listObj->getPosFinLista(): ", listObj->getPosFinLista(), W_PARANOIC);

                for (unsigned int i=listObj->getPosIniLista(); i <= listObj->getPosFinLista(); i++ ){
                    Traza::print("pintando: " + listObj->getListNames()->get(i), W_PARANOIC);
                    icono = listObj->getListIcons()->get(i);
                    if (i == listObj->getPosActualLista()){
                        drawRectAlpha(obj->getX() + 2, obj->getY() + 2 + cont*Constant::getMENUSPACE(), listObj->getW() - 3, Constant::getMENUSPACE() + INPUTCONTENT - 3, cAzulOscuro, listObj->isFocus() ? 255 : 128);
                        colorText = cBlanco;
                        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
                        drawText(listObj->getListNames()->get(i).c_str() , x + ((icono >= 0) ? ICOSPACE : 0), y + centeredY + cont*Constant::getMENUSPACE(), colorText);
                        colorText = listObj->isEnabled() && listObj->isFocus() ? cNegro : cGris;
                        TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
                    } else {
                        drawText(listObj->getListNames()->get(i).c_str() , x + ((icono >= 0) ? ICOSPACE : 0), y + centeredY + cont*Constant::getMENUSPACE(), colorText);
                    }

                    if (i < listObj->getPosFinLista())
                        pintarLinea(x + ((icono >= 0) ? ICOSPACE : 0), y + (cont+1)*Constant::getMENUSPACE(), w,  y + (cont+1)*Constant::getMENUSPACE(), cSeparator);

                    if (icono >= 0){
                        drawIco(icono, x, y + centeredY + cont*Constant::getMENUSPACE());
                    }

                    cont++;
                }
            }
            cachearObjeto(obj);
        } else {
            //Traza::print("Alto de la lista: ", h, W_DEBUG);
            cachearObjeto(obj);
        }
    }
}

/**
* Dibuja una lista en forma de combo
*/
void Ioutil::drawUIComboBox(Object *obj){
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();

            //Dibujando el label del input
            drawText(obj->getLabel().c_str(),x,y-FONTSIZE-4,cAzulOscuro);

        if (!obj->getImgDrawed()){
            UIComboBox *listObj = (UIComboBox *)obj;
            t_color colorText = listObj->isEnabled() ? cNegro : cGris;


            //Pintamos el contenedor del elemento seleccionado en el combo
            pintarContenedor(x, y, w, COMBOHEIGHT, listObj->isFocus() && listObj->isEnabled(), obj, cInputContent);
            //Pintamos la flechita caracteristica del combo con su correspondiente contenedor
            pintarLinea(x + w - 30, y, x + w - 30, y + COMBOHEIGHT, cInputBorder);
            pintarTriangulo (x + w - 15, y + COMBOHEIGHT - 5, 10, 10, false, colorText);

            if (listObj->getObjectType() == GUICOMBOBOX && listObj->getSize() > 0){
                int icono = -1;
                //Ponemos el color por defecto para las listas
                TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
                Traza::print("listObj->getPosIniLista(): ", listObj->getPosIniLista(), W_PARANOIC);
                Traza::print("listObj->getPosFinLista(): ", listObj->getPosFinLista(), W_PARANOIC);

                //Si se ha pulsado el combo, mostramos la lista del combo
                if (obj->isChecked()){
                    pintarContenedor(x, y + COMBOHEIGHT, w, Constant::getCOMBOLISTHEIGHT(), listObj->isFocus() && listObj->isEnabled(), obj, cInputContent);
                }

                //Incorporamos algo de espacio para pintar los contenidos del combo y de la lista
                x += INPUTCONTENT;
                y += INPUTCONTENT;

                //Pintamos el texto del elemento seleccionado que aparecera en el combo
                int selectedPos = listObj->getPosActualLista();
                SDL_Rect textArea = { 0, 0, w - 30 - INPUTCONTENT, Constant::getMENUSPACE() };
                drawTextInArea(listObj->getListNames()->get(selectedPos).c_str() , x + ((icono >= 0) ? ICOSPACE : 0), y, colorText, &textArea);
                Traza::print("pintamos el objeto seleccionado como: " + listObj->getListNames()->get(selectedPos), W_DEBUG);
                //pintarLinea(x, y + Constant::getINPUTH(), x + w - 2*INPUTCONTENT,  y + Constant::getINPUTH(), colorText);

                if (listObj->isChecked()){
                    listObj->calcularScrPos();
                    drawListContent(listObj, x, y + COMBOHEIGHT, w, Constant::getCOMBOLISTHEIGHT());
                }
            }
            cachearObjeto(obj);
        } else {
            cachearObjeto(obj);
        }
    }
}

/**
* Pone una marca en la posicion que se le indique
*/
void Ioutil::marcarPos(int x, int y){
    putpixelSafe(screen,x-1,y-1,SDL_MapRGB(screen->format, 255, 0, 0));
    putpixelSafe(screen,x-1,y+1,SDL_MapRGB(screen->format, 255, 0, 0));
    putpixelSafe(screen,x+1,y-1,SDL_MapRGB(screen->format, 255, 0, 0));
    putpixelSafe(screen,x+1,y+1,SDL_MapRGB(screen->format, 255, 0, 0));
    putpixelSafe(screen,x,y,SDL_MapRGB(screen->format, 255, 0, 0));
}

/**
* Pinta una lista
*/
void Ioutil::drawUIListGroupBox(Object *obj){
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();
        UIListGroup *listObj = (UIListGroup *)obj;

        if (listObj->isShowLetraPopup()){
                //Forzamos para que se repinte lo ultimo que habia por pantalla
                if (!listObj->getImgDrawed()){
                    listObj->setImgDrawed(true);
                }
                cachearObjeto(obj);
                //Pintamos el fondo traslucido para darle un efecto de ventana emergente
                //y lo volvemos a cachear una sola vez
                if (listObj->getBgLetraPopup() == false){
                    listObj->setBgLetraPopup(true);
                    listObj->setImgDrawed(false);
                    drawUIPopupFondo(obj, 20);
                    cachearObjeto(obj);
                }
                //Llamamos a la funcion que dibuja la seleccion de letras
                drawUILetraPopup(obj);
        } else if (!listObj->getImgDrawed()){
            int centeredY = (Constant::getMENUSPACE() - fontHeight) / 2;
            listObj->setBgLetraPopup(false);
            Traza::print("Repintando lista: " + listObj->getLabel(), W_DEBUG);
            Traza::print("Alto de la lista: ", h, W_PARANOIC);
            pintarContenedor(x,y,w,h,listObj->isFocus() && listObj->isEnabled() && listObj->isVerContenedor(), obj, obj->getColor());
            x += INPUTCONTENT;
            y += INPUTCONTENT;

            if (listObj->getObjectType() == GUILISTGROUPBOX && listObj->getSize() > 0){
                Traza::print("listObj->getPosIniLista(): ", listObj->getPosIniLista(), W_PARANOIC);
                Traza::print("listObj->getPosFinLista(): ", listObj->getPosFinLista(), W_PARANOIC);
                //Dibujamos el resto de la lista
                drawListGroupContent(obj, x, y, w, h);
            } else if (listObj->getObjectType() == GUILISTIMG && listObj->getSize() > 0){
                if (((UIImgList *)obj)->getMode() == IMGGROUPMODE){
                    drawListGroupContent(obj, x, y, w, h);
                } else if (((UIImgList *)obj)->getMode() == IMGTHUMBMODE) {
                    drawUIThumbnailImgBox(obj);
                }
            }
            cachearObjeto(listObj);
        } else {
            //Traza::print("Alto de la lista: ", h, W_DEBUG);
            cachearObjeto(listObj);
        }
        drawAllThumbnailBackgrounds(listObj);
    }
}

/**
 * 
 * @param listImages
 */
bool Ioutil::startLoadingThumbs(UIImgList *listImages){
    return imgLoader->startLoading(listImages, screen);
}

/**
 * 
 * @param obj
 */
void Ioutil::drawUIThumbnailImgBox(Object *obj){
    UIImgList *listObj = (UIImgList *)obj;
    int imgIndex = 0;
    t_region regionPantalla = {obj->getX(), obj->getY(), obj->getW(), obj->getH()};
    SDL_Rect *imgRect = new SDL_Rect();
    int newImagesToDraw = 0;
    
    for (unsigned int i=listObj->getPosIniLista(); i <= listObj->getPosFinLista(); i++ ){
        if (listObj->getRow(i) != NULL){
            UIPicture* pict = listObj->getRow(i)->GetUipicture();
            if (pict != NULL){
                if (pict->isSafeToDraw() && pict->getImgDrawed()){
                    pict->getImgGestor()->calcImgLocationFromIndex(imgIndex, pict->getW(), pict->getH()-1, regionPantalla, imgRect);
                    pict->setX(imgRect->x + pict->getImgGestor()->getImgLocationRelScreen().x);
                    pict->setY(imgRect->y + pict->getImgGestor()->getImgLocationRelScreen().y);
                    cachearObjeto(pict);
                }
            } else {
                newImagesToDraw++;
            }
        }
        imgIndex++;
    }
    
    Traza::print("newImagesToDraw", newImagesToDraw, W_DEBUG);
    if (newImagesToDraw > 0){
        if (listObj->isReloadImages()){
            startLoadingThumbs(listObj);
            listObj->setReloadImages(false);
        }
        listObj->setImgDrawed(false);
    }
    delete imgRect;
}

/**
 * 
 * @param obj
 */
void Ioutil::drawAllThumbnailBackgrounds(UIListGroup *obj){
    if (obj->getObjectType() == GUILISTIMG && obj->getSize() > 0){
        UIImgList *listObj = (UIImgList *)obj;
        if (listObj->getMode() == IMGTHUMBMODE) {
            int imgIndex = 0;
            loadFont(11);
            t_region regionPantalla = {obj->getX(), obj->getY(), obj->getW(), obj->getH()};
            SDL_Rect *imgRect = new SDL_Rect();
            for (unsigned int i=listObj->getPosIniLista(); i <= listObj->getPosFinLista(); i++ ){
                UIPicture* pict = listObj->getRow(i)->GetUipicture();
                t_posicion posicion;
                
                if (pict != NULL){
                    pict->getImgGestor()->calcImgLocationFromIndex(imgIndex, pict->getW(), pict->getH()-1, regionPantalla, imgRect);
                    posicion = pict->getImgGestor()->getImgLocationRelScreen();
                    if ((imgRect != NULL && posicion.w > 0 && posicion.h > 0) || i == listObj->getPosActualLista()){
                        drawThumbnailSelected(posicion, imgRect, i == listObj->getPosActualLista());
                    }
                } else {
                    listObj->getImgGestor()->calcImgLocationFromIndex(imgIndex, listObj->getPrevImgWidth(), listObj->getPrevImgHeight(), regionPantalla, imgRect);
                    drawThumbnailSelected(posicion, imgRect, i == listObj->getPosActualLista());
                }
                drawThumbnailText(listObj, i, imgRect);
                imgIndex++;
            }
            delete imgRect;
            loadFont(FONTSIZE);
        }
    }
}

/**
 * 
 * @param posicion: Posicion de la imagen respecto a su contenedor
 * @param imgRect: Contenedor de la imagen
 * @param selected
 */
void Ioutil::drawThumbnailSelected(t_posicion posicion, SDL_Rect *imgRect, bool selected){
    t_color cThumb = cColorThumbSelected;
    t_color cThumbBorder = cColorThumbSelectedBorder;
    
    if (selected){
        cThumb = cColorThumbSelected;
        cThumbBorder = cColorThumbSelectedBorder;
    } else {
        cThumb = cGrisClaro;
        cThumbBorder = cGris;
    }
    
    if (imgRect != NULL && posicion.w > 0 && posicion.h > 0){
        posicion.y += imgRect->y;
        posicion.x += imgRect->x;
        Sint16 dif = 2;
        
        if (selected){
            Sint16 calcTop = posicion.y - imgRect->y;
            Sint16 calcLeft = posicion.x - imgRect->x;
            Sint16 calcBottom = imgRect->y + imgRect->h - posicion.y - posicion.h;
            Sint16 calcRight = imgRect->x + imgRect->w - posicion.x - posicion.w;
            drawRect(imgRect->x, imgRect->y, imgRect->w, calcTop - dif, cThumb);
            drawRect(imgRect->x, imgRect->y, calcLeft - dif, imgRect->h, cThumb);
            drawRect(imgRect->x + calcLeft - dif, posicion.y + posicion.h - dif, posicion.w + dif, calcBottom + dif, cThumb);
            drawRect(posicion.x + posicion.w - dif, imgRect->y + calcTop - dif, calcRight + dif, imgRect->h - calcTop + dif, cThumb);
            drawRectLine(imgRect->x, imgRect->y, imgRect->w, imgRect->h, 1, cThumbBorder);
        } 
        
        //Degradado derecho
        pintarDegradadoDesplazado(posicion.x + posicion.w - dif,
                        posicion.y - 1,
                        posicion.x + posicion.w - dif,
                        posicion.y + posicion.h - dif,
                        3 , 1, 180, 220);

        //Degradado inferior
        pintarDegradadoDesplazado(posicion.x - 1,
                        posicion.y + posicion.h - dif,
                        posicion.x + posicion.w - dif,
                        posicion.y + posicion.h - dif,
                        3 , 1, 180, 220);
    } else {
        drawRectAlpha(imgRect->x, imgRect->y, imgRect->w, imgRect->h, cThumb, 150);
        drawRectLine(imgRect->x, imgRect->y, imgRect->w, imgRect->h, 1, cThumbBorder);
    }
}

/**
 * 
 * @param listObj
 * @param i
 * @param imgRect
 */
void Ioutil::drawThumbnailText(UIImgList *listObj, int i, SDL_Rect *imgRect){
    
    if (listObj == NULL)
        return;
    if (listObj->getSize() < 1)
        return;
    if (listObj->getRow(i) == NULL)
        return;
    if (listObj->getRow(i)->GetUipicture() == NULL)
        return;
    if (listObj->getRow(i)->GetListGroupCol().size() < 1)
        return;
    
    UIPicture* pict = listObj->getRow(i)->GetUipicture();
    string text = listObj->getRow(i)->GetListGroupCol().at(0)->getTexto();
    int posX = 0, posY = 0, posW = 0;
    int textW = fontStrLen(text);
    
//    posX = pict->getX() - pict->getImgGestor()->getImgLocationRelScreen().x;
//    if (pict->getW() > textW){
//        posX += (pict->getW() - textW) / 2;
//    }
//    posY = pict->getY() + pict->getH() - Constant::getMENUSPACE();
//    posW = pict->getW();
    
    posX = imgRect->x;
    if (imgRect->w > textW){
        posX += (imgRect->w - textW) / 2;
    }
    posY = imgRect->y + imgRect->h - Constant::getMENUSPACE();
    posW = imgRect->w;

    SDL_Rect textArea = { 0, 0, posW, Constant::getMENUSPACE() };
    
    drawTextInArea(text.c_str(), 
            posX,
            posY,
            listObj->isEnabled() ? cNegro : cGris,
            &textArea);
}

/**
* Metodo comun para pintar el listado y la lista del combo
*/
void Ioutil::drawListGroupContent(Object *obj, int x, int y, int w, int h){

    if (obj->isVisible()){
        try{

            UIListGroup *listObj = (UIListGroup *)obj;
            //t_color colorText = listObj->isEnabled() && listObj->isFocus() ? cNegro : cGris;
            t_color colorText = listObj->isEnabled() ? obj->getTextColor() : cGris;
            int cont = 0;
            int centeredY = 0;
            centeredY = (Constant::getMENUSPACE() - fontHeight) / 2;
            int posObjY = 0;

            //Pintamos el fondo de la cabecera
    //        drawRectAlpha(x - INPUTCONTENT + 1, y, listObj->getW() - 1,
    //                              Constant::getMENUSPACE(), cGrisClaro, listObj->isFocus() ? 255 : 128);
            pintarDegradado(x - INPUTCONTENT + 1,
                            y - INPUTCONTENT,
                            x - INPUTCONTENT - 1 + w,
                            y - INPUTCONTENT,
                            Constant::getMENUSPACE() + INPUTCONTENT , 180, 255);
            pintarDegradado(x - INPUTCONTENT + 1,
                            y - INPUTCONTENT,
                            x - INPUTCONTENT - 1 + w,
                            y - INPUTCONTENT,
                            5, 128, 180);



            if (listObj->isColsAdjustedToHeader() || listObj->sizeHeader() <= 0)
                listObj->clearHeaderWith();

            int sepCabecera = 0;
            SDL_Rect textArea = { 0, 0, 0, Constant::getMENUSPACE() };

            //Dibujamos las cabeceras
            for (unsigned int contCol=0; contCol < listObj->getSizeCol(); contCol++ ){
                int headerPixelSize = 0;
                TTF_SizeText(this->font,listObj->getHeaderCol(contCol)->getTexto().c_str(),&headerPixelSize,NULL );
                if (listObj->isColsAdjustedToHeader() || listObj->sizeHeader() <= contCol) {
                    listObj->addHeaderWith(headerPixelSize);
                }

                int temp = 0;
                //Solo pintamos texto que quepa en la columna
                textArea.w = listObj->getHeaderWith(contCol);
                //comprobamos que el texto no salga de los limites de la tabla
                if (x + sepCabecera + headerPixelSize > obj->getX() + obj->getW()){
                    if (x + sepCabecera > obj->getX() + obj->getW())
                        textArea.w = 0; //Cuando el texto ya se ha salido totalmente
                    else
                        textArea.w = obj->getX() + obj->getW() - (x + sepCabecera); //Cuando el texto esta fuera parcialmente
                } else if (sepCabecera <= 0 && contCol > 0){
                      Traza::print("Salimos del limite izquierdo", W_PARANOIC);
                      temp = sepCabecera;
                }

                //Pintamos el texto
                TTF_SetFontStyle(font, TTF_STYLE_BOLD);
                drawTextInArea(listObj->getHeaderCol(contCol)->getTexto().c_str(),
                         x + sepCabecera,
                         centeredY + y,
                         listObj->isEnabled() ? cNegro : cGris,
                         &textArea);
                TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

                sepCabecera += listObj->getHeaderWith(contCol) + INPUTCONTENT;
            }

             textArea.x = 0;
            //Primero recorremos todas las filas
            for (unsigned int i=listObj->getPosIniLista(); i <= listObj->getPosFinLista(); i++ ){
                Traza::print("pintando la fila",i, W_PARANOIC);
                posObjY = y + Constant::getMENUSPACE() + cont*Constant::getMENUSPACE();

                //Dibujamos el rectangulo del elemento seleccionado
                if (i == listObj->getPosActualLista()){
                    drawRectAlpha(x - INPUTCONTENT, posObjY, listObj->getW(),
                                  Constant::getMENUSPACE(), cAzul, listObj->isFocus() ? 255 : 128);
                    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
                    colorText = listObj->isEnabled() ? cBlanco : cGris;
                } else if (i == listObj->getLastSelectedPos() && listObj->isEnableLastSelected()){
                    //Dibujamos el rectangulo del ultimo elemento seleccionado
                    drawRectAlpha(x - INPUTCONTENT, posObjY, listObj->getW(),
                                  Constant::getMENUSPACE(), cVerde, 50);
                    colorText = listObj->isEnabled() ? obj->getTextColor() : cGris;
                } else {
                    //Dibujamos el resto de elementos con el formato adecuado
                    colorText = listObj->isEnabled() ? obj->getTextColor() : cGris;
                }

                //Se pinta el separador de elementos horizontales
                if (i > 0){
                    pintarLinea(x, y + (cont+1)*Constant::getMENUSPACE(), x + w - 2*INPUTCONTENT,  y + (cont+1)*Constant::getMENUSPACE(), cSeparator);
                }

                int sepGrupos = 0;
                Traza::print("pintando columnas de la fila",i, W_PARANOIC);
                //Ahora recorremos todas las columnas
                for (unsigned int contCol=0; contCol < listObj->getSizeCol(); contCol++ ){
                    ListGroupCol *column = listObj->getCol(i,contCol);
                    int sepIco = ((column->getIcono() >= 0) ? ICOSPACE : 0);
                    //Solo pintamos texto que quepa en la columna
                    textArea.w = listObj->getHeaderWith(contCol) - sepIco;
                    Traza::print("Ancho de columna",textArea.w, W_PARANOIC);
                    //comprobamos que el texto no salga de los limites de la tabla
                    int headerPixelSize = 0;
                    
                    if (column != NULL){
                        TTF_SizeText(this->font, column->getTexto().c_str(), &headerPixelSize, NULL );
                        Traza::print("Tam. Texto Col", headerPixelSize, W_PARANOIC);
                        
                        if (x + sepGrupos + headerPixelSize > obj->getX() + obj->getW()){
                            if (x + sepGrupos > obj->getX() + obj->getW())
                                textArea.w = 0;
                            else
                                textArea.w = obj->getX() + obj->getW() - (x + sepGrupos);
                        }
                    
                        Traza::print("pintando la columna: " + column->getTexto() , W_PARANOIC);
                        drawTextInArea(column->getTexto().c_str() , x + sepGrupos + sepIco, centeredY + posObjY, colorText, &textArea);
//                        //Se pinta el triangulo superior
//                        if (listObj->getPosIniLista() > 0){
//                            pintarTriangulo (listObj->getX() + listObj->getW() - 6, listObj->getY() + INPUTCONTENT + Constant::getMENUSPACE(), TRISCROLLBARTAM, TRISCROLLBARTAM, true, colorText);
//                        }
//                        //Se pinta el triangulo inferior
//                        if (listObj->getPosFinLista() + 1 < (unsigned int)listObj->getSize()){
//                            pintarTriangulo (listObj->getX() + listObj->getW() - 6, listObj->getY() + listObj->getH() - INPUTCONTENT, TRISCROLLBARTAM, TRISCROLLBARTAM, false, colorText);
//                        }
                        if (column->getIcono() >= 0){
                            drawIco(column->getIcono(), x + sepGrupos, centeredY + posObjY);
                        }
                    }
                    sepGrupos += listObj->getHeaderWith(contCol) + INPUTCONTENT;
                }
                cont++;
                TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
            }

            //Pintamos la barrita de desplazamiento
            drawScrollBar(listObj);


            Traza::print("pintando limites de columnas", W_PARANOIC);
            //Finalmente dibujamos los limites de las columnas
            int acumWidth = 0;
            int selectedPosX = 0;
            int selectedPosY = 0;
            bool colAnchorPressed = false;

            for (int i=0; i < listObj->getSizeCol() ; i++){
                acumWidth += listObj->getHeaderWith(i) + (i > 0 ? INPUTCONTENT : 0);
                selectedPosX = x + acumWidth;
                selectedPosY = y - INPUTCONTENT;
                colAnchorPressed = listObj->isColAnchorPressed() && listObj->getPosColAnchorPressed() == i;

                if (selectedPosX < listObj->getX() + listObj->getW() && selectedPosX > listObj->getX()){
                    pintarLinea(selectedPosX , selectedPosY, selectedPosX, selectedPosY + h - 2, colAnchorPressed ? cNegro : cGris);
                    if (colAnchorPressed){
                        pintarLinea(selectedPosX - 1, selectedPosY, selectedPosX - 1, selectedPosY + h - 2, cNegro);
                    }
                    int order = listObj->getHeaderCol(i)->getSortOrder();
                    if (order == 0){
                        pintarTriangulo(selectedPosX - TRIGLTAM,
                                        selectedPosY + Constant::getMENUSPACE()/2 + TRIGLTAM / 2,
                                        TRIGLTAM, TRIGLTAM, false, cNegro);
                    } else if (order == 1){
                        pintarTriangulo(selectedPosX - TRIGLTAM,
                                        selectedPosY + Constant::getMENUSPACE()/2 - TRIGLTAM / 2,
                                        TRIGLTAM, TRIGLTAM, true, cNegro);
                    }
                }
            }
        } catch (Excepcion &e){
            Traza::print("Error al pintar la lista agrupada: " + string(e.getMessage()), W_ERROR);
        }
    }
}



/**
* Pinta una lista
*/
void Ioutil::drawUIListBox(Object *obj){
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();
        UIList *listObj = (UIList *)obj;

        if (listObj->isShowLetraPopup()){
                //Forzamos para que se repinte lo ultimo que habia por pantalla
                if (!listObj->getImgDrawed()){
                    listObj->setImgDrawed(true);
                }
                cachearObjeto(obj);
                //Pintamos el fondo traslucido para darle un efecto de ventana emergente
                //y lo volvemos a cachear una sola vez
                if (listObj->getBgLetraPopup() == false){
                    listObj->setBgLetraPopup(true);
                    listObj->setImgDrawed(false);
                    drawUIPopupFondo(obj, 20);
                    cachearObjeto(obj);
                }
                //Llamamos a la funcion que dibuja la seleccion de letras
                drawUILetraPopup(obj);
        } else if (!listObj->getImgDrawed()){
            int centeredY = (Constant::getMENUSPACE() - fontHeight) / 2;
            listObj->setBgLetraPopup(false);
            Traza::print("Repintando lista: " + listObj->getLabel(), W_PARANOIC);
            Traza::print("Alto de la lista: ", h, W_PARANOIC);

            if (!listObj->isSmoothDraw()){
                pintarContenedor(x,y,w,h,listObj->isFocus() && listObj->isEnabled(), obj, obj->getColor());
                x += INPUTCONTENT;
                y += INPUTCONTENT;

                t_color colorText = listObj->isEnabled() && listObj->isFocus() ? obj->getTextColor() : cGris;
                if (listObj->getObjectType() == GUILISTBOX && listObj->getSize() > 0){
                    //Ponemos el color por defecto para las listas
                    colorText = listObj->isEnabled() && listObj->isFocus() ? obj->getTextColor() : cGris;
                    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
                    Traza::print("listObj->getPosIniLista(): ", listObj->getPosIniLista(), W_PARANOIC);
                    Traza::print("listObj->getPosFinLista(): ", listObj->getPosFinLista(), W_PARANOIC);
                    //Dibujamos el resto de la lista
                    if (listObj->getListScheme() == SCHEMEICONS){
                        drawListIcoHor(obj, x, y, w, h);
                    } else {
                        drawListContent(obj, x, y, w, h);
                    }
                }
            } else {
                if (listObj->getListScheme() == SCHEMEICONS){
                    drawListIcoHorSmooth(listObj, x, y, w, h);
                }
            }

            cachearObjeto(obj);
        } else {
            Traza::print("Alto de la lista: ", h, W_PARANOIC);
            cachearObjeto(obj);
        }
    }
}

/**
*
*/
void Ioutil::drawListIcoHorSmooth(UIList *obj, int x, int y, int w, int h){

    string msg = Constant::TipoToStr(obj->getPosActualLista()) + ":"
    + Constant::TipoToStr(obj->getLastPosActualLista()) + "; getPosIniLista: "
    + Constant::TipoToStr(obj->getPosIniLista()) + " - getPosFinLista: "
    + Constant::TipoToStr(obj->getPosFinLista());

    static int lastPos = 0;
    int sfcW = 0;
    int sfcH = 0;
    const int nElems = obj->getNIconsHoriz();
    const int elemWidth = w / nElems;
    const int elemHeight = h / nElems;
    const int centeredY = (h - elemHeight) / 2;
    //La imagen contenida debe generar tambien los lados de los extremos de la lista
    int totalWsfc = (obj->getNIconsHoriz() + 2) * elemWidth;
    string rutaLogo;
    ImagenGestor imgGestor;
    SDL_Surface *mySurface;
    SDL_Surface *fondoSrf;
    SDL_Rect dstrect = {0,0, elemWidth, elemHeight};
    SDL_Rect scrRect = {x, y + centeredY,w, elemHeight};
    SDL_Rect fondoRect = {x, y + centeredY,w, elemHeight};
    SDL_Rect smoothRect = {0,0, w, elemHeight};

    if (obj->smoothSfc != NULL){
        if (obj->smoothSfc->w != totalWsfc || obj->smoothSfc->h != elemHeight){
            SDL_FreeSurface(obj->smoothSfc);
            obj->smoothSfc = SDL_CreateRGBSurface(SDL_SWSURFACE, totalWsfc, elemHeight,
                        screen->format->BitsPerPixel,
                        rmask,
                        gmask,
                        bmask,
                        amask);
        }
    } else {
        obj->smoothSfc = SDL_CreateRGBSurface(SDL_SWSURFACE, totalWsfc, elemHeight,
                        screen->format->BitsPerPixel,
                        rmask,
                        gmask,
                        bmask,
                        amask);
    }

    sfcW = obj->smoothSfc->w;
    sfcH = obj->smoothSfc->h;

    fondoSrf = SDL_CreateRGBSurface(SDL_SWSURFACE, fondoRect.w, fondoRect.h,
                        screen->format->BitsPerPixel,rmask,
                        gmask,
                        bmask,
                        amask);


    int inicio = obj->getPosIniLista() > 0 ? obj->getPosIniLista() - 1 : 0;
    int fin = obj->getPosFinLista() < obj->getSize() - 1 ? obj->getPosFinLista() + 1 : obj->getPosFinLista();
    int total = fin - inicio + 1;


    //Para actualizar el fondo mientras movemos la imagen
    SDL_BlitSurface(screen, &fondoRect, fondoSrf, NULL);

    if (lastPos != obj->getPosActualLista()){
        if (obj->getPosActualLista() > obj->getLastPosActualLista()){
            msg += " Derecha";
            if (obj->getPosActualLista() > 1){
                SDL_Rect itemRect = {0, 0, sfcW, elemHeight};

                int itmp=0;
                int relPostmp=0;
                while (itmp < total && inicio + itmp <= fin && inicio + itmp < obj->getSize()){
                    relPostmp = inicio + itmp;
                    if (relPostmp < obj->getPosIniLista() && obj->getPosActualLista() > obj->getNIconsHoriz() -1){
                        itemRect.x += elemWidth;
                    }
                    itmp++;
                }

                int sumaDespl = 0;
                const int desp = elemWidth / 10;

                while (sumaDespl < elemWidth){
                    sumaDespl += desp;
                    itemRect.x += sumaDespl < elemWidth ? desp : (sumaDespl - elemWidth);
                    //SDL_BlitSurface(fondoSrf,NULL, screen, &fondoRect);
                    SDL_BlitSurface(obj->smoothSfc, &itemRect, screen, &scrRect);
                    SDL_UpdateRect(screen, scrRect.x, scrRect.y, scrRect.w, scrRect.h);
                    //SDL_Delay(1);
                    SDL_BlitSurface(fondoSrf, NULL, screen, &fondoRect);
                }
            }
        } else if (obj->getPosActualLista() < obj->getLastPosActualLista()){
            msg += " Izquierda";
            if (obj->getLastPosActualLista() > 1){
                SDL_Rect itemRect = {elemWidth, 0, sfcW, elemHeight};

                int sumaDespl = 0;
                const int desp = elemWidth / 10;

                while (itemRect.x > 0){
                    sumaDespl += desp;
                    itemRect.x -= sumaDespl < elemWidth ? desp : (sumaDespl - elemWidth);
                    //SDL_BlitSurface(fondoSrf,NULL, screen, &fondoRect);
                    SDL_BlitSurface(obj->smoothSfc, &itemRect, screen, &scrRect);
                    SDL_UpdateRect(screen, scrRect.x, scrRect.y, scrRect.w, scrRect.h);
                    //SDL_Delay(100);
                    SDL_BlitSurface(fondoSrf, NULL, screen, &fondoRect);
                }
            }
        }
    }

    Traza::print("Inicio: " + Constant::TipoToStr(inicio) + "; Fin: " + Constant::TipoToStr(fin), W_PARANOIC);
    //Reseteamos a todo transparente
    SDL_FillRect(obj->smoothSfc, NULL, obj->smoothSfc->format->colorkey);

    int i=0;
    int relPos=0;
    t_color colorText = obj->isEnabled() ? obj->getTextColor() : cGris;
    string text;
    Dirutil dir;

    while (i < total && inicio + i <= fin && inicio + i < obj->getSize()){
        relPos = inicio + i;

        if (relPos < obj->getPosIniLista()){
            smoothRect.x += elemWidth;
        }

        rutaLogo = dirInicial + Constant::getFileSep() + "emuImgs" + Constant::getFileSep()
            + "logos" + Constant::getFileSep() + obj->getListImage()->get(relPos) + ".png";

        //Traza::print("rutaLogo: " + rutaLogo, W_PARANOIC);
        SDL_Rect textArea = { 0, 0, elemWidth, elemHeight };

        if (relPos != obj->getPosActualLista()){
            textArea.w = elemWidth - elemWidth / 3;
            textArea.h = elemHeight - elemHeight / 3;
        } else {
            textArea.w = elemWidth;
            textArea.h = elemHeight;
        }

        if (dir.existe(rutaLogo) && imgGestor.loadImgFromFileZoom(rutaLogo.c_str(), &mySurface, &textArea)){
            dstrect.x = i * elemWidth + (elemWidth - mySurface->w) / 2;
            dstrect.y = (elemHeight - mySurface->h) / 2;
            SDL_SetColorKey(mySurface, mySurface->format->colorkey, SDL_ALPHA_OPAQUE);
            SDL_SetAlpha( mySurface, 0, SDL_ALPHA_OPAQUE );
            SDL_BlitSurface(mySurface, NULL, obj->smoothSfc, &dstrect);
            SDL_FreeSurface( mySurface );
            mySurface = NULL;
        } else {
            int fontStyle = TTF_GetFontStyle(font);

            if (!(fontStyle & TTF_STYLE_BOLD) && relPos == obj->getPosActualLista()) {
                loadFont(FONTSIZE + 8);
                TTF_SetFontStyle(font, fontStyle | TTF_STYLE_BOLD);
            }
            text = obj->getListNames()->get(relPos);
            dstrect.x = i * elemWidth + (elemWidth - fontStrLen(text)) / 2;
            drawTextInAreaSrf(text.c_str(), dstrect.x, (elemHeight - fontHeight) / 2, colorText, NULL, obj->smoothSfc, true);

            if (relPos == obj->getPosActualLista()) {
                loadFont(FONTSIZE);
                TTF_SetFontStyle(font, fontStyle);
            }
        }
        i++;
    }

    SDL_BlitSurface(obj->smoothSfc, &smoothRect, screen, &scrRect);
    SDL_FreeSurface(fondoSrf);
    lastPos = obj->getPosActualLista();
    Traza::print("drawListIcoHorSmooth: " + msg, W_PARANOIC);
}


/**
*
*/
void Ioutil::drawListIcoHor(Object *obj, int x, int y, int w, int h){

    if (obj->isVisible()){

        UIList *listObj = (UIList *)obj;
        int icono = -1;
        //t_color colorText = listObj->isEnabled() && listObj->isFocus() ? cNegro : cGris;
        t_color colorText = listObj->isEnabled() ? listObj->getTextColor() : cGris;
        int cont = 0;


        const int nElems = listObj->getNIconsHoriz();
        const int elemWidth = w / nElems;
        const int elemHeight = h/3;
        SDL_Rect textArea = { 0, 0, elemWidth, elemHeight };
        int centeredY = h / 2 - elemHeight / 2;
        SDL_Rect dstrect = {0,0, elemWidth, elemHeight};
        //SDL_Surface *mySurface = SDL_CreateRGBSurface(SURFACE_MODE, textArea.w, textArea.h, screen->format->BitsPerPixel,0, 0, 0, 0);
        SDL_Surface *mySurface;

        int i=0;
        int relPos = 0;
        int tamLabel = 0;
        string rutaLogo;
        ImagenGestor imgGestor;

        while (i < nElems && listObj->getPosIniLista() + i <= listObj->getPosFinLista()){
            relPos = listObj->getPosIniLista() + i;
            tamLabel = fontStrLen(listObj->getListNames()->get(relPos).c_str());
            rutaLogo = dirInicial + Constant::getFileSep() + "emuImgs" + Constant::getFileSep()
            + "logos" + Constant::getFileSep() + listObj->getListImage()->get(relPos) + ".png";
            Traza::print("rutaLogo: " + rutaLogo, W_PARANOIC);

//            if (relPos == listObj->getPosActualLista()){
//                drawRectAlpha(x + i * elemWidth,
//                              y + centeredY,
//                              elemWidth, elemHeight, cAzul, 128);
//            }
//            else {
//                drawRectAlpha(x + i * elemWidth,
//                              y + centeredY,
//                              elemWidth, elemHeight, cGrisClaro, 128);
//            }

            drawRectAlpha(x + i * elemWidth,
                              y + centeredY,
                              elemWidth, elemHeight, cGrisClaro, 128);

            if (listObj->getListImage()->get(relPos).empty()){
                drawTextInArea(listObj->getListNames()->get(relPos).c_str()
                           , x + ((icono >= 0) ? ICOSPACE : 0) + i * elemWidth + ( elemWidth - tamLabel) / 2,
                             y + centeredY + (elemHeight - fontHeight) / 2, colorText, &textArea);
            } else {

                if (relPos != listObj->getPosActualLista()){
                    textArea.w = elemWidth - elemWidth / 3;
                    textArea.h = elemHeight - elemHeight / 3;
                } else {
                    textArea.w = elemWidth;
                    textArea.h = elemHeight;
                }

                if (imgGestor.loadImgFromFileZoom(rutaLogo.c_str(), &mySurface, &textArea)){
                    dstrect.x = x + i * elemWidth + (elemWidth - mySurface->w) / 2;
                    dstrect.y = y + centeredY + (elemHeight - mySurface->h) / 2;
                    SDL_BlitSurface(mySurface, NULL, screen, &dstrect);
                    SDL_FreeSurface( mySurface );
                    mySurface = NULL;
                } else {
                    drawTextInArea(listObj->getListNames()->get(relPos).c_str()
                           , x + ((icono >= 0) ? ICOSPACE : 0) + i * elemWidth + ( elemWidth - tamLabel) / 2,
                             y + centeredY + (elemHeight - fontHeight) / 2, colorText, &textArea);
                }
            }

            if (relPos != listObj->getPosActualLista()){
                drawRectAlpha(x + i * elemWidth,
                              y + centeredY,
                              elemWidth, elemHeight, cGrisClaro, 128);
            }

            i++;
        }
    }
}

/**
* Metodo comun para pintar el listado y la lista del combo
*/
void Ioutil::drawListContent(Object *obj, int x, int y, int w, int h){

    if (obj->isVisible()){

        UIList *listObj = (UIList *)obj;
        int icono = -1;
        //t_color colorText = listObj->isEnabled() && listObj->isFocus() ? cNegro : cGris;
        t_color colorText = listObj->isEnabled() ? listObj->getTextColor() : cGris;
        int cont = 0;
        int centeredY = 0;

        if (listObj->getObjectType() == GUILISTBOX){
            centeredY = (Constant::getMENUSPACE() - fontHeight) / 2;
        }

        SDL_Rect textArea = { 0, 0, w - INPUTCONTENT, Constant::getMENUSPACE() };

        for (unsigned int i=listObj->getPosIniLista(); i <= listObj->getPosFinLista(); i++ ){
            Traza::print("pintando: " + listObj->getListNames()->get(i), W_PARANOIC);
            icono = listObj->getListIcons()->get(i);

            textArea.w = w - INPUTCONTENT - (icono >= 0 ? ICOSPACE : 0);

            if (i == listObj->getPosActualLista()){
                drawRectAlpha(x - INPUTCONTENT, y + cont*Constant::getMENUSPACE(), listObj->getW(),
                               Constant::getMENUSPACE(), cAzul, listObj->isFocus() ? 255 : 128);
                colorText = cBlanco;
                TTF_SetFontStyle(font, TTF_STYLE_BOLD);
                drawTextInArea(listObj->getListNames()->get(i).c_str() , x + ((icono >= 0) ? ICOSPACE : 0),
                               y + centeredY + cont*Constant::getMENUSPACE(), colorText, &textArea);
                colorText = listObj->isEnabled() ? listObj->getTextColor() : cGris;
                TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
            } else if (i == listObj->getLastSelectedPos() && listObj->isEnableLastSelected()){
                //Dibujamos el rectangulo del ultimo elemento seleccionado
                drawRectAlpha(x - INPUTCONTENT, y + cont*Constant::getMENUSPACE(), listObj->getW(),
                              Constant::getMENUSPACE(), cVerde, 50);
                drawTextInArea(listObj->getListNames()->get(i).c_str() , x + ((icono >= 0) ? ICOSPACE : 0),
                               y + centeredY + cont*Constant::getMENUSPACE(), listObj->isEnabled() ? cBlanco : cGris, &textArea);

            } else {
                drawTextInArea(listObj->getListNames()->get(i).c_str() , x + ((icono >= 0) ? ICOSPACE : 0), y + centeredY + cont*Constant::getMENUSPACE(), colorText, &textArea);
            }

            pintarLinea(x, y + (cont+1)*Constant::getMENUSPACE(), x + w - 2*INPUTCONTENT,  y + (cont+1)*Constant::getMENUSPACE(), cSeparator);

            if (icono >= 0){
                drawIco(icono, x, y + centeredY + cont*Constant::getMENUSPACE());
            }
            cont++;
        }
        drawScrollBar(listObj);
    }
}


/**
*
*/
void Ioutil::drawUIInputWide(Object *obj){
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();

        UIInput *objInput = (UIInput *)obj;

        //Dibujando el inputText
        pintarContenedor(x,y,w,h,obj->isFocus() && obj->isEnabled(), obj, cInputContent);
        //Dibujando el label del input
        drawText(obj->getLabel().c_str(),x,y-FONTSIZE-4, obj->getColor());

        x += INPUTCONTENT;
        y += INPUTCONTENT;

        bool activo = true;
        static long before = SDL_GetTicks();

        if (obj->isFocus()){
            long now = SDL_GetTicks();
            if ((now - before) < 250){
                activo = true;
            } else if ((now - before) < 500 && (now - before) > 250){
                activo = false;
            } else if ((now - before) > 500){
                before = now;
            }
        } else {
            activo = false;
        }

        t_color colorText = obj->isEnabled() ? obj->getTextColor() : cGris;
        tInput myInput = calculaTextoInput(obj);



        if (objInput->getSelectionIni() > -1 && objInput->getSelectionFin() > -1){

            int nSel = abs(objInput->getSelectionFin() - objInput->getSelectionIni());

            int iniSel = objInput->getSelectionIni();
            if (objInput->getSelectionIni() != objInput->getPosChar()){
                iniSel = objInput->getSelectionIni() > 0 ? objInput->getSelectionIni() - 1 : 0;
            }

            string textoSeleccionado = objInput->getText().substr(iniSel, nSel);
            Traza::print("textoSeleccionado: " + textoSeleccionado, W_DEBUG);
            int txtLen = fontStrLen(textoSeleccionado);
            int maxW = txtLen < objInput->getW() - 2*INPUTCONTENT ? txtLen : objInput->getW() - 2*INPUTCONTENT;
            SDL_Rect selTextLocation = {0, 0,w,h};

            if (objInput->getSelectionIni() == objInput->getPosChar()){
                //Seleccionando texto a la izquierda del cursor
                if (maxW + myInput.cursorX > obj->getX() + obj->getW()){
                    maxW = obj->getX() + obj->getW() - myInput.cursorX - INPUTCONTENT;
                }
                drawRect(myInput.cursorX, myInput.cursorY, maxW,FONTSIZE, cAzul);
//                drawTextInArea(textoSeleccionado.c_str(), myInput.cursorX, myInput.cursorY, cBlanco, &selTextLocation);
            } else {
                //Seleccionando texto a la derecha del cursor
                if (objInput->getSelectionFin() > objInput->getSelectionIni()){
                    string charAnt = objInput->getText().substr(objInput->getSelectionIni(), objInput->getSelectionFin() - objInput->getSelectionIni());
                    int posX = myInput.cursorX - fontStrLen(charAnt);

                    if (posX < obj->getX())
                        posX = obj->getX() + INPUTCONTENT;

                    drawRect(posX, myInput.cursorY, maxW,FONTSIZE, cAzul);
//                    drawTextInArea(textoSeleccionado.c_str(), posX, myInput.cursorY, cBlanco, &selTextLocation);
                }
            }
        }

        drawTextInArea(myInput.texto.c_str(), x, y, colorText, NULL);

        if (activo && obj->isEnabled())
            drawText("|", myInput.cursorX, myInput.cursorY + fontDescent, colorText);

    }
}

/**
* El sizeInput es el tamanyo en pixels del input
*/
tInput Ioutil::calculaTextoInput(Object *obj){

    UIInput *objInput = (UIInput *)obj;

    int sizeInput = objInput->getW() - 2*INPUTCONTENT;
    tInput dato;                       //Devuelve el contenido que se mostrara en el input
    string text = objInput->getText();   //Texto completo
    unsigned int totalTexto = text.length();
    unsigned int offset = objInput->getOffset(); //Establece la primera letra que se pinta en el input
    unsigned int tamText = text.length() - offset;   //Establece el tamanyo del texto que se va a pintar
    int textSizeResult = 0;                 //Se usa para calcular el tamanyo del texto que podemos mostrar para que quepa en el input
    unsigned int posFin = 0;                //Establece la ultima letra que estamos pintando en el input

    dato.cursorX = objInput->getX() + INPUTCONTENT;
    dato.cursorY = objInput->getY() + INPUTCONTENT;

    //Si el cursor esta en una letra que no se muestra por pantalla, forzamos el offset
    //a ir a la posicion del cursor. Luego recalcularemos la ultima letra que mostramos
    //por pantalla en el bucle siguiente
    if (objInput->getPosChar() < offset){
        offset = objInput->getPosChar();
    }

    do{
        //Calculamos el resto del texto que cabe en el input a partir de la posicion
        //de la letra que indica la variable offset
        if (offset < totalTexto){
            dato.texto = text.substr(offset, tamText);
            while ((textSizeResult = fontStrLen(dato.texto)) > sizeInput && tamText > 0){
                tamText--;
                dato.texto = text.substr(offset, tamText);
            }
        }
        //Calculamos cual es la posicion de la ultima letra que hemos pintado
        posFin = offset + tamText;
        //En el caso de que la posicion del cursor del input este mas alejado de la
        //ultima letra que pintamos, debemos mostrar una letra mas del principio del texto
        if (objInput->getPosChar() > posFin){
            offset++;
        }
        //Si hemos movido el offset por la condicion anterior, debemos volver a recalcular el final
        //Por eso realizamos un do...while
    }while (objInput->getPosChar() > posFin && offset < totalTexto);

    //Calculamos la posicion del cursor
    if (posFin == objInput->getPosChar()){
        dato.cursorX += textSizeResult;
    } else if (objInput->getPosChar() > offset && objInput->getPosChar() < totalTexto){
        dato.cursorX += fontStrLen(text.substr(offset, objInput->getPosChar() - offset));
    }

    //Guardamos la posicion del offset en el objeto
    objInput->setOffset(offset);
    return dato;
}

/**
 * 
 * @param obj
 * @param evento
 */
void Ioutil::drawUIProgressBar(Object *obj, tEvento *evento){

    UIProgressBar *objProg = (UIProgressBar *)obj;
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();

        pintarContenedor(x,y,w,h,objProg->isFocus() && objProg->isEnabled(), obj, cInputContent);

        if (w > 0 && h > 0){
            
            if (objProg->getProgressMax() > 0){
                int wsel = w;
                if (objProg->getProgressPos() <= objProg->getProgressMax()){
                    wsel = (objProg->getProgressPos() / (float)objProg->getProgressMax()) * w; 
                }
                
                if (wsel > 0)
                    drawRect(x+INPUTBORDER,y+INPUTBORDER,wsel,h-INPUTBORDER,cAzul); // Dibujo el progreso
            }
            

            //Calculamos el hint de la barra cuando pasamos el mouse por encima
            if (evento->mouse_x > 0 && evento->mouse_y > 0){
                //Indicamos que estamos sobre la barra
                objProg->setMouseOverBar(evento->mouse_x > obj->getX() && evento->mouse_x < obj->getX() + obj->getW()
                && evento->mouse_y > obj->getY() && evento->mouse_y < obj->getY() + obj->getH());
                //Especificamos la posicion
                if (objProg->getMouseOverBar()){
                    objProg->setPosXNow(evento->mouse_x);
                    int dif = evento->mouse_x > 0 ? evento->mouse_x - objProg->getX() : 0;
                    float percent = objProg->getW() >= 1 ? dif/(float)objProg->getW() : 0;
                    if (evento->mouse_x == x + w - INPUTBORDER){
                        percent = 1;
                    } else if(evento->mouse_x == x + INPUTBORDER){
                        percent = 0;
                    }


                    long tempPos = ceil(objProg->getProgressMax() * percent);

                    objProg->setLastTimeTick(SDL_GetTicks());
                    if (objProg->getTypeHint() == HINT_TIME){
                        objProg->setLabel(Constant::timeFormat(tempPos));
                    } else if (objProg->getTypeHint() == HINT_PERCENT){
                        objProg->setLabel(Constant::TipoToStr(ceil(percent * 100)) + "%");
                    }
                }
            }

            if (objProg->getMouseOverBar()){
                pintarHint(objProg->getPosXNow(), obj->getY() - fontHeight, fontStrLen(objProg->getLabel()),
                           fontHeight, objProg->getLabel(), cGrisClaro);
            }
        }
    }
}

/**
*
*/
void Ioutil::drawUISlider(Object *obj, tEvento *evento){

    UISlider *objProg = (UISlider *)obj;
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();

        //Se calcula la altura del boton
        int hsel = objProg->getProgressMax() > 0 ? (1.0 - objProg->getProgressPos() / (float)objProg->getProgressMax()) * h : h/2;
        //Se pinta la barra de desplazamiento
        if (!obj->getImgDrawed()){
            Colorutil colorUtil;
            t_color degradedColor;

            int corte = ceil(objProg->getProgressMax()/(float)3);
            int corte2 = corte * 2;

            if (objProg->getProgressPos() < corte ){
                colorUtil.calcDegradation(cAzulTotal, cVerde, corte);
                colorUtil.getDegradedColor(objProg->getProgressPos(), &degradedColor);
            } else if (objProg->getProgressPos() < corte2 ){
                colorUtil.calcDegradation(cVerde, cAmarillo, corte);
                colorUtil.getDegradedColor(objProg->getProgressPos() - corte, &degradedColor);
            } else {
                colorUtil.calcDegradation(cAmarillo, cRojo , corte);
                colorUtil.getDegradedColor(objProg->getProgressPos() - corte2, &degradedColor);
            }

            pintarContenedor(x,y,w,h,objProg->isFocus() && objProg->isEnabled(), obj, objProg->isEnabled() ? degradedColor : cGrisClaro);
            if (w > 0 && h > 0){
                if (hsel > 0){
                    drawRect(x+INPUTBORDER,y+INPUTBORDER,w-INPUTBORDER,hsel,cGris); // Dibujo la parte gris
        }
    }
            Traza::print("drawUISlider", W_DEBUG);
            cachearObjeto(obj);
        } else {
            cachearObjeto(obj);
}
        //Se pinta el icono del desplazador de la barra
        drawIco(btnSliderEQ, x + w/2 - FAMFAMICONW / 2, y+INPUTBORDER + hsel - FAMFAMICONH / 2, 17,17);

        SDL_Rect labelLocation = { x + w/2 - fontStrLen(obj->getLabel()) / 2 + 2,
                                   y + h + 10,
                                   0, 0};

        if (!obj->isOtherDrawed()){
            int tmpFontSize = FONTSIZE;
            loadFont(9);
            drawText(obj->getLabel().c_str(), labelLocation.x, labelLocation.y, obj->getTextColor());
            labelLocation.w = fontStrLen(obj->getLabel());
            labelLocation.h = fontHeight;
            loadFont(tmpFontSize);
            cachearPosicion(obj,&labelLocation);
        } else {
            cachearPosicion(obj,&labelLocation);
        }

        //Calculamos el hint de la barra cuando pasamos el mouse por encima
        if (evento->mouse_x > 0 && evento->mouse_y > 0 && objProg->isShowHint()){
            //Indicamos que estamos sobre la barra
            objProg->setMouseOverBar(evento->mouse_x > obj->getX() && evento->mouse_x < obj->getX() + obj->getW()
            && evento->mouse_y > obj->getY() && evento->mouse_y < obj->getY() + obj->getH());
            //Especificamos la posicion
            if (objProg->getMouseOverBar() ){
                objProg->setPosYNow(evento->mouse_y);
                int dif = evento->mouse_y > 0 ? evento->mouse_y - objProg->getY() : 0;
                float percent = objProg->getH() >= 1 ? dif/(float)objProg->getH() : 0;
                percent = 1.0 - percent;
                long tempPos = ceil(objProg->getProgressMax() * percent);

                objProg->setLastTimeTick(SDL_GetTicks());
                if (objProg->getTypeHint() == HINT_TIME){
                    objProg->setLabel(Constant::timeFormat(tempPos));
                } else if (objProg->getTypeHint() == HINT_PERCENT){
                    objProg->setLabel(Constant::TipoToStr(ceil(percent * 100)) + "%");
                }
            }
        }

        //Si debemos hacerlo, pintamos el hint
        if (objProg->getMouseOverBar() && objProg->isShowHint()){
            pintarHint(x + w + 3, objProg->getPosYNow(), fontStrLen(objProg->getLabel()),
                       fontHeight, objProg->getLabel(), cGrisClaro);
        }
    }
}
/**
*
*/
void Ioutil::drawUILabel(Object *obj){
    if (obj->isVisible()){
        int textx = obj->getX();
        int texty = obj->getY();
        int txtcent = 0;

        if (obj->isCentered()){
            txtcent = fontStrLen(obj->getLabel()) / 2;
        }

        drawText(obj->getLabel().c_str(), textx - txtcent, texty, obj->getTextColor());
    }
}

/**
*
*/
void Ioutil::drawUIButton(Object *obj){
    if (obj->isVisible()){
        SDL_Rect imgLocation = { (short int)obj->getX() , (short int)obj->getY(), (short unsigned int)obj->getW(), (short unsigned int)obj->getH() };

        if (!obj->getImgDrawed() || !obj->isVerContenedor()){
            int textx = obj->getX();
            int texty = obj->getY();
            int txtcentY = (obj->getH() - fontHeight)/2;
            int txtcentX = (obj->getW() - INPUTCONTENT * 2 - fontStrLen(obj->getLabel())) / 2;
            int icocentY = (obj->getH() - FAMFAMICONH) / 2;

            if (txtcentX < 0 || fontStrLen(obj->getLabel()) + txtcentX + 5 >= obj->getW()  ) txtcentX = 0;
            if (txtcentY < 0) txtcentY = 0;
            if (icocentY < 0) icocentY = 0;

            if (obj->isVerContenedor()){
                textx += INPUTCONTENT;
                //Se pinta el fondo del boton. Para los botones es una imagen que hemos guardado en el gestor de iconos
                bool btnDrawed = false;

                if (obj->isEnabled()){
                if (obj->isFocus()){
                        btnDrawed = gestorIconos->drawIcono(boton_selected, screen, 0, obj->getX(), obj->getY(), obj->getW(), obj->getH());
                    } else {
                        btnDrawed = gestorIconos->drawIcono(boton, screen, 0, obj->getX(), obj->getY(), obj->getW(), obj->getH());
                    }
                } else {
                    btnDrawed = gestorIconos->drawIcono(boton_disabled, screen, 0, obj->getX(), obj->getY(), obj->getW(), obj->getH());
                }

                //Si ha habido algun problema pintando el boton, pintamos un fondo que resalte un poco sobre el texto
                //que pintamos a continuacion
                if (!btnDrawed){
                    pintarContenedor(obj->getX(),obj->getY(),obj->getW(),obj->getH(), false, obj, cAzulOscuro);
                }
                //Pintamos el texto del boton
                drawText(obj->getLabel().c_str(), textx + txtcentX, texty + txtcentY, cBlanco);
            }

            //Pintamos el icono del boton
            drawIco(obj->getIcon(), obj->isVerContenedor() ? textx + 3 : textx, texty + icocentY);
            if (!obj->isVerContenedor()){
                //Si solo pintamos el icono, podemos mostrar un texto como hint
                if (obj->isFocus()){
                    int posHintY = texty + txtcentY - fontHeight - 4;
                    int posHintX = textx;

                    if (posHintY < 0){
                        posHintY = obj->getY() + obj->getH() + 4;
                        posHintX += obj->getW();
                    }
                    pintarHint(posHintX, posHintY, fontStrLen(obj->getLabel()), fontHeight, obj->getLabel(), cGrisClaro);
                }
            }


            if (obj->isVerContenedor()){
                    //Reseteamos el surface
                    obj->getImgGestor()->setSurface(NULL);
                    //Guardamos la imagen en el objeto
                    takeScreenShot(&obj->getImgGestor()->surface, imgLocation);
                    //Indicamos que ya hemos pintado la imagen
                    obj->setImgDrawed(true);

            }
        } else {
            printScreenShot(&obj->getImgGestor()->surface, imgLocation);
        }
    }
}

/**
*
*/
void Ioutil::showMessage(string mensaje, unsigned long delay){
    showMessage(mensaje, delay, true, t_posicion(0,0,0,0));
}

void Ioutil::showMessage(string mensaje, unsigned long delay, bool restoreBackground, t_posicion txtDesp){
    const int borde = 20;
    mensaje = Constant::txtDisplay(mensaje);
    SDL_Surface *pointerSurface = NULL;
    if (pointerSurface == NULL && restoreBackground){
        takeScreenShot(&pointerSurface);
    }
    setCursor(cursor_arrow);
    drawRectAlpha(borde, borde, this->w - 2*borde , this->h - 2*borde , cNegro, 200);
    t_color color = {128,128,128};
    drawUITitleBorder("Aviso",borde + 10, color);
    drawTextCent(mensaje.c_str(),txtDesp.x, txtDesp.y, true, true, cBlanco);
    flipScr();
    SDL_Delay(delay);

    if (restoreBackground)
        printScreenShot(&pointerSurface);
}

/**
*
*/
void Ioutil::setAutoMessage(string var){
    if (mensajeAuto == "" ) {
        mensajeAuto = var;
    }
}

/**
*
*/
void Ioutil::showAutoMessage(){
    static int time = 2 * FRAMELIMIT; //2 segundos
    static SDL_Surface *pointerSurface = NULL;

    if(mensajeAuto != "" && time >= 0){
        if (mensajeAuto != ""){
            int pixelDato = 0;
            TTF_SizeText(this->font,mensajeAuto.c_str(),&pixelDato,NULL );

            if (pointerSurface == NULL){
                takeScreenShot(&pointerSurface);
            }

            drawRect( 0, 0, pixelDato, FONTSIZE+2, cNegro);
            drawText(mensajeAuto.c_str(),0,0,cBlanco);
            SDL_UpdateRect(screen, 0, 0, pixelDato, FONTSIZE+2);
        }

        if (time > 0){
            time--;
        } else {
            int pixelDato = 0;
            TTF_SizeText(this->font,mensajeAuto.c_str(),&pixelDato,NULL );

            if (pointerSurface != NULL){
                SDL_Rect textLocation = {0, 0, (short unsigned int)pixelDato, (short unsigned int)(FONTSIZE+2)};
                SDL_BlitSurface(pointerSurface, &textLocation, screen, &textLocation);
                SDL_UpdateRect(screen, 0, 0, pixelDato, FONTSIZE+2);
                SDL_FreeSurface(pointerSurface);
                pointerSurface = NULL;
            }
            time = 2 * FRAMELIMIT; //2 segundos
            mensajeAuto = "";
        }
    }
}


/**
*
*/
void Ioutil::showCheck(Object *obj){

    int x = obj->getX();
    int y = obj->getY();

    pintarContenedor(x,y,CHECKW, CHECKH, obj->isFocus(), obj, cInputContent);
    //Escribir el label del input text
    drawText(obj->getLabel().c_str(),x+5+CHECKW,y,obj->getTextColor());

    if (obj->isChecked())
        drawRect(x+2,y+2,CHECKW-4,CHECKH-4,cAzul); // Dibujo el input como seleccionado o no
}

/**
*
*/
int Ioutil::fontStrLen(string str){
    int pixelSize = 0;
    TTF_SizeText(this->font,str.c_str(),&pixelSize,NULL );
    return pixelSize;
}

/**
*
*/
int Ioutil::fontStrLenCent(string str)
{
    return -1*fontStrLen(str)/2;
}

/**
 * Return the pixel value at (x, y)
 * NOTE: The surface must be locked before calling this!
 */
Uint32 Ioutil::getpixel(SDL_Surface *surface, const int x, const int y)
{
    //int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

/**
* Hace lo mismo que putpixel pero comprobando que no se superen las dimensiones de la pantalla
*/
void Ioutil::putpixelSafe(SDL_Surface *surface, const int x, const int y, const Uint32 pixel){

    if (x < surface->w && y < surface->h && x >= 0 && y >= 0)
        putpixel(surface, x, y, pixel);
//    else
//        Traza::print("Pintando en zona no segura: " + Constant::TipoToStr(x) + "; " + Constant::TipoToStr(y)+ "; w: " +  Constant::TipoToStr(this->w) + "; h: " +  Constant::TipoToStr(this->h), W_DEBUG);

}

void Ioutil::putpixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel)
{
     //Draw_Pixel(surface, x,y, pixel);
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}
///**
// * Set the pixel at (x, y) to the given value
// * NOTE: The surface must be locked before calling this!
// */
//inline void Ioutil::putpixel(SDL_Surface *surface, const int x, const int y, const Uint32 pixel)
//{
//    //int bpp = surface->format->BytesPerPixel;
//    /* Here p is the address to the pixel we want to set */
//    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
//
//    switch(surface->format->BytesPerPixel) {
//    case 1:
//        *p = pixel;
//        break;
//
//    case 2:
//        *(Uint16 *)p = pixel;
//        break;
//
//    case 3:
//        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
//            p[0] = (pixel >> 16) & 0xff;
//            p[1] = (pixel >> 8) & 0xff;
//            p[2] = pixel & 0xff;
//        } else {
//            p[0] = pixel & 0xff;
//            p[1] = (pixel >> 8) & 0xff;
//            p[2] = (pixel >> 16) & 0xff;
//        }
//        break;
//
//    case 4:
//        *(Uint32 *)p = pixel;
//        break;
//    }
//}

/**
*
*/
void Ioutil::pintarCirculo (int n_cx, int n_cy, int r, t_color color)
{
//    int centro_x=0;
//    int centro_y=0;
//
//    int x_izq=x - r;
//    int x_der=x + r;
//    int y_izq=y - r;
//    int y_der=y + r;
//
//    if (x_izq < 0) x_izq = 0;
//    if (x_der > screen->w) x_der = screen->w - 1;
//    if (y_izq < 0) y_izq = 0;
//    if (y_der > screen->h) y_der = screen->h - 1;
//
//    int raiz=0;
//    int radio2 = r*r;
//
//    int i=x_izq;
//    int j=y_izq;
//
//    Uint32 r_color = SDL_MapRGB(screen->format, color.r,color.g,color.b);
//
//    /* Lock the screen for direct access to the pixels */
//    if ( SDL_MUSTLOCK(screen) ) {
//        if ( SDL_LockSurface(screen) < 0 ) {
//            Traza::print("Can't lock screen:" + string(SDL_GetError()), W_ERROR);
//        }
//    }
//
//    while (i < x_der){
//        while (j < y_der){
//            centro_x = i - x;
//            centro_y = j - y;
//            //Funcion del circulo -> x^2 + y^2 = r^2
//            raiz = centro_x*centro_x + centro_y*centro_y;
//            //el -2 es un factor de correccion necesario para circulos pequenyos, sino queda muy pixelado
//            if (raiz < radio2-4){
//                putpixelSafe(screen,i,j,r_color);
//            }
//            j++;
//        }
//        i++;
//        j=y_izq;
//    }
//
//    if ( SDL_MUSTLOCK(screen) ) {
//        SDL_UnlockSurface(screen);
//    }

 // if the first pixel in the screen is represented by (0,0) (which is in sdl)
    // remember that the beginning of the circle is not in the middle of the pixel
    // but to the left-top from it:

    double error = (double)-r;
    double x = (double)r -0.5;
    double y = (double)0.5;
    double cx = n_cx - 0.5;
    double cy = n_cy - 0.5;
    Uint32 r_color = SDL_MapRGB(screen->format, color.r,color.g,color.b);

    while (x >= y)
    {
        putpixelSafe(screen, (int)(cx + x), (int)(cy + y), r_color);
        putpixelSafe(screen, (int)(cx + y), (int)(cy + x), r_color);

        if (x != 0)
        {
            putpixelSafe(screen, (int)(cx - x), (int)(cy + y), r_color);
            putpixelSafe(screen, (int)(cx + y), (int)(cy - x), r_color);
        }

        if (y != 0)
        {
            putpixelSafe(screen, (int)(cx + x), (int)(cy - y), r_color);
            putpixelSafe(screen, (int)(cx - y), (int)(cy + x), r_color);
        }

        if (x != 0 && y != 0)
        {
            putpixelSafe(screen, (int)(cx - x), (int)(cy - y), r_color);
            putpixelSafe(screen, (int)(cx - y), (int)(cy - x), r_color);
        }

        error += y;
        ++y;
        error += y;

        if (error >= 0)
        {
            --x;
            error -= x;
            error -= x;
        }
    }
}

/*
 * SDL_Surface 32-bit circle-fill algorithm without using trig
 *
 * While I humbly call this "Celdecea's Method", odds are that the
 * procedure has already been documented somewhere long ago.  All of
 * the circle-fill examples I came across utilized trig functions or
 * scanning neighbor pixels.  This algorithm identifies the width of
 * a semi-circle at each pixel height and draws a scan-line covering
 * that width.
 *
 * The code is not optimized but very fast, owing to the fact that it
 * alters pixels in the provided surface directly rather than through
 * function calls.
 *
 * WARNING:  This function does not lock surfaces before altering, so
 * use SDL_LockSurface in any release situation.
 */
void Ioutil::pintarFillCircle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel)
{
    // Note that there is more to altering the bitrate of this
    // method than just changing this value.  See how pixels are
    // altered at the following web page for tips:
    //   http://www.libsdl.org/intro.en/usingvideo.html
    static const int BPP = 4;

    double r = (double)radius;

    for (double dy = 1; dy <= r; dy += 1.0)
    {
        // This loop is unrolled a bit, only iterating through half of the
        // height of the circle.  The result is used to draw a scan line and
        // its mirror image below it.

        // The following formula has been simplified from our original.  We
        // are using half of the width of the circle because we are provided
        // with a center and we need left/right coordinates.

        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = cx - dx;

        // Grab a pointer to the left-most pixel for each half of the circle
        Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
        Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;

        for (; x <= cx + dx; x++)
        {
            *(Uint32 *)target_pixel_a = pixel;
            *(Uint32 *)target_pixel_b = pixel;
            target_pixel_a += BPP;
            target_pixel_b += BPP;
        }
    }
}

void Ioutil::pintarSemiCirculo (int x, int y, int r, t_color color, int angle)
{
    int centro_x=0;
    int centro_y=0;

    int x_izq=x - r;
    int x_der=x + r;
    int y_izq=y - r;
    int y_der=y + r;

    if (x_izq < 0) x_izq = 0;
    if (x_der > screen->w) x_der = screen->w - 1;
    if (y_izq < 0) y_izq = 0;
    if (y_der > screen->h) y_der = screen->h - 1;

    int raiz=0;
    int radio2 = r*r;

    int i=x_izq;
    int j=y_izq;

    Uint32 r_color = SDL_MapRGB(screen->format, color.r,color.g,color.b);

    /* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            Traza::print("Can't lock screen:" + string(SDL_GetError()), W_ERROR);
        }
    }

    while (i < x_der){
        while (j < y_der){
            centro_x = i - x;
            centro_y = j - y;
            //Funcion del circulo -> x^2 + y^2 = r^2
            raiz = centro_x*centro_x + centro_y*centro_y;
            //el -2 es un factor de correccion necesario para circulos pequenyos, sino queda muy pixelado
            if (raiz < radio2-4){
                putpixelSafe(screen,i,j,r_color);
            }
            j++;
        }
        i++;
        j=y_izq;
    }

    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }

}

/**
*
*/
void Ioutil::pintarIconoProcesando(bool refreshBackground){
        long delay = 0;
    static unsigned long before = 0;
    static int grados = 0;
    const int iconW = 50;
    const int iconH = 50;
    int x = screen->w/2 - iconW / 2;
    int y = screen->h/2 - iconH / 2;
    SDL_Rect iconRect = {(short int)x, (short int)y, iconW, iconH};
    SDL_Rect iconRectFondo = {(short int)x - iconW / 2, (short int)y - iconH / 2, iconW * 2, iconH * 2};
    static SDL_Surface *mySurface = NULL;

    if (refreshBackground){
        takeScreenShot(&mySurface, iconRectFondo);
        grados = 0;
    }
        int limite = FRAMEPERIOD/25;
            before = SDL_GetTicks();
            printScreenShot(&mySurface, iconRectFondo);
            drawIco(reloj_de_arena, grados, iconRect.x, iconRect.y, iconRect.w, iconRect.h);

    grados += 5;
    t_region region = {iconRectFondo.x, iconRectFondo.y, iconRectFondo.w, iconRectFondo.h};
    updateScr(&region);
            delay = before - SDL_GetTicks() + limite;
            if(delay > 0) SDL_Delay(delay);
        }

/**
*
*/
void Ioutil::fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, t_color color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}

void Ioutil::drawFastHLine(int x, int y, int w, t_color color) {
  // Update in subclasses if desired!
  pintarLinea(x, y, x+w-1, y, color);
}

/**
*
*/
void Ioutil::pintarTriangulo (int x, int y, int base, int altura, bool isUP, t_color color)
{
    //x e y son la cima del triangulo
    //      (x,y)
    //       /\    -
    //      /  \   |  altura
    //      ----   -
    //      base
    //      |--|

    // a^2 = b^2+c^2
    //
    // Parte derecha => Punto central Base = (x, y - altura)
    //                  vertice derecho = (x + base/2, y - altura)

//    int x_arriba=x - base/2;
    int x_der=x + base/2;
    int y_arriba=y;
    int y_debajo=y + altura;

    int numerador = (y_arriba - y_debajo);
    int denominador = (x - x_der);

    float pendiente = numerador/(float)denominador;

    //Funcion de la recta -> j = (y1-y2)/(x1-x2) * (i-x1) + y1
    int calc_j = 0;
    int baseNuevoPlano = y + altura;

    Uint32 r_color = SDL_MapRGB(screen->format, color.r,color.g,color.b);

	/* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            Traza::print("Can't lock screen:" + string(SDL_GetError()), W_ERROR);
        }
    }

    for (int j = y_arriba; j < y_debajo; j++)
    {
        for (int i = x; i < x_der; i++)
        {
            calc_j = (int)(pendiente * (i-x)) + y;

            if (j > calc_j)
            {
                if (!isUP)
                {
                    putpixelSafe(screen,i,2*baseNuevoPlano - j - 2*altura,r_color);
                    putpixelSafe(screen,(2*x - i),2*baseNuevoPlano - j - 2*altura,r_color);
                }
                else
                {
                    putpixelSafe(screen,i,j,r_color);
                    putpixelSafe(screen,(2*x - i),j,r_color);
                }
            }
        }
    }

   	if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }

}

/**
*
*/
void Ioutil::msg_processing(unsigned int posString, unsigned int tam)
{
   if (posString <= tam && tam > 0)
   {
        const char *msg={"Espere un momento porfavor"};
        int sizeMsg = fontStrLen(msg);
        int sizePercent = fontStrLen("100%");

        int pos_v = (this->getHeight() - (2*Constant::getMENUSPACE()))/2;
        //const int radio = 10;
        const int radio = FONTSIZE/2;
        const int posX = 40;
        int ancho = this->getWidth() - 2*posX -radio;
        const int posY = pos_v + FONTSIZE + Constant::getMENUSPACE() + radio;
        char datosProgreso[30];
        SDL_Rect rectTitulo = { (short int)(posX + radio/2-1),(short int)(posY - radio+1), (short unsigned int)ancho, (short unsigned int)(radio*2-1)};

        clearScr(cBgScreen);
        drawText(msg,(this->getWidth() - sizeMsg)/2,pos_v,cBlanco);

        pintarCirculo(posX, posY , radio, cBlanco);
        pintarCirculo(posX + ancho, posY , radio, cBlanco);
        drawRect(rectTitulo.x,rectTitulo.y,rectTitulo.w,rectTitulo.h,cBlanco);
        ancho = ancho * (int)(posString/(float)tam);

        if (ancho > 0){
            pintarCirculo(posX, posY , radio-1, cAzul);
            pintarCirculo(posX + ancho, posY , radio-1,cAzul);
            SDL_Rect rectTitulo2 = { (short int)(posX + radio/2-1),(short int)(posY - radio+1+1), (short unsigned int)ancho, (short unsigned int)((radio-1)*2-1)};
            drawRect(rectTitulo2.x,rectTitulo2.y,rectTitulo2.w,rectTitulo2.h,cAzul);
        }

        sprintf(datosProgreso,"%2.0f",posString/(float)tam*100);
        strcat(datosProgreso,"%");
        t_color colorTemp;

        if (posString > tam/2){
            colorTemp = cBlanco;
        } else {
            colorTemp = cNegro;
        }

        drawText(datosProgreso,(this->getWidth() - sizePercent)/2,pos_v + FONTSIZE + Constant::getMENUSPACE(),colorTemp);
        flipScr();
   }
}

/**
*
*/
string Ioutil::configButtonsJOY(tEvento *evento){
    Traza::print("casoJOYBUTTONS: Inicio", W_PARANOIC);
    bool salir = false;
    string salida = "";

    long delay = 0;
    unsigned long before = 0;
    const char* JoystickButtonsMSG[] = {"Arriba","Abajo","Izquierda","Derecha","Aceptar","Cancelar", "Pagina anterior", "Pagina siguiente", "Select", "Buscar elemento"};
    int JoyButtonsVal[] = {JOY_BUTTON_UP, JOY_BUTTON_DOWN, JOY_BUTTON_LEFT, JOY_BUTTON_RIGHT, JOY_BUTTON_A, JOY_BUTTON_B, JOY_BUTTON_L, JOY_BUTTON_R, JOY_BUTTON_SELECT, JOY_BUTTON_R3};
    //Posiciones de los botones calculadas en porcentaje respecto al alto y ancho de la imagen
    t_posicion_precise imgButtonsRelScreen[] = {{0.3512,0.682,0,0},{0.3512,0.84,0,0},{0.295,0.76,0,0},{0.4075,0.76,0,0},
            {0.79375,0.616,0,0},{0.87625,0.496,0,0},{0.2225,0.194,0,0},{0.7775,0.194,0,0},{0.39375,0.512,0,0},{0.60875,0.512,0,0}};

    int tam = 10;
    int i=0;
    UIPicture obj;

    obj.setX(0);
    obj.setY(0);
    obj.setW(this->getWidth());
    obj.setH(this->getHeight());
    obj.loadImgFromFile(Constant::getAppDir() +  Constant::getFileSep() + "imgs" + Constant::getFileSep() + "xbox_360_controller-small.png");
    //Para que se guarde la relacion de aspecto
    obj.getImgGestor()->setBestfit(false);
    //Para redimensionar la imagen al contenido
    obj.getImgGestor()->setResize(true);

    do{
        SDL_Event event;
        before = SDL_GetTicks();

        if (!obj.getImgDrawed()){
            //Limpiamos la pantalla
            clearScr(cBgScreen);
            //Dibujamos la imagen
            drawImgObj(&obj);
            //Obtenemos las variables que indican la posicion de la imagen una vez que
            //ha sido pintada por pantalla
            int imgX = obj.getImgGestor()->getImgLocationRelScreen().x;
            int imgY = obj.getImgGestor()->getImgLocationRelScreen().y;
            int imgW = obj.getImgGestor()->getImgLocationRelScreen().w;
            int imgh = obj.getImgGestor()->getImgLocationRelScreen().h;
            double relacionAncho =  obj.getImgGestor()->getImgOrigWidth() > 1 ? this->getWidth() / (double) obj.getImgGestor()->getImgOrigWidth()  : 0.2;
            double relacionAlto =  obj.getImgGestor()->getImgOrigHeight() > 1 ? this->getHeight() / (double) obj.getImgGestor()->getImgOrigHeight()  : 0.2;
            //Marcamos la posicion del boton que hay que pulsar
            pintarCirculo(imgW * imgButtonsRelScreen[i].x + imgX, imgh * imgButtonsRelScreen[i].y + imgY, 40 * (relacionAncho < relacionAlto ? relacionAncho : relacionAlto), cRojo);
//            pintarFillCircle(screen,
//                             imgW * imgButtonsRelScreen[i].x + imgX,
//                             imgh * imgButtonsRelScreen[i].y + imgY,
//                             40 * relacionAncho,
//                             SDL_MapRGB(screen->format, 255,0,0));

            //Dibujamos el texto de la accion
            drawTextCent(JoystickButtonsMSG[i], 0, 20, true, false, cBlanco);
            cachearObjeto(&obj);
        } else {
            cachearObjeto(&obj);
        }

        //Mostramos todo por pantalla
        flipScr();
        if( SDL_PollEvent( &event ) ){
             switch( event.type ){
                case SDL_QUIT:
                    salir = true;
                    break;
                case SDL_VIDEORESIZE:
                    screen = SDL_SetVideoMode (event.resize.w, event.resize.h, SCREEN_BITS, SCREEN_MODE);
                    this->w = event.resize.w;
                    this->h = event.resize.h;
                    evento->resize = true;
                    obj.setW(this->getWidth());
                    obj.setH(this->getHeight());
                    obj.setImgDrawed(false);
                    break;
                case SDL_KEYDOWN: // PC buttons
                    if (event.key.keysym.sym == SDLK_ESCAPE){
                        salir = true;
                    }
                    break;

                case SDL_JOYBUTTONDOWN :
                    JoyMapper::setJoyMapper(JoyButtonsVal[i], event.jbutton.button);
                    i++;
                    obj.setImgDrawed(false);
                    break;
                case SDL_JOYHATMOTION:
                    if (event.jhat.value != 0){ //Solo en el momento del joydown
                        JoyMapper::setJoyMapper(JoyButtonsVal[i], JOYHATOFFSET + event.jhat.value);
                        i++;
                        obj.setImgDrawed(false);
                    }
                    break;
                case SDL_JOYAXISMOTION:
                    int normValue;
                    if((abs(event.jaxis.value) > DEADZONE) != (abs(mPrevAxisValues[event.jaxis.which][event.jaxis.axis]) > DEADZONE)){
                        if(abs(event.jaxis.value) <= DEADZONE){
                            normValue = 0;
                        } else {
                            if(event.jaxis.value > 0)
                                normValue = 1;
                            else
                                normValue = -1;
                        }
                        if (normValue != 0){
                            int valor = (abs(normValue) << 4 | event.jaxis.axis) * normValue;
                            JoyMapper::setJoyMapper(JoyButtonsVal[i], JOYAXISOFFSET + valor);
                            i++;
                            obj.setImgDrawed(false);
                        }
                    }
                    mPrevAxisValues[event.jaxis.which][event.jaxis.axis] = event.jaxis.value;
                    break;
             }
        }

        if (i == tam){
            salir = true;
        }

        delay = before - SDL_GetTicks() + TIMETOLIMITFRAME;
        if(delay > 0) SDL_Delay(delay);
    } while (!salir);

    JoyMapper::saveJoyConfig();
    return salida;
}

/**
*
*/
void Ioutil::pintarCursor(int x, int y, int cursor){
    //Si el cursor esta oculto, nadie podra volverlo a habilitar salvo la funcion WaitForKey
    if (actualCursor != cursor_hidden && actualCursor != cursor){
        setCursor(cursor);
    }
}

/**
* Dibuja varias lineas horizontales a partir de una posicion degrandando el gris progresivamente desde
* el valor grayIni al valor grayFin
*/
void Ioutil::pintarDegradado(int x1, int y1, int x2, int y2, int lineas, int grayIni, int grayFin){
    int inc = (grayFin - grayIni) / (lineas - 1);
    t_color tempColor;

    for (int i=0; i < lineas; i++){
        tempColor.r = grayIni + i*inc;
        tempColor.g = tempColor.r;
        tempColor.b = tempColor.r;
        pintarLinea(x1, y1 + i, x2, y2 + i, tempColor);
    }
}
/**
* Dibuja varias lineas horizontales a partir de una posicion degrandando el gris progresivamente desde
* el valor grayIni al valor grayFin
*/
void Ioutil::pintarDegradadoDesplazado(int x1, int y1, int x2, int y2, int lineas, int desplazamiento, int grayIni, int grayFin){
    int inc = (grayFin - grayIni) / (lineas - 1);
    t_color tempColor;

    for (int i=0; i < lineas; i++){
        tempColor.r = grayIni + i*inc;
        tempColor.g = tempColor.r;
        tempColor.b = tempColor.r;
        pintarLinea(x1 + i*desplazamiento, y1 + i *desplazamiento, x2 + i*desplazamiento, y2 + i *desplazamiento, tempColor);
    }
}

/**
*
*/
void Ioutil::drawScrollBar(Object *obj){
    //Tamanyo por defecto minimo de la barra de desplazamiento
    int scrollbarHeight = MINSCROLLBARHEIGHT;
    //Variable para almacenar el incremento de la posicion de la barra de desplazamiento
    //segun el tamanyo de la lista
    int posRelativa = 0;
    bool allElementsVisible = false;


    int minY = obj->getY() + INPUTCONTENT + TRISCROLLBARTAM;
    int maxY = obj->getY() + obj->getH() - INPUTCONTENT - TRISCROLLBARTAM;
    bool drawScroll = false;

    if (obj->getObjectType() == GUILISTBOX || obj->getObjectType() == GUILISTGROUPBOX || obj->getObjectType() == GUITREELISTBOX){
        UIListCommon *objList = (UIListCommon *)obj;
        int cabecera = (obj->getObjectType() == GUILISTGROUPBOX ? Constant::getMENUSPACE() : 0); 
        
        minY += cabecera;
        if (objList->getSize() > objList->getElemVisibles()){
            //Cuantos mas elementos haya en la lista, mas pequenya sera la barra de desplazamiento
            scrollbarHeight = (maxY - minY) * objList->getElemVisibles() / (float) objList->getSize();
        } else {
            //Si no hay suficientes elementos en la lista para que se supere el maximo que se
            //puede mostrar por pantalla, se pinta la barra entera
            scrollbarHeight = (maxY - minY);
            allElementsVisible = true;
        }
        //Comprobamos que la barra tenga un tamanyo minimo
        scrollbarHeight = scrollbarHeight < MINSCROLLBARHEIGHT ? MINSCROLLBARHEIGHT : scrollbarHeight;

        if (objList->getSize() > objList->getElemVisibles()){
            //Calculamos la posicion Y que se tendra que agregar a la minima posicion Y desde la que
            //partira la barra
            posRelativa = (maxY - minY - scrollbarHeight) * objList->getPosActualLista() / (float) objList->getSize();
        }
        drawScroll = objList->isShowScrollbar() && ((allElementsVisible && objList->isShowScrollbarAlways()) || !allElementsVisible) ;
        
        
        //Se pinta el triangulo superior
        if (objList->getPosIniLista() > 0){
            pintarTriangulo (obj->getX() + obj->getW() - 6, obj->getY() + INPUTCONTENT + cabecera, TRISCROLLBARTAM, TRISCROLLBARTAM, true, obj->getTextColor());
        }
        //Se pinta el triangulo inferior
        if (objList->getPosFinLista() + 1 < (unsigned int)objList->getSize()){
            pintarTriangulo (obj->getX() + obj->getW() - 6, obj->getY() + obj->getH() - INPUTCONTENT, TRISCROLLBARTAM, TRISCROLLBARTAM, false, obj->getTextColor());
        }

    } else if(obj->getObjectType() == GUITEXTELEMENTSAREA) {
        UITextElementsArea *objText = (UITextElementsArea *)obj;

        if ( objText->getMaxOffsetY() > objText->getH()){
            scrollbarHeight = (maxY - minY) * objText->getH() / (float) objText->getMaxOffsetY();
        } else {
            scrollbarHeight = (maxY - minY);
            allElementsVisible = true;
        }

        posRelativa = -1 * (maxY - minY - scrollbarHeight) * objText->getOffsetDesplazamiento() / (float) objText->getMaxOffsetY();
        drawScroll = true;

        //Se pinta el triangulo superior
        if (abs(objText->getOffsetDesplazamiento()) > 0){
            pintarTriangulo (obj->getX() + obj->getW() - 6, obj->getY() + INPUTCONTENT
                             , TRISCROLLBARTAM, TRISCROLLBARTAM, true, obj->getTextColor());
        }
        //Se pinta el triangulo inferior
        if (abs(objText->getOffsetDesplazamiento()) < abs(objText->getMaxOffsetY())){
            pintarTriangulo (obj->getX() + obj->getW() - 6, obj->getY() + obj->getH()
                             , TRISCROLLBARTAM, TRISCROLLBARTAM, false, obj->getTextColor());
        }
    }

    if (drawScroll){
        //Dibujamos la barra de scroll
        drawRectLine(obj->getX() + obj->getW() - SCROLLBARWIDTH - INPUTCONTENT + 1,
             minY + posRelativa,
             SCROLLBARWIDTH, scrollbarHeight,
             1, obj->getTextColor());
    }




}

/**
*
*/
void Ioutil::drawUITextElementsArea(Object *obj){

    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();

        if (!obj->getImgDrawed()){

            UITextElementsArea *objTextElement = (UITextElementsArea *)obj;
            SDL_Rect areaLocation = { (short int)x, (short int)y + objTextElement->getOffsetDesplazamiento(), (short int)w, (short int)h };

            //drawRectLine(x, y, w, h, 1, cVerde);
            if (obj->isVerContenedor()){
                pintarContenedor(x,y,w,h,obj->isFocus() && obj->isEnabled(), obj, obj->getColor());
            }

            if (obj->getObjectType() == GUITEXTELEMENTSAREA){
                UITextElementsArea *objText = (UITextElementsArea *)obj;
                int len = objText->getTextVector()->size();
                TextElement *elem;
                int maxPxLabel = 0;
                int tmpPX = 0;

//                if (obj->isVerContenedor()){
//                    pintarDegradado(x + 1,
//                                y,
//                                x + w,
//                                y,
//                                Constant::getMENUSPACE() + INPUTCONTENT , 180, 255);
//                    pintarDegradado(x + 1,
//                                y,
//                                x + w,
//                                y,
//                                5, 128, 180);
//                }

                //Calculamos el elemento con el label mas grande
                for (int i=0; i < len; i++){
                    tmpPX = fontStrLen(objText->getTextVector()->at(i)->getLabel());
                    if (tmpPX > maxPxLabel)
                        maxPxLabel = tmpPX;
                }

                for (int i=0; i < len; i++){
                    elem = objText->getTextVector()->at(i);
                    //Se dibuja el label
                    drawText(elem->getLabel().c_str(),
                             x + elem->getPos()->x,
                             y + elem->getPos()->y + objTextElement->getOffsetDesplazamiento(), obj->getTextColor());

                    int tmpFontSize = FONTSIZE;
                    loadFont(elem->getStyle()->fontSize);

                    int fontStyle = TTF_GetFontStyle(font);
                    if (elem->getStyle()->bold){
                        if (!(fontStyle & TTF_STYLE_BOLD)) {
                            TTF_SetFontStyle(font, fontStyle | TTF_STYLE_BOLD);
                        }
                    }

                    //Se dibuja el texto
                    drawTextInsideArea(i,
                             elem->getPos()->x + (elem->isUseMaxLabelMargin() ?  maxPxLabel : 0) + TEXLABELTEXTSPACE,
                             elem->getPos()->y, obj, &areaLocation);

                    loadFont(tmpFontSize);
                    if (elem->getStyle()->bold){
                        TTF_SetFontStyle(font, fontStyle);
                    }

                }
            }
            cachearObjeto(obj);
        } else {
            cachearObjeto(obj);
        }

    }
}

/**
*
*/
void Ioutil::drawTextInsideArea( int posArrayTexto, int x, int y, Object *obj, SDL_Rect *textLocation){
    if (font != NULL){

        UITextElementsArea *objText = (UITextElementsArea *)obj;
        
        TextElement *txtElem = objText->getTextVector()->at(posArrayTexto);
        string dato = txtElem->getText();

        SDL_Color foregroundColor = { (unsigned char)obj->getTextColor().r,
        (unsigned char)obj->getTextColor().g, (unsigned char)obj->getTextColor().b };

        dato = Constant::replaceAll(dato, "\n", " \n ");
        vector<string> vtexto = Constant::split(dato, " ");
        string tmpStr = "";
        short int acumLinePx = 0;
        short int tamPalabra = 0;
        const short int maxLineaPx = textLocation->w - ( x );
        short int offsetY = 0;
        bool retorno = false;
        int objPosY = obj->getY();

        
        for (int i = 0; i < vtexto.size(); i++){
            tmpStr = vtexto.at(i);
            retorno = tmpStr.compare("\n") == 0;
            tamPalabra = fontStrLen(tmpStr + " ");
                       
            if (acumLinePx + tamPalabra >= maxLineaPx || retorno){
                offsetY += Constant::getMENUSPACE();
                acumLinePx = 0;
            }

            if (!retorno){
                SDL_Rect screenLocation = { (short int)textLocation->x + x + acumLinePx,
                                            (short int)textLocation->y + y + offsetY, 0, 0 };

                if (screenLocation.y < obj->getY() + obj->getH() - Constant::getMENUSPACE()
                    && screenLocation.y >= obj->getY() ){
                    
                    SDL_Surface* textSurface =  TTF_RenderText_Blended(font, tmpStr.c_str(), foregroundColor);
                    SDL_BlitSurface(textSurface, NULL, screen, &screenLocation);
                    SDL_FreeSurface(textSurface);
                    
                    if (txtElem->getIco() >= 0 && i == vtexto.size() -1)
                        drawIco(txtElem->getIco(),screenLocation.x + tamPalabra, screenLocation.y);
                    
                    if (objText->getSelectedPos() == posArrayTexto && !txtElem->getUrl().empty()){
                        pintarLinea(screenLocation.x,
                                    screenLocation.y + txtElem->getStyle()->fontSize,
                                    screenLocation.x + tamPalabra,
                                    screenLocation.y + txtElem->getStyle()->fontSize,
                                    obj->getTextColor());
                    }
                }
            }

            if (acumLinePx + tamPalabra < maxLineaPx && !retorno){
                acumLinePx += tamPalabra;
            }
    }

        objText->setMaxOffsetY(offsetY);
        //drawScrollBar(obj);

    } else {
        Traza::print("Fallo en drawTextInArea: La fuente es NULL", W_ERROR);
    }
}


/**
* Pinta una lista
*/
void Ioutil::drawUITreeListBox(Object *obj){
    if (obj->isVisible()){
        int x = obj->getX();
        int y = obj->getY();
        int w = obj->getW();
        int h = obj->getH();
        UITreeListBox *listObj = (UITreeListBox *)obj;

        
        int centeredY = (Constant::getMENUSPACE() - fontHeight) / 2;
        Traza::print("Repintando lista: " + listObj->getLabel(), W_PARANOIC);
        Traza::print("Alto de la lista: ", h, W_PARANOIC);

        pintarContenedor(x,y,w,h,listObj->isFocus() && listObj->isEnabled(), obj, obj->getColor());
        x += INPUTCONTENT;
        y += INPUTCONTENT;

        t_color colorText = listObj->isEnabled() && listObj->isFocus() ? obj->getTextColor() : cGris;
        if (listObj->getObjectType() == GUITREELISTBOX && listObj->getSize() > 0){
            //Ponemos el color por defecto para las listas
            colorText = listObj->isEnabled() && listObj->isFocus() ? obj->getTextColor() : cGris;
            TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
            Traza::print("listObj->getPosIniLista(): ", listObj->getPosIniLista(), W_PARANOIC);
            Traza::print("listObj->getPosFinLista(): ", listObj->getPosFinLista(), W_PARANOIC);
            //Dibujamos el resto de la lista
            //if (listObj->getListScheme() == SCHEMEICONS){
                //drawListIcoHor(obj, x, y, w, h);
            //} else {
                drawTreeListContent(obj, x, y, w, h);
            //}
        }
           
    }
}

void Ioutil::drawTreeListContent(Object *obj, int x, int y, int w, int h){
    if (obj->isVisible()){
        
        UITreeListBox *listObj = (UITreeListBox *)obj;
        int icono = -1;
        //t_color colorText = listObj->isEnabled() && listObj->isFocus() ? cNegro : cGris;
        t_color colorText = listObj->isEnabled() ? listObj->getTextColor() : cGris;
        int cont = 0;
        int centeredY = 0;
        
        centeredY = (Constant::getMENUSPACE() - fontHeight) / 2;
        SDL_Rect textArea = { 0, 0, w - INPUTCONTENT, Constant::getMENUSPACE() };
        
        if (!listObj->getImgDrawed()){
            int lastNode = -1;
            for (unsigned int i=listObj->getPosIniLista(); i <= listObj->getPosFinLista(); i++ ){
                TreeNode node = listObj->get(i);
                if (lastNode != node.realPos){
                    lastNode = node.realPos; //workaround to avoid repetitions
                    Traza::print("pintando: " + node.text 
                                    + " realpos: " + Constant::TipoToStr(node.realPos)
                                    + " i: " + Constant::TipoToStr(i)
                                    + " FinLista: " + Constant::TipoToStr(listObj->getPosFinLista())
                                , W_PARANOIC);
                    icono = node.ico;
                    textArea.w = w - INPUTCONTENT - (icono >= 0 ? ICOSPACE : 0);

                    if (i == listObj->getPosActualLista()){
                        drawRectAlpha(x - INPUTCONTENT, y + cont*Constant::getMENUSPACE(), listObj->getW(),
                                       Constant::getMENUSPACE(), cAzul, listObj->isFocus() ? 255 : 128);
                        colorText = cBlanco;
                        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
                        drawTextInArea(node.text.c_str() , x + ((icono >= 0) ? ICOSPACE : 0),
                                       y + centeredY + cont*Constant::getMENUSPACE(), colorText, &textArea);
                        colorText = listObj->isEnabled() ? listObj->getTextColor() : cGris;
                        TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
                    } else if (i == listObj->getLastSelectedPos() && listObj->isEnableLastSelected()){
                        //Dibujamos el rectangulo del ultimo elemento seleccionado
                        drawRectAlpha(x - INPUTCONTENT, y + cont*Constant::getMENUSPACE(), listObj->getW(),
                                      Constant::getMENUSPACE(), cVerde, 50);
                        drawTextInArea(node.text.c_str() , x + ((icono >= 0) ? ICOSPACE : 0),
                                       y + centeredY + cont*Constant::getMENUSPACE(), listObj->isEnabled() ? cBlanco : cGris, &textArea);

                    } else {
                        drawTextInArea(node.text.c_str() , x + ((icono >= 0) ? ICOSPACE : 0), y + centeredY + cont*Constant::getMENUSPACE(), colorText, &textArea);
                    }

                    pintarLinea(x, y + (cont+1)*Constant::getMENUSPACE(), x + w - 2*INPUTCONTENT,  y + (cont+1)*Constant::getMENUSPACE(), cSeparator);

                    if (icono >= 0){
                        drawIco(icono, x, y + centeredY + cont*Constant::getMENUSPACE());
                    }
                    cont++;
                }
            }
            drawScrollBar(listObj);
            cachearObjeto(obj);
        } else {
            Traza::print("Alto de la lista: ", h, W_PARANOIC);
            cachearObjeto(obj);
        }
    }    
}
