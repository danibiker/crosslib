#ifndef FILELAUNCH_H_INCLUDED
#define FILELAUNCH_H_INCLUDED

#include <string>
using namespace std;

class FileLaunch{
        public:
             FileLaunch();
             ~FileLaunch();
             string nombreemu;
             string rutaexe;
             string fileexe;
             string parmsexe;
             string rutaroms;
             string nombrerom;
             string titlerom;
             string nplayers;
             string categ;
             bool descomprimir;
             bool fixoption;
    };

#endif // FILELAUNCH_H_INCLUDED
