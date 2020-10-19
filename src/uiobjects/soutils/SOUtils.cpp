/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SOUtils.cpp
 * Author: Ryuk
 * 
 * Created on 23 de marzo de 2020, 18:34
 */

#include "SOUtils.h"

SOUtils::SOUtils() {
}

SOUtils::~SOUtils() {
}

/**
 * 
 * @param argv
 */
string SOUtils::iniciarSistema(char** argv){
    //cout << "Iniciando descarga" << endl;
    Dirutil dir;

    //#ifdef WIN
        string appDir = argv[0];
        int pos = appDir.rfind(Constant::getFileSep());
        if (pos == string::npos){
            FILE_SEPARATOR = FILE_SEPARATOR_UNIX;
            pos = appDir.rfind(FILE_SEPARATOR);
            tempFileSep[0] = FILE_SEPARATOR;
        }
        appDir = appDir.substr(0, pos);
        if (appDir[appDir.length()-1] == '.'){
            appDir.substr(0, appDir.rfind(Constant::getFileSep()));
        }
        
        if (!dir.existe(appDir) || pos == string::npos){
            //cerr << "El directorio: " << appDir << " no existe" << endl;
            appDir = dir.getDirActual();
        } 
        //else {
        //    cout << "El directorio: " << appDir << " existe" << endl;
        //}
        Constant::setAppDir(appDir);
    //#endif // WIN

//    #ifdef UNIX
//        Dirutil dir;
//        Constant::setAppDir(dir.getDirActual());
//    #endif // UNIX
    
    string rutaTraza = appDir + Constant::getFileSep() + "Traza.txt";
    //cout << "rutaTraza: " << rutaTraza << endl;
    Traza *traza = new Traza(rutaTraza.c_str());
    loadConfig();
    return appDir;
}

/**
 * 
 * @return 
 */
bool SOUtils::loadConfig(){
    try{
        string configIniFile = Constant::getAppDir() + Constant::getFileSep() + "config.ini";
        ListaIni<Data> *config = new ListaIni<Data>();
        Traza::print("Cargando configuracion", W_DEBUG);
        config->loadFromFile(configIniFile);
        config->sort();
        
        int trazaLevel = Constant::strToTipo<int>(config->get(config->find("loglevel")).getValue());
        if ( trazaLevel > W_PARANOIC || trazaLevel < 0){
            trazaLevel = W_DEBUG;
        }
        
        Constant::setTrazaLevel(trazaLevel);
        string pip = config->find("proxyip") >= 0 ? config->get(config->find("proxyip")).getValue() : "";
        string ppt = config->find("proxyport") >= 0 ? config->get(config->find("proxyport")).getValue() : "";
        string pu = config->find("proxyuser") >= 0 ? config->get(config->find("proxyuser")).getValue() : "";
        string pp = config->find("proxypass") >= 0 ? config->get(config->find("proxypass")).getValue() : "";
        Constant::setPROXYIP(pip);
        Constant::setPROXYPORT(ppt);
        Constant::setPROXYUSER(pu);
        Constant::setPROXYPASS(pp);
        delete config;
    } catch (Excepcion &e){
        Traza::print("Error al cargar la configuracion", W_ERROR);
        return false;
    }
    return true;
}

/**
 * 
 * @return 
 */
std::string SOUtils::GetClipboardText(){
  std::string text = "";
  #ifdef WIN
  // Try opening the clipboard
  if (! OpenClipboard(NULL)){
    return "";
  }

  // Get handle of clipboard object for ANSI text
  HANDLE hData = GetClipboardData(CF_TEXT);
  if (hData == NULL)
   return "";

  // Lock the handle to get the actual text pointer
  char * pszText = static_cast<char*>( GlobalLock(hData) );
  if (pszText == NULL)
    return "";

  // Save text in a string class instance
  text = pszText;

  // Release the lock
  GlobalUnlock( hData );

  // Release the clipboard
  CloseClipboard();
#endif

#ifdef UNIX
  char *result;
  unsigned long ressize, restail;
  const char bufname[] = "CLIPBOARD";
  const char fmtname[] = "UTF8_STRING"; //"STRING"
  int resbits;
  Display *display = XOpenDisplay(NULL);
  unsigned long color = BlackPixel(display, DefaultScreen(display));
  Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);
  
  Atom bufid = XInternAtom(display, bufname, False),
       fmtid = XInternAtom(display, fmtname, False),
       propid = XInternAtom(display, "XSEL_DATA", False),
       incrid = XInternAtom(display, "INCR", False);
  XEvent event;

  XConvertSelection(display, bufid, fmtid, propid, window, CurrentTime);
  do {
    XNextEvent(display, &event);
  } while (event.type != SelectionNotify || event.xselection.selection != bufid);

  if (event.xselection.property)
  {
    XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, False, AnyPropertyType,
      &fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);

    if (fmtid == incrid)
      printf("Buffer is too large and INCR reading is not implemented yet.\n");
    else
      printf("%.*s", (int)ressize, result);
    text.assign(result);
    XFree(result);
  }
  else // request failed, e.g. owner can't convert to the target format
    printf("Request failed.\n");
  
  XDestroyWindow(display, window);
  XCloseDisplay(display);

#endif  
  return text;
}

/**
*
*/
void SOUtils::waitms(unsigned long ms){
    #ifdef WIN
        Sleep(ms);
    #elif UNIX
        sleep(ms);
    #endif
}