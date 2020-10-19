#include "Dirutil.h"

/**
*
*/
Dirutil::Dirutil(){
}

/**
*
*/
Dirutil::~Dirutil(){
}


/**
*
*/
void Dirutil::listarDirRecursivo(string dir,  vector <FileProps> * filelist){
    listarDirRecursivo(dir, filelist, "");
}

/**
*
*/
void Dirutil::listarDirRecursivo(string dir,  vector <FileProps> * filelist, string filtro){
    vector <FileProps> *tempfilelist = new vector <FileProps>();
    unsigned int numDirs = 0;

    try{
        Traza::print("Escaneando: " + dir, W_DEBUG);
        numDirs = listarDirSinOrden(dir.c_str(), tempfilelist, filelist, filtro);
        string tempDir = "";

        if (tempfilelist != NULL){
            for (unsigned int i=0; i < numDirs; i++){
                tempDir = dir + FILE_SEPARATOR + tempfilelist->at(i).filename;
                if (tempfilelist->at(i).filetype == TIPODIRECTORIO)
                    listarDirRecursivo(tempDir, filelist, filtro);
            }
            delete tempfilelist;
       }
    } catch(Excepcion &e) {
        Traza::print("Error Dirutil::listarDirRecursivo - " +  string(e.getMessage()), W_ERROR);
        delete tempfilelist;
    }
}

/**
 * 
 * @param strdir
 * @param tempFilelist
 * @param filelistFinal
 * @param filtro
 * @param maxSearchFiles
 * @param maxResults
 * @return 
 */
unsigned int Dirutil::listarDirSinOrdenMaxFiles(const char *strdir, vector <FileProps> * tempFilelist
                                        ,vector <FileProps> * filelistFinal, string filtro, int maxSearchFiles, int maxResults){
    DIR *dp;
    struct dirent *dirp;
    char * concatDir = NULL;
    //const char  tempFileSep[2] = {FILE_SEPARATOR,'\0'};
    unsigned int lenDir = 0;
    int numSearchFiles = 0;
    int numResults = 0;

    try{
        //Miramos a ver si el directorio a explorar tiene una / al final
        if (strdir != NULL){

            int dirlen = sizeof(strdir);
            bool sepDir = false;
            if (dirlen > 0){
                if (strdir[dirlen-1] != FILE_SEPARATOR){
                    dirlen++;
                    sepDir = true;
                }
            }
            if((dp  = opendir(strdir)) == NULL) {
                Traza::print("Error al listar el directorio: " + string(strdir), W_ERROR);
                throw(Excepcion(EFIO));
            } else {
                string extension;
                while ((dirp = readdir(dp)) != NULL && (numSearchFiles < maxSearchFiles || maxSearchFiles == 0)
                        && (numResults < maxResults || maxResults == 0)) {
                    concatDir = new char[dirlen+sizeof(dirp->d_name)+1];//Contamos +1 con el fin de cadena
                    strcpy(concatDir,strdir);
                    if (sepDir){
                        strcat(concatDir,tempFileSep);
                    }
                    strcat(concatDir,dirp->d_name);

                    if (strcmp(dirp->d_name,".") != 0 && strcmp(dirp->d_name,"..") != 0){
                        FileProps propFile;
                        propFile.dir = strdir;
                        propFile.filename = dirp->d_name;

                        Traza::print("listarDirSinOrden. dir: " + propFile.dir, W_PARANOIC);
                        Traza::print("listarDirSinOrden. filename: " + propFile.filename, W_PARANOIC);
                        
                        setFileProperties(&propFile, concatDir);
                        
                        if(isDir(concatDir)){
                            if (tempFilelist != NULL){
                                propFile.filetype = TIPODIRECTORIO;
                                propFile.ico = folder;
                                tempFilelist->push_back(propFile);
                                lenDir++;
                            }
                        } else {
                            extension = getExtension(propFile.filename);
                            Constant::lowerCase(&extension);
                            if (filtro.empty() || filtro.find(extension) != string::npos){
                                propFile.filetype = TIPOFICHERO;
                                propFile.ico = page_white;
                                filelistFinal->push_back(propFile);
                                numResults++;
                            }
                        }
                    }
                    delete concatDir;
                    numSearchFiles++;
                }
                Traza::print("Se han encontrado " + Constant::TipoToStr(lenDir) + " directorios y " + Constant::TipoToStr(numResults) + " ficheros", W_DEBUG);
                closedir(dp);
            }
        }
    } catch(Excepcion &e) {
        Traza::print("Error Dirutil::listarDir - " + string(e.getMessage()), W_ERROR);
        throw(e);
    }
    return lenDir;
}

/**
 * 
 * @param strdir
 * @param tempFilelist
 * @param filelistFinal
 * @param filtro
 * @return 
 */
unsigned int Dirutil::listarDirSinOrden(const char *strdir, vector <FileProps> * tempFilelist
                                        ,vector <FileProps> * filelistFinal, string filtro){
    return listarDirSinOrdenMaxFiles(strdir, tempFilelist, filelistFinal, filtro, 0, 0);
}

/**
 * 
 * @param strdir
 * @param filelist
 * @param filtro
 * @return 
 */
unsigned int Dirutil::listarDir(const char *strdir, listaSimple<FileProps> * filelist, string filtro){
    DIR *dp;
    struct dirent *dirp;
    char * concatDir = NULL;
//    const char  tempFileSep[2] = {FILE_SEPARATOR,'\0'};
    FileProps propFile;
    unsigned int nFiles [2] = {0,0};
    unsigned int totalFiles = 0;
    listaSimple<FileProps> * listaFicheros = new listaSimple<FileProps>();
    listaSimple<FileProps> * listaDirectorios = new listaSimple<FileProps>();
    Traza::print("listarDir: " + Constant::TipoToStr(strdir), W_DEBUG);

    try{
        //Miramos a ver si el directorio a explorar tiene una / al final
        if (strdir != NULL){
            int dirlen = sizeof(strdir);
            bool sepDir = false;
            if (dirlen > 0){
                if (strdir[dirlen-1] != FILE_SEPARATOR){
                    dirlen++;
                    sepDir = true;
                }
            }

            countDir(strdir, nFiles);
            Traza::print("Reservando " + Constant::TipoToStr(nFiles[0]) + " directorios y " + Constant::TipoToStr(nFiles[1]) + " ficheros", W_DEBUG);
            listaDirectorios->reservar(nFiles[0]);
            listaFicheros->reservar(nFiles[1]);
            string extension;

            if((dp  = opendir(strdir)) == NULL) {
                Traza::print("Error al listar el directorio: " + string(strdir), W_ERROR);
                throw(Excepcion(EFIO));
            } else {
                Traza::print("Recorriendo directorios", W_DEBUG);
                while ((dirp = readdir(dp)) != NULL) {
                    concatDir = new char[dirlen+sizeof(dirp->d_name)+1];//Contamos +1 con el fin de cadena
                    strcpy(concatDir,strdir);
                    if (sepDir){
                        strcat(concatDir,tempFileSep);
                    }
                    strcat(concatDir,dirp->d_name);

                    if (strcmp(dirp->d_name,".") != 0){
                        propFile.dir = strdir;
                        propFile.filename = dirp->d_name;
                        
                        setFileProperties(&propFile, concatDir);
                        
                        if(isDir(concatDir)){
                            propFile.filetype = TIPODIRECTORIO;
                            if (strcmp(dirp->d_name,"..") == 0){
                                propFile.ico = bullet_go;
                            } else {
                                propFile.ico = folder;
                            }

                            listaDirectorios->add(propFile);
                        } else {
                            propFile.filetype = TIPOFICHERO;
                            propFile.ico = findIcon(dirp->d_name);
                            extension = getExtension(propFile.filename);
                            Constant::lowerCase(&extension);
                            if (filtro.empty() || filtro.find(extension) != string::npos){
                                listaFicheros->add(propFile);
                            }
                        }
                    }
                    delete concatDir;
                }
                closedir(dp);
                totalFiles = listaDirectorios->getSize() + listaFicheros->getSize();
                //Mostramos los ficheros de forma ordenada
                listaDirectorios->sort();
                listaFicheros->sort();
                //Reservamos espacio en el objeto que almacena los directorios
                if (totalFiles > 0)
                    filelist->reservar(totalFiles);
                //Primero mostramos los directorios
                for (unsigned int i = 0; i < listaDirectorios->getSize(); i++){
                    filelist->add(listaDirectorios->get(i));
                }
                //y luego los ficheros
                for (unsigned int i = 0; i < listaFicheros->getSize(); i++){
                    filelist->add(listaFicheros->get(i));
                }
                Traza::print("Ficheros anyadidos", W_PARANOIC);
            }
        }

    } catch(Excepcion &e) {
        Traza::print("Error Dirutil::listarDir - " + string(e.getMessage()), W_ERROR);
        throw(e);
    }
    delete listaFicheros;
    delete listaDirectorios;
    return totalFiles;
}

/**
* El directorio deberia venir siempre con una / al final
*/
unsigned int Dirutil::listarDir(const char *strdir, listaSimple<FileProps> * filelist){
    return listarDir(strdir,filelist,"");
}

/**
*/
unsigned int Dirutil::countDir(const char *strdir){
    unsigned int nFiles = 0;
    DIR *dir;

    if ((dir = opendir(strdir)) != NULL){
        while (readdir(dir) != NULL) {
            nFiles++;
        }
        closedir(dir);
    }
    Traza::print("countDir. nFiles: " + Constant::TipoToStr(nFiles), W_DEBUG);
    return nFiles;
}


/**
* Lista los ficheros y directorios a partir de un directorio determinado
* Devuelve en la posicion 0 de la variable nFiles el numero de directorios
* y en la posicion 1 el nï¿½mero de ficheros
*/
void Dirutil::countDir(const char *strdir, unsigned int *nFiles){
    DIR *dir;
    struct dirent *dirp;
    string filePath = "";

    if ((dir = opendir(strdir)) != NULL){
        while ((dirp = readdir(dir)) != NULL) {
            filePath = strdir + Constant::getFileSep() + dirp->d_name;
            if (isDir(filePath))
                nFiles[0]++;
            else
                nFiles[1]++;
        }
        closedir(dir);
    }
    Traza::print("countDir: Hay " + Constant::TipoToStr(nFiles[0]) + " directorios y " + Constant::TipoToStr(nFiles[1]) + " ficheros", W_DEBUG);
}

void Dirutil::countDir(const char *strdir, unsigned int *nFiles, vector<string> *files, string filtro){
    DIR *dir;
    struct dirent *dirp;
    string filePath = "";

    if ((dir = opendir(strdir)) != NULL){
        while ((dirp = readdir(dir)) != NULL) {
            filePath = strdir + Constant::getFileSep() + dirp->d_name;
            if (isDir(filePath)){
                if (string(".").compare(dirp->d_name) != 0 && string("..").compare(dirp->d_name) != 0){
                    nFiles[0]++;
                    countDir(filePath.c_str(), nFiles, files, filtro);
                }
            }
            else if (filtro.find(this->getExtension(dirp->d_name)) != string::npos){
                files->push_back(dirp->d_name);
                nFiles[1]++;
            }

        }
        closedir(dir);
    }
    Traza::print("countDir: Hay " + Constant::TipoToStr(nFiles[0]) + " directorios y " + Constant::TipoToStr(nFiles[1]) + " ficheros", W_DEBUG);
}

/**
*Con este metodo comprobamos que el directorio al que tengo que ir es el adecuado
*Convertiremos C:\\Archivos de programa\\.. en
* C:\\
*
*/
void Dirutil::comprobarDirPadre(char * dirDestino)
{
    if (dirDestino != NULL)
    {
        const char tempSep[2] = {FILE_SEPARATOR,'\0'};
        char file[FILENAME_MAX];
        strcpy(file,dirDestino);


        int pos = Constant::contiene(dirDestino,FILE_PARENT);


        if (pos > 0){
            int i = pos;
            char letra = 0;
            bool encontrado = false;

            do{
                i--;
                letra = dirDestino[i];
                if (letra == FILE_SEPARATOR) encontrado = true;

            }while (i > 0 && !encontrado);

            if (encontrado){
                memset(file, '\0',FILENAME_MAX-1);

            }
            for (int j=0;j<i;j++){
                file[j] = dirDestino[j];
            }

            //Este es el caso en el que llegamos a la raiz de la unidad
            //Si no hay ningun separador de unidad lo incluimos
            if (Constant::contiene(file,FILE_SEPARATOR) < 0){
                strcat(file,tempSep);
            }
        }
        memset(dirDestino, '\0',FILENAME_MAX-1);
        strcpy(dirDestino,file);
    }
}

/**
* Devuelve true si se ha hecho el cambio al directorio.
* False si no se ha podido hacer el cambio. P.ejm: Cambio por un fichero
*/
bool Dirutil::changeDirRelative(const char *str){
    char ruta[PATH_MAX];
    strcpy(ruta,this->getDirActual());
    dircat(ruta,str);
    comprobarDirPadre(ruta);

    if(isDir(ruta)){
        return (chdir(ruta) != -1);
    } else {
        return false;
    }
}

/**
* Devuelve true si se ha hecho el cambio al directorio.
* False si no se ha podido hacer el cambio. P.ejm: Cambio por un fichero
*/
bool Dirutil::changeDirAbsolute(const char *str){
    if(isDir(str)){
        return (chdir(str) != -1);
    } else {
        return false;
    }
}

/**
*/
void Dirutil::dircat(char *dir, const char *file){
    const char tempSep[2] = {FILE_SEPARATOR,'\0'};
    int dirlen = sizeof(dir);
    if (dirlen > 0){
        if (dir[dirlen-1] != FILE_SEPARATOR){
            strcat(dir,tempSep);
        }
        strcat(dir,file);
    }
}

/**
* Obtiene el directorio de un fichero
*/
string Dirutil::getFolder(string file){
    if(isDir(file)){
        return file;
    } else {
        size_t found;
        found = file.find_last_of(FILE_SEPARATOR);
        if (found > 0){
            return file.substr(0,found);
        } else {
            return file;
        }
    }
}

/**
* Obtiene el directorio de un fichero
*/
string Dirutil::getFileName(string file){
    if(isDir(file)){
        return file;
    } else {
        size_t found;
        found = file.find_last_of(FILE_SEPARATOR);
        if (found != string::npos){
            return file.substr(found  + 1, file.length() - found - 1);
        } else {
            return file;
        }
    }
}

string Dirutil::getFileNameNoExt(string file){
    if(isDir(file)){
        return file;
    } else {
        size_t found, foundExt;
        found = file.find_last_of(FILE_SEPARATOR);
        foundExt = file.find_last_of(".");

        if (found == string::npos){
            found = 0;
        }

        if (found > 0 && foundExt > found){
            string name = file.substr(found  + 1, foundExt - found - 1);
            Traza::print("getFileNameNoExt: name" + name, W_PARANOIC);
            return name;
        } else if (foundExt > found){
            string name = file.substr(0, foundExt);
            Traza::print("getFileNameNoExt: name" + name, W_PARANOIC);
            return name;
        } else {
            return file;
        }
    }
}

/**
* Obtiene la extension del fichero
*/
string Dirutil::getExtension(string file){
    string ext = "";
    if (!file.empty()){
         if(!isDir(file)){
            size_t found;
            found = file.find_last_of(".");
            if (found > 0 && found < file.length()){
                ext = file.substr(found);
            }
        }
        Constant::lowerCase(&ext);
    }

    return ext;
}

/**
*/
char * Dirutil::getDir(char *buffer){
    memset(buffer, '\0',FILENAME_MAX-1);
    return getcwd(buffer, PATH_MAX);
}

/**
*/
char * Dirutil::getDirActual(){
    return getDir(rutaActual);
}

string Dirutil::GetShortUtilName(string path){
    string retorno = "";
    #ifdef WIN
//    cout << "rutaSqlPlus: " << rutaSqlPlus << endl;
    int length = path.length();
    char* buffer = new char[length];
    memset (buffer,'\0',length);
//    cout << rutaSqlPlus << endl;
    length = GetShortPathName(path.c_str(), buffer, length);
//    cout << "buffer: " << buffer << endl;
    if (length != 0){
        retorno = buffer;
    }
    delete [] buffer;
    #else
        retorno = path;
    #endif
    return retorno;
}

string Dirutil::GetFullUtilName(string path){
    string retorno = "";
    #ifdef WIN
    int length = path.length();
    char* buffer = new char[length];
    memset (buffer,'\0',length);
    char** lppPart={NULL};
    length = GetFullPathName(path.c_str(), length, buffer,lppPart);
    //length = GetLongPathName(path.c_str(), buffer, length);
    if (length != 0){
        retorno = buffer;
    }
    delete [] buffer;
    #else
        retorno = path;
    #endif
    return retorno;
}

/**
*
*/
int Dirutil::findIcon(const char *filename){

    char ext[5] = {' ',' ',' ',' ','\0'};
    int len = 0;

    if (filename != NULL){
        len = strlen(filename);
        if ( len > 4){
            ext[3] = filename[len-1];
            ext[2] = filename[len-2];
            ext[1] = filename[len-3];
            ext[0] = filename[len-4];
        }
    }
    string data = ext;
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);

    if (data.find(".txt") != string::npos || data.find(".inf") != string::npos){
        return page_white_text;
    } else if (data.find(".gpu") != string::npos || data.find(".gpe") != string::npos
        || data.find(".exe") != string::npos || data.find(".bat") != string::npos
        || data.find(".com") != string::npos){
        return page_white_gear;
    } else if (data.find(".gz") != string::npos || data.find(".z") != string::npos
        || data.find(".tar") != string::npos || data.find(".zip") != string::npos
        || data.find(".rar") != string::npos){
	    return page_white_compressed;
	} else if (data.find(".bmp") != string::npos || data.find(".jpg") != string::npos
        || data.find(".jpeg") != string::npos || data.find(".png") != string::npos
        || data.find(".gif") != string::npos ){
        return page_white_picture;
    } else if (data.find(".bin") != string::npos){
        return page_white_zip;
    } else {
        return page_white;
    }

}

/**
*
*/
bool Dirutil::borrarArchivo(string ruta){

    if (isDir(ruta))
        return false;
    else {
        if (existe(ruta))
            return (remove(ruta.c_str()) != 0) ? false : true;
        else
            return false;
    }
}


/**
* Comprueba si existe el directorio o fichero pasado por parï¿½metro
*/
bool Dirutil::existe(string ruta){
    if(isDir(ruta)){
        return true;
    } else {
        FILE *archivo = fopen(ruta.c_str(), "r");
        if (archivo != NULL) {
            fclose(archivo);
            return true; //TRUE
        } else {
            return false; //FALSE
        }
    }
}

bool Dirutil::setFileProperties(FileProps *propFile, string ruta){
    boolean ret = true;
    
    struct stat info;
    stat(ruta.c_str(), &info);
    
    if(S_ISDIR(info.st_mode)){
        propFile->filetype = TIPODIRECTORIO;
        propFile->extension = STR_DIR_EXT;
    } else {
        propFile->filetype = TIPOFICHERO;
        propFile->fileSize = (size_t)info.st_size;
        propFile->extension = getExtension(ruta);
    }
    char mbstr[36];
    propFile->creationTime = formatdate(mbstr, info.st_ctime);
    propFile->modificationTime = formatdate(mbstr, info.st_mtime);
    propFile->iCreationTime = time(&info.st_ctime);
    propFile->iModificationTime = time(&info.st_mtime);
    
//    struct tm * timeinfo = localtime(&info.st_ctime); // or gmtime() depending on what you want
//    printf("File time and date: %s", asctime(timeinfo));
    
//    std::cout << formatdate(mbstr, info.st_ctime) << '\n';
    
    return ret;
}

/**
 * 
 * @param str
 * @param val
 * @return 
 */
char* Dirutil::formatdate(char* str, time_t val){
    int tam = 36;
    strftime(str, tam, "%d/%m/%y %H:%M", localtime(&val));
    return str;
}

/**
 * 
 * @param ruta
 * @return 
 */
bool Dirutil::isDir(string ruta){
    struct stat info;
    stat(ruta.c_str(), &info);

    if(S_ISDIR(info.st_mode)){
        return true;
    } else{
        return false;
    }

}

/**
* pathname - Cadena de caracteres. Deberia tener el mismo tamanyo que len
* len - tamanyo de la cadena de caracteres
*/
void Dirutil::getProgramDir(char *pathname, int len){
    //char pathname[PATH_MAX];
    #ifdef WIN
    HMODULE hMe=GetModuleHandle(NULL);
    GetModuleFileName(hMe,pathname,len);
    #endif
}

/**
*
*/
void Dirutil::copyFile(string srcFile, string dstFile){
     std::ifstream  src(srcFile.c_str(), std::ios::binary);
     std::ofstream  dst(dstFile.c_str(), std::ios::binary);
     dst << src.rdbuf();
}

/**
*
*/
int Dirutil::createDir(string dir){
    if (!existe(dir)) {
        #ifdef WIN
            return mkdir(dir.c_str());
        #else
            return mkdir(dir.c_str(), 0777);
        #endif
    } else {
        return 0;
    }
}


int Dirutil::do_mkdir(const char *path, mode_t mode)
{
    Stat            st;
    int             status = 0;

    if (stat(path, &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        #ifdef WIN
        if (mkdir(path) != 0 && errno != EEXIST)
        #else
        if (mkdir(path, mode) != 0)
        #endif
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        status = -1;
    }

    return(status);
}

/**
** mkpath - ensure all directories in path exist
** Algorithm takes the pessimistic view and works top-down to ensure
** each directory in path exists, rather than optimistically creating
** the last element and working backwards.
*/
int Dirutil::mkpath(const char *path, mode_t mode)
{
    char           *pp;
    char           *sp;
    int             status;
    //char           *copypath = STRDUP(path);
    char           *copypath = new char[strlen(path)+1];
    strcpy(copypath, path);

    status = 0;
    pp = copypath;
    while (status == 0 && (sp = strchr(pp, FILE_SEPARATOR)) != 0)
    {
        if (sp != pp)
        {
            /* Neither root nor double slash in path */
            *sp = '\0';
            status = do_mkdir(copypath, mode);
            *sp = FILE_SEPARATOR;
        }

        pp = sp + 1;
    }
    if (status == 0)
        status = do_mkdir(path, mode);
    delete copypath;
    return (status);
}

/**
*
*/
std::ifstream::pos_type Dirutil::filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

/**
*
*/
void Dirutil::getDrives(vector<t_drive *> *myvector){
    #ifdef WIN
    uint32_t mydrives = 100;// buffer length
    char lpBuffer[100];// buffer for drive string storage
    string driveStr;
    GetLogicalDriveStrings( mydrives, lpBuffer);

    if (myvector != NULL)
        myvector->clear();

    bool salir = false;

    for(int i = 0; i<100 && !salir; i++){
        t_drive *myDrive = new t_drive();

        if (lpBuffer[i] == 0 && driveStr.empty()){
            salir = true;
        } else if (lpBuffer[i] == 0 && !driveStr.empty()){
            myDrive->drive = driveStr;
            myDrive->driveType = GetDriveType(driveStr.c_str());

            char Label[MAX_PATH];
            uint32_t dwDontNeedThis;
            if ( !GetVolumeInformation ( myDrive->drive.c_str(),
                                         Label,
                                         sizeof ( Label ),
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         0 ))
            {
                strcpy(Label,"");
            }
            myDrive->label = Label;


            switch (myDrive->driveType){
                case DRIVE_FIXED:
                    myDrive->driveTypeString = "HDD";
                    myDrive->ico = drive;
                    break;

                case DRIVE_CDROM:
                    myDrive->driveTypeString = "CD/DVD";
                    myDrive->ico = drive_cd;
                    break;

                case DRIVE_REMOVABLE:
                    myDrive->driveTypeString = "USB";
                    myDrive->ico = drive_disk;
                    break;

                case DRIVE_REMOTE:
                    myDrive->driveTypeString = "Red";
                    myDrive->ico = drive_network;
                    break;

                default:
                    myDrive->driveTypeString = "";
                    myDrive->ico = drive_disk;
                    break;
            }

            myvector->push_back(myDrive);
            driveStr = "";
        } else if (lpBuffer[i] >= 32 && lpBuffer[i] <= 122){
            driveStr += lpBuffer[i];
        }
    }
#else
     t_drive *myDrive = new t_drive();
     myDrive->driveTypeString = "HDD";
     myDrive->ico = drive;
     myDrive->label = "Root";
     myDrive->drive = "/";
     myDrive->driveType = DRIVE_FIXED;
     
     myvector->push_back(myDrive);
#endif
}
/**
*
*/
void Dirutil::borrarDir(string path)
{
        struct dirent *entry = NULL;
        DIR *dir = NULL;
        dir = opendir(path.c_str());
        while( (entry = readdir(dir)) )
        {
            DIR *sub_dir = NULL;
            FILE *file = NULL;
            char abs_path[MAX_PATH] = {0};
            //if(*(entry->d_name) != '.' && *(entry->d_name) != '..')
            if (strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0)
            {
                sprintf(abs_path, "%s/%s", path.c_str(), entry->d_name);
                if( (sub_dir = opendir(abs_path)))
                {
                    closedir(sub_dir);
                    borrarDir(abs_path);
                }
                else
                {
                    if( (file = fopen(abs_path, "r")))
                    {
                        fclose(file);
                        remove(abs_path);
                    }
                }
            }
        }
        rmdir(path.c_str());
}
