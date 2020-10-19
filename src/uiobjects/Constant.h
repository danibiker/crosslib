#ifndef CONSTANT_H_INCLUDED
#define CONSTANT_H_INCLUDED

#ifdef WIN
    #ifndef WINVER
        #define WINVER 0x0502
    #endif
    #include <winsock2.h>
    #include <windows.h>
#else 
    #include <unistd.h>
#endif

#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <typeinfo>
#include <ctime>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <algorithm>  //transform
#include <cctype>
#include <iomanip>
#include <cstddef>
#include <stdint.h>
#include <math.h>


using namespace std;

#define PI 3.141592653589793238462643383279502884
#define MOUSE_BUTTON_LEFT		1
#define MOUSE_BUTTON_MIDDLE	2
#define MOUSE_BUTTON_RIGHT	3
#define MOUSE_BUTTON_WHEELUP	4
#define MOUSE_BUTTON_WHEELDOWN	5
#define MOUSE_BUTTON_X1         6
#define MOUSE_BUTTON_X2         7
/* set this to any of 512,1024,2048,4096              */
/* the lower it is, the more FPS shown and CPU needed */
#define BUFFERSPECTRUMVIS 4096

//static const int gpsinoW = 640;
//static const int gpsinoH = 350;
static const int gpsinoW = 128;
static const int gpsinoH = 160;
static const int mapWidth = 256;
static const int mapHeight = 256;

    struct t_color{
        //t_color(int r, int g, int b){this->r = r; this->g = g; this->b = b;}
        int r;
        int g;
        int b;
    };

    class t_posicion{
        public:

        t_posicion(int x, int y, int w, int h){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }

        t_posicion(){
            x=0;y=0;w=0;h=0;
        }

        int x;
        int y;
        int w;
        int h;
    };

    struct t_iPos{
        int x;
        int y;
        int w;
        int h;
    };

    struct t_hostname{
        string error;
        int addType;
        string hostname;
        vector<string> ip;
        vector<string> altHostname;
    };

    struct t_posicion_precise{
        double x;
        double y;
        double w;
        double h;
    };




    typedef enum {
        launch_system,          //0
        launch_spawn,           //1
        launch_create_process,  //2
    } launchMethods;

    typedef enum {
        cursor_hidden,
        cursor_arrow,
        cursor_resize,
        cursor_hand,
        cursor_wait,
        totalCursors
    } enumCursors;


    typedef enum {
        page_white_text,
        folder,
        page_white,
        bullet_go,
        door_out,
        bullet_wrench,
        clockIco,
        page_find,
        folder_explore,
        magnifier,
        chart_bar,
        cross,
        tick,
        fontIco,
        font_add,
        color_swatch,
        text_align_justify,
        layout_sidebar,
        clock_delete,
        book_go,
        bullet_black,
        page_white_gear,
        page_white_compressed,
        page_white_acrobat,
        page_white_c,
        page_white_h,
        page_white_paint,
        application_form_edit,
        deleteIco,
        sound,
        sound_mute,
        emoticon_waii,
        page_white_zip,
        page_white_picture,
        controller,
        add,
        boton,
        boton_selected,
        reloj_de_arena,
        arrow_refresh,
        application_xp_terminal,
        film,
        control_fastforward ,
        control_fastforward_blue ,
        control_pause ,
        control_pause_blue ,
        control_play ,
        control_play_blue ,
        control_rewind ,
        control_rewind_blue ,
        control_stop ,
        control_stop_blue,
        ico_h_resize,
        database_add,
        music,
        btn_repeat,
        btn_repeat_off,
        btn_random,
        btn_random_off,
        FlechaEsquinaSupIzq,
        control_equalizer,
        control_equalizer_blue,
        btnSliderEQ,
        btn_off,
        btn_on,
        btn_reset_eq,
        dropbox_png,
        google_png,
        boton_disabled,
        server_add,
        cd_add,
        cd,
        drive,
        drive_cd,
        drive_disk,
        drive_network,
        map_go,
        onedrive_png,
        new_window,
//        trackSeg,
//        map_add,
        directory,
        totalIcons
    } enumIco;

    typedef enum{ TIPODIRECTORIO,
            TIPOFICHERO
    } enumFileAttr;

    static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

    static const char* SIZES_STR[] = {"B","KB","MB","GB","TB"};

    typedef enum{W_FATAL = 0,W_ERROR,W_WARN,W_INFO,W_DEBUG,W_PARANOIC} enumTraza;

    static const string USERAGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0";
    static const string OBJLISTABROWSER2 = "listaBrowser2";
    static const string BTNACEPTARBROWSER = "btnAceptarDir2";
    static const string BTNCANCELARBROWSER = "btnCancelarDir2";
    static const string TITLESCREEN = "TitleScreen";
    static const string ARTDIRBROWSER = TITLESCREEN;
    static const string TOKENFILENAME = "token.ini";


    //static const int MAXMENU = 13;  //Maximo numero de menus disponibles
    static const int MAXOBJECTS = 40; //Numero maximo de objetos (inputs, checks, etc.) que se pueden mostrar en una pagina
    //*********************************************************************************/
    //Cuidado con no sobrepasar el numero de elementos que define la constante MAXMENU
    //*********************************************************************************/

    static const t_color color1 = {0xBB, 0xDB, 0x9B};
    static const t_color color2 = {0xAB, 0xC4, 0xA1};
    static const t_color color3 = {0x9D, 0xB4, 0xAB};
    static const t_color color4 = {0x8D, 0x9D, 0x90};
    static const t_color color5 = {0x87, 0x8E, 0x76};


    static const t_color cNegro = {0,0,0};
    static const t_color cNegroClaro = {0x18,0x18,0x18};
    static const t_color cBlanco = {0xFF,0xFF,0xFF};
    static const t_color cMagenta = {0xFF,0x00,0xFF};
    static const t_color cVerdeOscuro = {56,142,9};
    static const t_color cRojo  = {0xFF,0,0};
    static const t_color cVerde = {0,0xFF,0};
    static const t_color cTurquesa = {0,127,127};
    static const t_color cAmarillo = {0xFF,0xFF,0};
    static const t_color cAzul  = {66,134,244};
    static const t_color cAzulOscuro = {50,104,202};
    static const t_color cAzulTotal  = {0,0,0xFF};
    static const t_color cGris  = {0x80,0x80,0x80};
    static const t_color cGrisOscuro  = {0x28,0x28,0x28};
    static const t_color cGrisClaro  = {225,225,225};
    static const t_color cNaranja  = {0xFA,0x70,0x00};
    static const t_color cBgScreen = cGrisOscuro;
    static const t_color cBgImageBin = cNegro;
    static const t_color cBgMenu = cBlanco;
    static const t_color cBorde1Button = {0x40,0x40,0x40};
    static const t_color cBorde2Button = cGris;
    static const t_color cTitleScreen = cAzulOscuro;
    static const t_color cFondoMapa = {0,0,51};
    static const t_color cColorElevacion = {127,127,153};
    static const t_color cColorThumbSelected = {204,232,255};
    static const t_color cColorThumbSelectedBorder = {153,209,255};
    static const t_color cColorThumbRelieve1 = {187,187,187};
    static const t_color cColorThumbRelieve2 = {221,221,221};


    //static const t_color cButtonContent = {0xD4,0xD0,0xC8};
    static const t_color cButtonContent = cAzul;
    static const t_color cInputBorder = {0x78,0x78,0x78};
    static const t_color cInputContent = cBlanco;
    static const t_color cSeparator = {237, 237, 237};
    static const t_color cSelection = cAzul;

    static const bool TRAZA_ENABLED = true;
    static const unsigned int INILIST = 30; //Numero de elementos que se crearan inicialmente para las listas
    static const char FONT_TYPE[] = {"Arimo-Regular.ttf"}; //Fuente de los textos
    static const char ARRAYLETRAS[] = {" abcdefghijklmnï¿½opqrstuvwxyz0123456789-+*/,;.:-_[]{}'?ï¿½!ï¿½|@#$%&()"};
    static const char FILEBBDD[] = {"romgestor.sqlite"};
    static const bool SHOWFPS = false;   //Muestra los frames actuales
    static const bool LIMITFPS = true; //Especifica si se realiza una limitacion maxima de frames
    static const unsigned long FRAMELIMIT = 30; //Limite de frames
    static const unsigned long FRAMEPERIOD = 1000; //Periodo en el que se comprobara la velocidad de frames. en ms
    static const unsigned long TIMETOLIMITFRAME = FRAMEPERIOD/FRAMELIMIT; //Tiempo que deberia utilizar cada frame
    static const int COLORMENUSEL = 0xFF9F9F; //El color tiene que ser clarito
    static const unsigned int MAXQUERY = 10; //Maxima longitud de palabra de un input
    static const unsigned int MAXQUERYINT = 10; //Maxima longitud de palabra de un input de tipo int
    static const unsigned long DBLCLICKSPEED = 300; //tiempo en ms para poder hacer un doble click
    static const unsigned long KEYDOWNSPEED = 50;
    static const unsigned long KEYRETRASO = 500;
    static const unsigned long MOUSEVISIBLE = 8000;
    static const double ZOOMINCREMENT = 0.1; //Incremento de zoom que se aplicara a la imagen
    static const int MOVEINCREMENT = 15; //Pixels que se desplazara la imagen con una pulsacion
    static const bool VIEWALPHA = false;
    static const float zoomText = 1;
    static const int textFontYCorrection = 0;
    static const int ICOSPACE = 25 * zoomText;
    static const int FAMFAMICONW = 16 * zoomText;
    static const int FAMFAMICONH = 16 * zoomText;
    static const int ICOBOTTOMSPACE = 5 * zoomText;
    static const unsigned char FONTSIZE = 12 * zoomText;
    static const int BORDERSELECTION = 2 * zoomText;
    static const int CHECKW = 11 * zoomText; //Ancho de la cajita de seleccion de un input check
    static const int CHECKH = 11 * zoomText; //Alto de la cajita de seleccion de un input check
    static const int BUTTONW = 165 * zoomText;
    static const int BUTTONH = 25 * zoomText;
    static const int BUTTONSERVERW = 100 * zoomText;
    static const int MINIBUTTONH = 25 * zoomText;
    static const int MINIBUTTONW = 25 * zoomText;
    static const int TIMEW = 50;
    static const int SEPTIMER = 8;
    static const int PROGRESSSEPBOTTOM = 15;
    static const int SEPPANELMEDIA = 10;
    static const int PROGRESSHEIGHT = 10;
    static const int BUTTONBORDER = 1; //Borde del boton
    //Variables para el formulario de querys
    static const int SPACIOLETRA = 14; //Separacion de cada letra del contenido del input
    static const int INPUTW=540 * zoomText;
    static const int INPUTBORDER = 1; //Borde del input
    static const int TITLEBORDER = 10 * zoomText; //Separacion del borde respecto de la izquierda de la pantalla
    static const int TITLESPACE = 10 * zoomText; //Separacion del titulo respecto del borde
    static const int TITLEFONTSIZE = 20; //Tamanyo del titulo
    static const int INPUTCONTENT = 4 * zoomText; //Separacion en y del contenido del input respecto al input
    static const int INPUTDELIMITER = 5 * zoomText; //Separacion en y de los delimitadores del contenido del input respecto al contenido del input
    static const int INPUTDELIMITERSELECTOR = 24; //Separacion en y del selector (el triangulo) del contenido del input respecto al contenido del input
    static const int SELECTORBASE = 10* zoomText; //Tamanyo de la base del triangulo del selector del input
    static const int SELECTORHEIGHT = 10* zoomText; //Tamanyo de la altura del triangulo del selector del input
    static const int TEXLABELTEXTSPACE = 5 * zoomText; //Separacion del label respecto a su texto
    static const int IMGBOXARTMARGIN = 10 * zoomText; //Alto de la imagen de la rom
    static const int ALBUMWIDTH = 200;
    static const int COMBOHEIGHT = 20;
    //Variables para el formulario de querys
    static const int MENUCATEGWIDTH = 100;
    static const int TRIGLTAM = 10;
    static const int TRISCROLLBARTAM = 8;
    static const int SCROLLBARWIDTH = 6;
    static const int MINSCROLLBARHEIGHT = 20;
    static const int DEADZONE = 23000;
    static const int JOYHATOFFSET = 100;
    static const int JOYAXISOFFSET = 200;
    static const string URI_SEPARATOR = "/";
    static const string DIR_IMG_ROMS_IMGS = "romImgs";

     /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        static const uint32_t rmask = 0xff000000;
        static const uint32_t gmask = 0x00ff0000;
        static const uint32_t bmask = 0x0000ff00;
        static const uint32_t amask = 0x000000ff;
    #else
        static const uint32_t rmask = 0x000000ff;
        static const uint32_t gmask = 0x0000ff00;
        static const uint32_t bmask = 0x00ff0000;
        static const uint32_t amask = 0xff000000;
    #endif

    typedef enum {JOY_BUTTON_A = 0,
                        JOY_BUTTON_B,
                        JOY_BUTTON_X,
                        JOY_BUTTON_Y,
                        JOY_BUTTON_L,
                        JOY_BUTTON_R,
                        JOY_BUTTON_SELECT,
                        JOY_BUTTON_START,
                        JOY_BUTTON_L3,
                        JOY_BUTTON_R3,
                        JOY_BUTTON_UP,
                        JOY_BUTTON_UPLEFT,
                        JOY_BUTTON_LEFT,
                        JOY_BUTTON_DOWNLEFT,
                        JOY_BUTTON_DOWN,
                        JOY_BUTTON_DOWNRIGHT,
                        JOY_BUTTON_RIGHT,
                        JOY_BUTTON_UPRIGHT,
                        JOY_BUTTON_VOLUP,
                        JOY_BUTTON_VOLDOWN,
                        JOY_BUTTON_CLICK,
                        JOY_AXIS1_RIGHT,
                        JOY_AXIS1_LEFT,
                        JOY_AXIS1_UP,
                        JOY_AXIS1_DOWN,
                        JOY_AXIS2_RIGHT,
                        JOY_AXIS2_LEFT,
                        JOY_AXIS2_UP,
                        JOY_AXIS2_DOWN,
                        JOY_AXIS_L2,
                        JOY_AXIS_R2,
                        MAXJOYBUTTONS} joystickButtons;



    //Dependientes del sistema
    static const char FILE_SEPARATOR_UNIX = '/';

    #ifdef WIN
        static const int SCREEN_BITS = 32;
        static char FILE_SEPARATOR = 0x5C; //Separador de directorios para win32
        static const char FILE_PARENT[] = {"\\.."}; //Separador de directorios para win32
        static const char RUTA_TRAZA[] = {"D:\\traza.txt"}; //Directorio de las trazas
        static const char RUTA_TRAZA_PROC[] = {"C:\\trazaProc.txt"}; //Directorio de las trazas
        static const char RUTA_TRAZA_HIJO[] = {"C:\\trazaHIJO.txt"}; //Directorio de las trazas
        static const char RUTA_RETROARCH_CFG[] = {"es_systems.cfg"};
        //static const char RUTA_TRAZA[] = {"C:\\traza.txt"}; //Directorio de las trazas
        static const char VBCRLF[] = {"\n"}; //Retorno de carro para las trazas o ficheros en windows \n
        static const int CURSORVISIBLE = 1;
        static const int THUMBW = 0;    //Si THUMBW != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBX
        static const int THUMBH = 0;    //Si THUMBH != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBY
        static const int MAXTHUMBX = 7; //Si THUMBW == 0 ponemos por defecto este numero de imagenes en una fila
        static const int MAXTHUMBY = 5; //Si THUMBH == 0 ponemos por defecto este numero de imagenes en una colummna
        static const int SEPTHUMB = 2;  //Separacion del cuadro de seleccion de cada thumb
        static const string CMD_LAUNCH_BROWSER = "explorer";
    #elif UNIX
        static const int SCREEN_BITS = 16;
        static char FILE_SEPARATOR = FILE_SEPARATOR_UNIX; //Separador de directorios para unix
        static const char FILE_PARENT[] = {"/.."}; //Separador de directorios para unix
        static const char RUTA_TRAZA[] = {"/mnt/usb/traza.txt"}; //Directorio de las trazas
        static const char RUTA_TRAZA_PROC[] = {"./trazaProc.txt"}; //Directorio de las trazas
        static const char RUTA_TRAZA_HIJO[] = {"./trazaHijo.txt"}; //Directorio de las trazas
        static const char RUTA_RETROARCH_CFG[] = {"/home/pi/.emulationstation/es_systems.cfg"};
        static const char VBCRLF[] = {"\r\n"}; //Retorno de carro para las trazas o ficheros en UNIX \r\n
        static const int CURSORVISIBLE = 1;
        static const int THUMBW = 0;    //Si THUMBW != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBX
        static const int THUMBH = 0;    //Si THUMBH != 0, las imagenes del thumb tendran el tamanyo especificado MAXTHUMBY
        static const int MAXTHUMBX = 3; //Si THUMBW == 0 ponemos por defecto este numero de imagenes en una fila
        static const int MAXTHUMBY = 3; //Si THUMBH == 0 ponemos por defecto este numero de imagenes en una colummna
        static const int SEPTHUMB = 2;  //Separacion del cuadro de seleccion de cada thumb
        static const string CMD_LAUNCH_BROWSER = "x-www-browser";
    #endif


//    #ifndef WIN
//        typedef unsigned long DWORD;
//        typedef unsigned short WORD;
//        typedef unsigned int UNINT32;
//        typedef uint8_t byte;
//    #endif
        //No puede ser constante porque
        static char tempFileSep[2] = {FILE_SEPARATOR,'\0'};
        static const double EARTHRADIUS = 6367.0;

        static const int MAXVALUEFFT = 1500;
        static const int NBIQUADFILTERS = 9;
        static const double frecsEQ[NBIQUADFILTERS] = {60.0,170.0,310.0,600.0,1000.0,3000.0,6000.0,10000.0,14000.0};
        //Los valores de winamp eran: 60, 170, 310, 600, 1000, 3000, 6000, 12000, 14000, 16000
        static const char frecsEQStr[NBIQUADFILTERS][5] = {{"60"},{"170"},{"310"},{"600"},{"1K"},{"3K"},{"6K"},{"10K"},{"14K"}};
        static const double QFrecsEQ[NBIQUADFILTERS] = {1.0,1.0,1.0,2.0,5.0,5.0,5.0,5.0,5.0};
        static const std::string WHITESPACE = " \n\r\t";
        static const string DIR_PREV = "thumbs";

class Constant{
    private:
        static string appDir;
        static int WINDOW_WIDTH;
        static int WINDOW_HEIGHT;
        static int TRAZA_LEVEL;
        static int MENUSPACE;
        static int INPUTH;
        static int EXEC_METHOD;
        static string PROXYIP;
        static string PROXYPORT;
        static string PROXYUSER;
        static string PROXYPASS;
        static int MAXTHREADSDOWN;
        static bool UPDATE_MISSING;
        static int COMBOLISTHEIGHT;
        static int IMGBOXARTWIDTH; //Ancho de la imagen de la rom
        static int IMGBOXARTHEIGHT; //Alto de la imagen de la rom
        static uint8_t c1;  // Last character buffer
        static size_t CURL_DOWNLOAD_LIMIT;
        static t_color BACKGROUNDCOLOR;
        

    public:

        Constant(){}
        ~Constant(){};
        static int getWINDOW_WIDTH(){return WINDOW_WIDTH;}
        static void setWINDOW_WIDTH(int pWidth){
            WINDOW_WIDTH = pWidth;
        }
        static int getWINDOW_HEIGHT(){return WINDOW_HEIGHT;}
        static void setWINDOW_HEIGHT(int pHeight){
            WINDOW_HEIGHT = pHeight;
        }
        static void setTrazaLevel(int var){TRAZA_LEVEL = var;}
        static int getTrazaLevel(){return TRAZA_LEVEL;}
        static int upperZero(int);
        static int getMENUSPACE(){return MENUSPACE;}
        static void setMENUSPACE(int var){MENUSPACE = var;}
        static int getINPUTH(){return INPUTH;}
        static void setINPUTH(int var){INPUTH = var;}
        static void setExecMethod(int var){EXEC_METHOD = var;}
        static int getExecMethod(){return EXEC_METHOD;}
        static string getPROXYIP(){return PROXYIP;}
        static void setPROXYIP(string var){PROXYIP = var;}
        static string getPROXYPORT(){return PROXYPORT;}
        static void setPROXYPORT(string var){PROXYPORT = var;}
        static string getPROXYUSER(){return PROXYUSER;}
        static void setPROXYUSER(string var){PROXYUSER = var;}
        static string getPROXYPASS(){return PROXYPASS;}
        static void setPROXYPASS(string var){PROXYPASS = var;}
        static void setMAXTHREADSDOWN(int var){MAXTHREADSDOWN = var;}
        static int getMAXTHREADSDOWN(){return MAXTHREADSDOWN;}
        static bool isUPDATE_MISSING(){return UPDATE_MISSING;}
        static void setUPDATE_MISSING(bool var){UPDATE_MISSING = var;}
        static int getCOMBOLISTHEIGHT(){return COMBOLISTHEIGHT;}
        static void setCOMBOLISTHEIGHT(int var){COMBOLISTHEIGHT = var;}
        static int getIMGBOXARTWIDTH(){return IMGBOXARTWIDTH;}
        static void setIMGBOXARTWIDTH(int var){IMGBOXARTWIDTH = var;}
        static int getIMGBOXARTHEIGHT(){return IMGBOXARTHEIGHT;}
        static void setIMGBOXARTHEIGHT(int var){IMGBOXARTHEIGHT = var;}
        static void setBACKGROUNDCOLOR(t_color BACKGROUNDCOLOR);
        static t_color getBACKGROUNDCOLOR();


        template<class TIPO> static string TipoToStr(TIPO number){
           stringstream ss;//create a stringstream
           ss << number;//add number to the stream
           return ss.str();//return a string with the contents of the stream
        }

        template<class TIPO> static TIPO strToTipo(string str){
                TIPO i;
                stringstream s_str( str );
                s_str >> i;
                return i;
        }
        static string getAppDir();
        static void setAppDir(string var);
        static std::vector<std::string> &split(std::string s, std::string  delim, std::vector<std::string> &elems);
        static std::vector<std::string> split(std::string s, std::string  delim);
        static std::vector<std::string> splitPos(std::string s, size_t posicion);
        static char * split(const char *texto, char delim, unsigned int elemPos);
        static std::vector<std::string> splitChar(const std::string &s, char delim);
        static std::vector<std::string> &splitChar(const std::string &s, char delim, std::vector<std::string> &elems);
        static int contiene (const char *s1, const char *s2);
        static int contiene (const char *s1, const char s2);
        static string pad(string var, int paddedLength, char caracter);
        static std::string padRight(std::string str, size_t s, char caracter);
        static string replaceAll(std::string str, std::string tofind, std::string toreplace);
        static string replace(std::string str, std::string tofind, std::string toreplace);
        static string replace(std::string str, std::string tofind, std::string toreplace, int fromPos);
        static string replaceMetaCaracters(string);
        static string toAlphanumeric(string input);
        static void stringTrim(char *dato);
        static string getFileSep();
        static string fecha();
        static string fechaAnyo();
        static string httpDate();
        static void lowerCase(string *var);
        static void upperCase(string *var);
        static int stricmp(const char *s1, const char *s2);
        static string timeFormat(unsigned long long input_seconds);
        static int countInStr(string var);
        static bool isEspecialChar(string var);
        static string removeEspecialChars(string var);
        static bool isEspecialCharAll(string var);
        static string removeEspecialCharsAll(string var);
        static void loadFromFile(string file, std::vector<std::string> *myVector);
        static double diferenciaTiempos(time_t x, time_t y);
        static void setTime(string date, time_t *x);
        static uint8_t utf8ascii(uint8_t ascii);
        static void utf8ascii(char* s);
        static string toAnsiString(string dato);
        static string toUTF8(string s);
        static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
        static std::string base64_encode(string var);
        static std::string base64_decode(std::string const& encoded_string);
        static inline bool is_base64(unsigned char c);
        static string url_encode(string value);
        static string url_decode(string str);
        static string txtDisplay(string str);
        static std::string to_hex( unsigned int c );
        static string uencodeUTF8(string latin1Str);
        static string udecodeUTF8(string &SRC);
        static inline std::string codePointToUTF8(unsigned int cp);
        static unsigned int getRandomRange(int min_, int max_);
        static int contarPalabrasIguales(string str1, string str2);
        static int getHostname(string hostname, t_hostname *structHostname);
        static std::string TrimLeft(const std::string& s);
        static std::string TrimRight(const std::string& s);
        static std::string Trim(const std::string& s);
        static std::string cutToLength(const std::string& s, int cutLen);
        static std::string printBytesSize(double bytes, int precision);
        static void setCURL_DOWNLOAD_LIMIT(size_t CURL_DOWNLOAD_LIMIT);
        static size_t getCURL_DOWNLOAD_LIMIT();

};

#endif // CONSTANT_H_INCLUDED
