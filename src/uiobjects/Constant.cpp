#include "Constant.h"

#ifdef UNIX
    #include <X11/Xlib.h>
#endif

string Constant::appDir;
int Constant::WINDOW_WIDTH = 640;
int Constant::WINDOW_HEIGHT = 480;
int Constant::TRAZA_LEVEL = W_DEBUG;
int Constant::MENUSPACE = 30 * zoomText; //Separacion de cada elemento del menu
int Constant::INPUTH = 30 * zoomText; //Alto del input
int Constant::EXEC_METHOD = launch_system;
string Constant::PROXYIP = "";
string Constant::PROXYPORT = "";
string Constant::PROXYUSER = "";
string Constant::PROXYPASS = "";
int Constant::MAXTHREADSDOWN = 1;
bool Constant::UPDATE_MISSING = true;
int Constant::COMBOLISTHEIGHT = (Constant::INPUTH + INPUTCONTENT) * 5;
int Constant::IMGBOXARTWIDTH = 250; //Ancho de la imagen de la rom
int Constant::IMGBOXARTHEIGHT = 250; //Alto de la imagen de la rom
uint8_t Constant::c1;  // Last character buffer
size_t Constant::CURL_DOWNLOAD_LIMIT = 0;


static const char* DIAS[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static const char* MESES[] = {"Jan" , "Feb" , "Mar" , "Apr" , "May" , "Jun" , "Jul" , "Aug" , "Sep" , "Oct" , "Nov" , "Dec" };
static const char* METACHARS[] = {" ", "*", "?", "[", "]", "'","\"" ,"$", ";", "&", "(", ")", "|", "^", "<", ">"};
static const char* SPECIALCHARS[] = {/*",","-",*/".","/","_","�","!","\"","#","$","%","&","�","(",")","*","+",":",";","<","=",">","�","?","@","[","\\","]","^","`","{","|","}","~","'"};
static const char* ALLSPECIALCHARS[] = {",","-",".","/","_","�","!","\"","#","$","%","&","�","(",")","*","+",":",";","<","=",">","�","?","@","[","\\","]","^","`","{","|","}","~","'"};

/**
*
*/
string Constant::getAppDir(){
//            if (appDir.empty()){
//                char *buffer = new char[FILENAME_MAX];
//                memset(buffer, '\0',FILENAME_MAX-1);
//                getcwd(buffer, PATH_MAX);
//                appDir = buffer;
//            }
    return appDir;
}

/**
*
*/
void Constant::setAppDir(string var){
    appDir = var;
}

/**
*
*/
void Constant::lowerCase(string *var){
    std::transform(var->begin(), var->end(), var->begin(), ::tolower);
}
void Constant::upperCase(string *var){
    std::transform(var->begin(), var->end(), var->begin(), ::toupper);
}


bool Constant::isEspecialChar(string var){
    int i=0;
    bool found = false;
    int tam = sizeof(SPECIALCHARS) / sizeof(SPECIALCHARS[0]);

    while (i < tam && !found){
        if (var.compare(SPECIALCHARS[i]) == 0){
            found = true;
        }
        i++;
    }
    return found;
}

/**
*
*/
string Constant::removeEspecialChars(string var){
    string result = var;
    int tam = sizeof(ALLSPECIALCHARS) / sizeof(ALLSPECIALCHARS[0]);
    size_t pos = 0;
    for (int i=0; i < tam; i++){
        pos = var.find(ALLSPECIALCHARS[i]);
        if (pos != string::npos)
            result = replaceAll(result, ALLSPECIALCHARS[i], "");
    }
    return result;
}

bool Constant::isEspecialCharAll(string var){
    int i=0;
    bool found = false;
    int tam = sizeof(ALLSPECIALCHARS) / sizeof(ALLSPECIALCHARS[0]);

    while (i < tam && !found){
        if (var.compare(ALLSPECIALCHARS[i]) == 0){
            found = true;
        }
        i++;
    }
    return found;
}

/**
*
*/
string Constant::removeEspecialCharsAll(string var){
    string result = var;
    int tam = sizeof(ALLSPECIALCHARS) / sizeof(ALLSPECIALCHARS[0]);
    size_t pos = 0;
    for (int i=0; i < tam; i++){
        pos = var.find(ALLSPECIALCHARS[i]);
        if (pos != string::npos)
            result = replaceAll(result, ALLSPECIALCHARS[i], "");
    }
    return result;
}

/**
*
*/
string Constant::replaceMetaCaracters(string var){
    string result = var;
    int tam = sizeof(METACHARS) / sizeof(METACHARS[0]);
//    std::cout << tam << std::endl;
    for (int i=0; i < tam; i++){
        result = replaceAll(result, METACHARS[i], "\\" + TipoToStr(METACHARS[i]));
    }
    return result;
}


/**
*
*/
std::vector<std::string> &Constant::split(std::string s, std::string delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim.at(0))) {
        elems.push_back(item);
    }

//    if (!s.empty() && !delim.empty() && s.size() > 0){
//        char * writable = new char[s.size() + 1];
//        std::copy(s.begin(), s.end(), writable);
//        writable[s.size()] = '\0'; // don't forget the terminating 0
//
//        const char *delimConst = delim.c_str();
//
//        char * item = strtok (writable, delimConst);
//        while (item != NULL){
//          elems.push_back(item);
//          item = strtok (NULL, delimConst);
//        }
//        // don't forget to free the string after finished using it
//        delete[] writable;
//    }

    return elems;
}

/**
*
*/
std::vector<std::string> Constant::split(std::string s, std::string delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}


/**
*
*/
std::vector<std::string> Constant::splitChar(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return splitChar(s, delim, elems);
}

/**
*
*/
std::vector<std::string> &Constant::splitChar(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
//

//    if (!s.empty() && !delim.empty() && s.size() > 0){
//        char * writable = new char[s.size() + 1];
//        std::copy(s.begin(), s.end(), writable);
//        writable[s.size()] = '\0'; // don't forget the terminating 0
//
//        const char *delimConst = delim.c_str();
//
//        char * item = strtok (writable, delimConst);
//        while (item != NULL){
//          elems.push_back(item);
//          item = strtok (NULL, delimConst);
//        }
//        // don't forget to free the string after finished using it
//        delete[] writable;
//    }

    return elems;
}


/**
*
*/

std::vector<std::string> Constant::splitPos(std::string s, size_t posicion) {
    std::vector<std::string> elems;

    if (posicion <= s.length()){
        elems.push_back(s.substr(0, posicion));
        elems.push_back(s.substr(posicion, string::npos));
    } else {
        elems.push_back("");
        elems.push_back("");
    }
    return elems;
}


/**
* Obtiene la cadena en la posicion especificada de una cadena de caracteres, en la que cada
* componente esta separado por un delimitador
*/
char * Constant::split(const char *texto, char delim, unsigned int elemPos) {
    //
    if (texto != NULL) {
        const char delimConst[2] = {delim, '\0'};
        size_t tamTexto = strlen(texto);
        //Se hace una copia del texto
        char * writable = new char[tamTexto + 1];
        strcpy(writable, texto);
        writable[tamTexto] = '\0'; // don't forget the terminating 0
        char * item = strtok (writable, delimConst);
        unsigned int i=0;

        while (item != NULL) {
            if (i == elemPos){
                tamTexto = strlen(item);
                //Si encontramos un resultado, creamos una nueva variable y borramos la copia
                char * resp = new char[tamTexto + 1];
                strcpy(resp, item);
                writable[tamTexto] = '\0'; // don't forget the terminating 0
                delete[] writable;
                return resp;
            }
            item = strtok (NULL, delimConst);
            i++;
        }
        delete[] writable;
    }
    return NULL;
}

/**
*
*/
int Constant::contiene (const char *s1, const char *s2){
    int len_s1=0, len_s2=0; //Tamanyos de los strings
    int j=0;  //para recorrer s2
    int i=0;  //para recorrer s1

    if (s1 != NULL && s2 != NULL)
    {
        len_s1 = strlen(s1);
        len_s2 = strlen(s2);

        for (i = 0; i < len_s1; i++)
        {
            if (s1[i] == s2[j])
            {
                j++;
            }
            else if (j != len_s2)
            {
                j = 0;
                if (s1[i] == s2[j])
                {
                    j++;
                }
            }
        }
    }

    if (j == len_s2 && j != 0)
    {
        return (i-j);
    }
    else
    {
        return -1;
    }
}

/**
*
*/
int Constant::contiene (const char *s1, const char s2)
{
    int len_s1=0; //Tamanyos de los strings
    int i=0;  //para recorrer s1

    if (s1 != NULL)
    {
        len_s1 = strlen(s1);

        while (s1[i] != s2 && i<len_s1)
        {

            i++;
        }
    }

    if (i != len_s1)
    {
        return (i);
    }
    else
    {
        return -1;
    }
}

/**
*
*/
string Constant::pad(string var, int paddedLength, char caracter){
//            stringstream ss;
//            ss << setw(sizepad) << var;
    if (paddedLength > var.size())
        var.insert(var.begin(), paddedLength - var.size(), caracter);
    return var;
}

/**
*
*/
string Constant::replaceAll(std::string str, std::string tofind, std::string toreplace){
    size_t position = 0;
    size_t lastPosition = 0;
    string replaced = "";

    if (!str.empty()){
        for ( position = str.find(tofind); position != std::string::npos; position = str.find(tofind,lastPosition) ){
                replaced.append(str.substr(lastPosition, position - lastPosition));
                replaced.append(toreplace);
                lastPosition = position + tofind.length();
        }
        if (str.length() > 0){
            replaced.append(str.substr(lastPosition, str.length()));
        }
    }
    return(replaced);
}

/**
*
*/
string Constant::replace(std::string str, std::string tofind, std::string toreplace){
    size_t position = 0;
    size_t lastPosition = 0;
    string replaced = "";

    if (!str.empty()){
        if ( (position = str.find(tofind)) != std::string::npos){
                replaced.append(str.substr(lastPosition, position - lastPosition));
                replaced.append(toreplace);
                lastPosition = position + tofind.length();
        }
        if (str.length() > 0){
            replaced.append(str.substr(lastPosition, str.length()));
        }
    }
    return(replaced);
}


/**
*
*/
string Constant::replace(std::string str, std::string tofind, std::string toreplace, int fromPos){
    size_t position = 0;
    size_t lastPosition = 0;
    string replaced = "";

    if (!str.empty()){
        if ( (position = str.find(tofind, fromPos)) != std::string::npos){
                replaced.append(str.substr(lastPosition, position - lastPosition));
                replaced.append(toreplace);
                lastPosition = position + tofind.length();
        }
        if (str.length() > 0){
            replaced.append(str.substr(lastPosition, str.length()));
        }
    }
    return(replaced);
}

/**
*
*/
void Constant::stringTrim(char *dato)
{
    int tam = strlen(dato);
    //int i = tam - 1;
    int i = tam;
    bool salir = false;

    int j=0;

    while (!salir && tam > 1 && j < tam)
    {
        if (dato[0] == ' ')
        {
            for (int k=1;k<tam;k++)
            {
                dato[k-1] = dato[k];
            }
            if (tam > 1) dato[tam-1] = ' ';
        }
        else
        {
            salir = true;
        }
        j++;
    }

    if (i > 0)
    {
        while (i > 0 && dato[i] == ' ' )
        {
            i--;
            dato[i] = '\0';
        }
    }
}

/**
* Obtiene el separador de directorios de windows o unix
*/
string Constant::getFileSep(){
    char tmpFileSep[2] = {FILE_SEPARATOR,'\0'};
    return string(tmpFileSep);
}

/**
*
*/
string Constant::fecha(){
    string theFecha = "";
    time_t tSac = time(NULL);       // instante actual
    struct tm* tmP = localtime(&tSac);
    theFecha = pad(TipoToStr(tmP->tm_mday),2,'0') + "-" + pad(TipoToStr(tmP->tm_mon+1),2,'0') + "-" + TipoToStr(tmP->tm_year+1900);
    theFecha = theFecha + " " + pad(TipoToStr(tmP->tm_hour),2,'0') + ":" + pad(TipoToStr(tmP->tm_min),2,'0') + ":" + pad(TipoToStr(tmP->tm_sec),2,'0');
    return theFecha;
}

string Constant::fechaAnyo(){
    string theFecha = "";
    time_t tSac = time(NULL);       // instante actual
    struct tm* tmP = localtime(&tSac);
    theFecha = TipoToStr(tmP->tm_year+1900) + pad(TipoToStr(tmP->tm_mon+1),2,'0') + pad(TipoToStr(tmP->tm_mday),2,'0') ;
    return theFecha;
}

/**
*
*/
string Constant::httpDate(){
    string theFecha = "";
    time_t tSac = time(NULL);       // instante actual
    struct tm* tmP = localtime(&tSac);
    theFecha = TipoToStr(DIAS[tmP->tm_wday]) + "," + TipoToStr(tmP->tm_mday) + " " + TipoToStr(MESES[tmP->tm_mon]) + " " + TipoToStr(tmP->tm_year+1900);
    theFecha.append(" " + pad(TipoToStr(tmP->tm_hour),2, '0') + ":" + pad(TipoToStr(tmP->tm_min),2,'0') + ":" + pad(TipoToStr(tmP->tm_sec),2,'0') + " GMT");
    return theFecha;
}

/**
*
*/
string Constant::timeFormat(unsigned long long input_seconds){
    string retorno = "0:00:00";

    unsigned long long hours = input_seconds / 60 / 60;
    unsigned long long minutes = (input_seconds / 60) % 60;
    unsigned long long seconds = input_seconds % 60;

    retorno = pad(TipoToStr(hours), 1, '0') + ":" + pad(TipoToStr(minutes), 2, '0') + ":" + pad(TipoToStr(seconds), 2, '0');
    return retorno;
}

/**
*
*/
int Constant::stricmp(const char *s1, const char *s2){
    for (; *s1 && *s2 && (toupper(*s1) == toupper(*s2)); ++s1, ++s2);
    return *s1 - *s2;
}

/**
*
*/
int Constant::upperZero(int var){
    if (var > 0)
        return var;
    else return 0;
}

/**
*
*/
int Constant::countInStr(string var){
    std::size_t pos = 0;
    int counter = 0;
    if (!var.empty()){
        while ((pos = var.find("?", pos)) != string::npos && pos < var.length()){
            counter++;
            pos++;
        }
    }
    return counter;
}

/**
*
*/
void Constant::loadFromFile(string file, std::vector<std::string> *myVector){
    std::string line;
    std::ifstream infile(file.c_str());
    while (std::getline(infile, line)){
        std::istringstream iss(line);
        myVector->push_back(line);
    }
}

/**
*
*/
void Constant::setTime(string date, time_t *x){
    size_t pos = date.find("T");
    struct tm * time = new tm();

    if (pos != string::npos){

        string fecha = date.substr(0, pos);
        string hora = date.substr(pos+1);

        pos = hora.find("Z");
        if (pos != string::npos){
            hora = hora.substr(0, pos);
        }

        std::vector<std::string> vFecha = Constant::split(fecha, "-");
        std::vector<std::string> vHora = Constant::split(hora, ":");

        if (vFecha.size() >= 3 && vHora.size() >= 3){
            time->tm_year = Constant::strToTipo<int>(vFecha.at(0))  - 1900;
            time->tm_mon = Constant::strToTipo<int>(vFecha.at(1)) - 1;
            time->tm_mday = Constant::strToTipo<int>(vFecha.at(2));
            time->tm_hour = Constant::strToTipo<int>(vHora.at(0));
            time->tm_min = Constant::strToTipo<int>(vHora.at(1));
            time->tm_sec = Constant::strToTipo<int>(vHora.at(2));
            *x = mktime(time);
        }
    }
    delete time;
}

double Constant::diferenciaTiempos(time_t x, time_t y){
    double d = difftime(y, x);
    return d;
}


std::string Constant::base64_encode(string var) {
    char *cipheredStr = new char[var.length()+ 1];
    strcpy(cipheredStr,var.c_str());
    string base64Value = Constant::base64_encode(reinterpret_cast<const unsigned char*>(cipheredStr), var.length());
    delete [] cipheredStr;
    return base64Value;
}

std::string Constant::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string Constant::base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

/**
*
*/
inline bool Constant::is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}



/**
*
*/
std::string Constant::GetClipboardText(){
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
void Constant::waitms(unsigned long ms){
    #ifdef WIN
        Sleep(ms);
    #elif UNIX
        sleep(ms);
    #endif
}

/**
*
*/
std::string Constant::to_hex( unsigned int c ){
    std::ostringstream stm ;
    stm << '%' << std::hex << std::uppercase << setw(2) << (0x000000FF & c) ;
    return stm.str() ;
}

/**
*
*/
string Constant::url_encode(string value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << '%' << setw(2) << int((unsigned char) c);
    }

    return escaped.str();
}

string Constant::txtDisplay(string str){
    return toAnsiString(url_decode(str));
}

/**
 * 
 * @param str
 * @return 
 */
string Constant::url_decode(string str){
    string ret;
    char ch;
    int i, ii, len = str.length();

    for (i=0; i < len; i++){
        if(str[i] != '%'){
            if(str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }else{
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}



/**
*
*/
string Constant::uencodeUTF8(string latin1Str) {
    ostringstream escaped;
    unsigned char c;
    char tempStr[2] = {' ','\0'};
    for (unsigned int i=0; i < latin1Str.length(); i++){
        c = latin1Str.at(i);
        //Si es menor a 128, no hace falta codificar en UTF-8
        if (c < 128){
            tempStr[0] = c;
            escaped << url_encode(string(tempStr));
        } else {
            //En caso contrario codificamos en UTF-8
            escaped << to_hex(0xC0 | (c >> 6));
            escaped << to_hex(0x80 | (c & 0x3f));
        }
    }
    return escaped.str();
}

string Constant::udecodeUTF8(string &SRC) {
    string ret;
    char ch;
    int ii;
    char utf8Part[3] = {' ',' ','\0'};

    for (unsigned i=0; i<SRC.length(); i++) {
        //Se busca el porcentaje
        if (int(SRC[i])==37) {
            sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
            if (ii >= 128){
                utf8Part[0] = static_cast<char>(ii);
                //Saltamos 3 porque sabemos que hay otro porcentaje
                //de por medio, ya que en utf tenemos dos numeros
                i = i + 3;
                sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
                utf8Part[1] = static_cast<char>(ii);
                Constant::utf8ascii(utf8Part);
                //Nos quedamos con el primer elemento
                ret += utf8Part[0];
                i = i + 2;
            } else {
                ch = static_cast<char>(ii);
                ret += ch;
                i = i + 2;
            }

        } else {
            ret += SRC[i];
        }
    }
    return (ret);
}


// Convert a single Character from UTF8 to Extended ASCII
// Return "0" if a byte has to be ignored
uint8_t Constant::utf8ascii(uint8_t ascii) {
    if ( ascii<128 )   // Standard ASCII-set 0..0x7F handling
    {   c1=0;
        return( ascii );
    }

    // get previous input
    uint8_t last = c1;   // get last char
    c1=ascii;         // remember actual character

    switch (last)     // conversion depnding on first UTF8-character
    {   case 0xC2: return  (ascii);  break;
        case 0xC3: return  (ascii | 0xC0);  break;
        case 0x82: if(ascii==0xAC) return(0x80);       // special case Euro-symbol
    }

    return  (0);                                     // otherwise: return zero, if character has to be ignored
}

// In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
void Constant::utf8ascii(char* s){
        int k=0;
        char c;
        for (unsigned int i=0; i<strlen(s); i++)
        {
                c = utf8ascii(s[i]);
                //c = codePointToUTF8(s[i])[0];
                if (c!=0)
                        s[k++]=c;
        }
        s[k]=0;
}

string Constant::toAnsiString(string s){
    string salida = "";
    char c;
    for (unsigned int i=0; i<s.length(); i++){
            c = utf8ascii(s.at(i));
            if (c!=0){
                salida += c;
            }
    }
    return salida;
}

string Constant::toUTF8(string s){
    string salida;
    int iChar;
    for (unsigned int i=0; i<s.length(); i++){
            iChar = (int)s.at(i);
            if (iChar < 0){
                iChar = 0x100 + iChar;
            }
            salida += codePointToUTF8(iChar);
    }
    return salida;
}

/// Converts a unicode code-point to UTF-8.
inline std::string Constant::codePointToUTF8(unsigned int cp) {
  std::string result;
  // based on description from http://en.wikipedia.org/wiki/UTF-8
  if (cp <= 0x7f) {
    result.resize(1);
    result[0] = static_cast<char>(cp);
  } else if (cp <= 0x7FF) {
    result.resize(2);
    result[1] = static_cast<char>(0x80 | (0x3f & cp));
    result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
  } else if (cp <= 0xFFFF) {
    result.resize(3);
    result[2] = static_cast<char>(0x80 | (0x3f & cp));
    result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
    result[0] = static_cast<char>(0xE0 | (0xf & (cp >> 12)));
  } else if (cp <= 0x10FFFF) {
    result.resize(4);
    result[3] = static_cast<char>(0x80 | (0x3f & cp));
    result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
    result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
    result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
  }

  return result;
}

/**
 * Removes all character not alphanumeric
 * @param input
 * @return 
 */
string Constant::toAlphanumeric(string input){
    string out = "";
    for (int i=0; i < input.size(); i++) {
        if ( ((int)input[i] >= 48 && (int)input[i] < 57) ||
             ((int)input[i] >= 65 && (int)input[i] < 90) ||
             ((int)input[i] >= 97 && (int)input[i] < 122)){
            out += input[i];
        }
    }
    return out;    
}

/**
*
*/
unsigned int Constant::getRandomRange(int min_, int max_){
    unsigned int next = min_ + (rand() % (int)(max_ - min_ + 1));
    return next;
}

/**
*
*/
int Constant::contarPalabrasIguales(string str1, string str2){
    Constant::lowerCase(&str1);
    Constant::lowerCase(&str2);

    std::vector<std::string> arr = Constant::split(str2, " ");
    int cont = 0;
    for (unsigned int i=0; i < arr.size(); i++){
        if (str1.find(arr.at(i)) != string::npos){
           cont++;
        }
    }
    return cont;
}

/**
*
*/

int Constant::getHostname(string hostname, t_hostname *structHostname){
#ifdef win
    DWORD dwError;
    char **pAlias;
    int i = 0;
    struct in_addr addr;
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
//        printf("WSAStartup failed: %d\n", iResult);
        structHostname->error = "WSAStartup failed: " + Constant::TipoToStr(iResult);
        return 1;
    }

    struct hostent *remoteHost = gethostbyname(hostname.c_str());

    if (remoteHost == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
//                printf("Host not found\n");
                structHostname->error = "Host not found";
                return 1;
            } else if (dwError == WSANO_DATA) {
//                printf("No data record found\n");
                structHostname->error = "No data record found";
                return 1;
            } else {
//                printf("Function failed with error: %ld\n", dwError);
                structHostname->error = "Function failed with error: " + Constant::TipoToStr(dwError);
                return 1;
            }
        }
    } else {
//        printf("Function returned:\n");
//        printf("\tOfficial name: %s\n", remoteHost->h_name);
        structHostname->hostname = remoteHost->h_name;
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
            //printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
            structHostname->altHostname.push_back(*pAlias);
        }
        structHostname->addType = remoteHost->h_addrtype;

//        printf("\tAddress type: ");
//        switch (remoteHost->h_addrtype) {
//        case AF_INET:
//            printf("AF_INET\n");
//            break;
//        case AF_NETBIOS:
//            printf("AF_NETBIOS\n");
//            break;
//        default:
//            printf(" %d\n", remoteHost->h_addrtype);
//            break;
//        }
//        printf("\tAddress length: %d\n", remoteHost->h_length);

        i = 0;
        if (remoteHost->h_addrtype == AF_INET){
            while (remoteHost->h_addr_list[i] != 0) {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
//                printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
                structHostname->ip.push_back(inet_ntoa(addr));
            }
        } else if (remoteHost->h_addrtype == AF_NETBIOS){
//            printf("NETBIOS address was returned\n");
            structHostname->error = "NETBIOS address was returned";
        }
    }
    WSACleanup();
    return 0;
#endif

#ifdef UNIX
    char Name[150];
//    int i=0;

//    #ifdef WIN32
//        TCHAR infoBuf[150];
//        DWORD bufCharCount = 150;
//        memset(Name, 0, 150);
//        if( GetComputerName( infoBuf, &bufCharCount ) )
//        {
//            for(i=0; i<150; i++)
//            {
//                Name[i] = infoBuf[i];
//            }
//        }
//        else
//        {
//            strcpy(Name, "Unknown_Host_Name");
//        }
//    #else
        memset(Name, 0, 150);
        gethostname(Name, 150);
//    #endif
    structHostname->hostname.assign(Name);
    structHostname->altHostname.push_back(Name);
    
        return 0;
#endif    
    
}

std::string Constant::TrimLeft(const std::string& s)
{
    size_t startpos = s.find_first_not_of(WHITESPACE);
    return (startpos == std::string::npos) ? "" : s.substr(startpos);
}

std::string Constant::TrimRight(const std::string& s)
{
    size_t endpos = s.find_last_not_of(WHITESPACE);
    return (endpos == std::string::npos) ? "" : s.substr(0, endpos+1);
}

std::string Constant::Trim(const std::string& s)
{   
    return TrimRight(TrimLeft(s));
}

static void Constant::setCURL_DOWNLOAD_LIMIT(size_t CURL_DOWNLOAD_LIMIT) {
    Constant::CURL_DOWNLOAD_LIMIT = CURL_DOWNLOAD_LIMIT;
}

static size_t Constant::getCURL_DOWNLOAD_LIMIT() {
    return CURL_DOWNLOAD_LIMIT;
}
