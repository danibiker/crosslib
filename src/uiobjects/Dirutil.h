#ifndef Dirutil_H
#define Dirutil_H


#include "fileprops.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "Traza.h"
#include "ListaSimple.h"
#include <stdio.h>
#include <vector>

#ifndef lint
/* Prevent over-aggressive optimizers from eliminating ID string */
const char jlss_id_mkpath_c[] = "@(#)$Id: mkpath.c,v 1.13 2012/07/15 00:40:37 jleffler Exp $";
#endif /* lint */

#define STR_DIR_EXT "Dir"

struct t_drive{
    string drive;
    int driveType;
    string driveTypeString;
    int ico;
    string label;
};

class Dirutil{
    public :
        Dirutil(); //Constructor
        ~Dirutil(); //Destructor

        void getProgramDir(char *pathname, int len); //Recupera el directorio y nombre del propio programa
        char * getDirActual(); //Recupera el directorio actual de navegacion
        bool changeDirRelative(const char *);
        bool changeDirAbsolute(const char *);
        void countDir(const char *, unsigned int *);
        unsigned int countDir(const char *);
        void dircat(char *, const char *);
        string getFolder(string );
        string getFileName(string );
        string getFileNameNoExt(string file);
        void listarDirRecursivo(string ,  vector <FileProps> * );
        void listarDirRecursivo(string dir,  vector <FileProps> * filelist, string filtro);
        unsigned int listarDir(const char *, listaSimple<FileProps> * );
        unsigned int listarDir(const char *strdir, listaSimple<FileProps> * filelist, string filtro);
        unsigned int listarFilesFast(const char *strdir, vector<FileProps> *filelist, string filtro);
        unsigned int listarFilesSuperFast(const char *strdir, vector<FileProps> *filelist, string filtro, bool order, bool properties);
        unsigned int listarDirFast(const char *strdir, vector<FileProps> * filelist, string filtro);
        unsigned int listarDirSinOrden(const char *, vector <FileProps> *, vector <FileProps> *, string filtro);
        unsigned int listarDirSinOrdenMaxFiles(const char *strdir, vector <FileProps> * tempFilelist
                                        ,vector <FileProps> * filelistFinal, string filtro, int maxSearchFiles, int maxResults);
        void countDir(const char *strdir, unsigned int *nFiles, vector<string> *files, string filtro);
        bool borrarArchivo(string);
        void borrarDir(string);
        bool setFileProperties(FileProps *propFile, string ruta);

        bool existe(string);
        bool isDir(string);
        string getExtension(string);
        int findIcon(const char *);
        void copyFile(string srcFile, string dstFile);
        int createDir(string dir);
        int mkpath(const char *path, mode_t mode);
        static int do_mkdir(const char * path, mode_t mode);
        string GetShortUtilName(string path);
        string GetFullUtilName(string path);
        std::ifstream::pos_type filesize(const char* filename);
        //void getDrives(vector<string> *myvector);
        void getDrives(vector<t_drive *> *myvector);
        char* formatdate(char* str, time_t val);



    private :
        char rutaActual[PATH_MAX]; //Ruta actual que se esta navegando
        void comprobarDirPadre(char *);
        char * getDir(char *); //Obtiene el directorio actual y retorna la ruta
        typedef struct stat Stat;
};

#endif //Dirutil_H
