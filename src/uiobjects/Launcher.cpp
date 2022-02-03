#ifdef WIN
    #include <winsock2.h>
    #include <windows.h>
#else
    #define MAX_PATH 255
    #define DRIVE_FIXED 0 
    #define DRIVE_CDROM 1
#endif // WIN

#include "Launcher.h"

#define LEER		0
#define ESCRIBIR	1
#define MAXTUBERIALEN 2000
#define BUFFER_SIZE 4096

/**
*
*/
Launcher::Launcher(){
    #ifdef UNIX
    desc_fich = open (RUTA_TRAZA_PROC, O_CREAT|O_APPEND|O_WRONLY, 0);
    dup2 (desc_fich, 1);		/* Redirige la salida normal */
    dup2 (desc_fich, 2);		/* Redirige la salida de error */
    #endif
}

/**
*
*/
Launcher::~Launcher(){
    Traza::print("Launcher::~Launcher", W_DEBUG);
    #ifdef UNIX
    log( "~Launcher(): Cerrando fichero de trazas");
    close (desc_fich);
    #endif
}

/**
* Descomprime el fichero .zip pasado por parametro y devuelve el nombre del fichero extraido
*/
string Launcher::descomprimirZIP(string filename){
    string filenameReturn = filename;
    log("descomprimirZIP");
    if (filename.find(".zip") != string::npos || filename.find(".ZIP") != string::npos){
        //Descomprimir el fichero
        UnzipTool *unzipTool = new UnzipTool();
        unzippedFileInfo salida = unzipTool->descomprimirZip(filename.c_str());
        delete unzipTool;
        filenameReturn = salida.rutaEscritura;
    }
    log("descomprimirZIP FIN");
    return filenameReturn;
}

/**
* Realiza un lanzamiento del programa que se encontro al parsearArgumentos
*/
bool Launcher::lanzarProgramaUNIXFork(FileLaunch *emulInfo){

    string ejecutable;
    string param;
    string rom;

    ejecutable = emulInfo->rutaexe;
    if (emulInfo->rutaexe.length() > 0 && emulInfo->fileexe.length() > 0){
        ejecutable += (emulInfo->rutaexe.at(emulInfo->rutaexe.length()-1) == FILE_SEPARATOR
               || emulInfo->fileexe.at(emulInfo->fileexe.length()-1) == FILE_SEPARATOR ? "" : tempFileSep);
    }
    ejecutable += emulInfo->fileexe;

    param = emulInfo->parmsexe;

    rom = emulInfo->rutaroms;
    if (emulInfo->rutaroms.length() > 0 && emulInfo->nombrerom.length() > 0){
            rom += (emulInfo->rutaroms.at(emulInfo->rutaroms.length()-1) == FILE_SEPARATOR
               || emulInfo->nombrerom.at(emulInfo->nombrerom.length()-1) == FILE_SEPARATOR ? "" : tempFileSep);
    }
    rom += emulInfo->nombrerom;


    #ifdef UNIX

        int extPos = rom.rfind(".");
        bool isZipped = false;
        if (extPos > 0){
            string extension = rom.substr(extPos);
            Constant::lowerCase(&extension);
            log( "extension: " + extension);
            if (extension.compare(".zip") == 0){
                isZipped = true;
            }
        }

        log( "*************Launcher::lanzarProgramaUNIX*************");
        string buffer = "";
        if (emulInfo->descomprimir){
            buffer = descomprimirZIP(rom);
        } else {
            buffer = rom;
        }

        Dirutil dir;
        dir.changeDirAbsolute(emulInfo->rutaexe.c_str());
        Executable execCmd = rutaEspecial(ejecutable, param, buffer);
        bool errorLaunch = false;
        
        string comando = execCmd.comandoFinal;
        Traza::print( "Vamos a lanzar el comando: " + comando, W_INFO);
        
        int ret = 0;
        if (Constant::getExecMethod() == launch_spawn || Constant::getExecMethod() == launch_create_process){
            switch(fork()){
                case -1 : // Error
                        errorLaunch = true;
                        break;
                case 0 :
                        // Call one of the exec -- personally I prefer execlp
                        if (execCmd.filenameinparms){
                            const char* argv[] = { execCmd.ejecutable.c_str(), execCmd.param.c_str(), NULL };
                            ret = execl(argv[0], argv[0], argv[1], NULL);
                        } else {
                            const char* argv[] = { execCmd.ejecutable.c_str(), execCmd.param.c_str(), execCmd.filerompath.c_str(), 0 };
                            ret = execl(argv[0], argv[0], argv[1], NULL);
                        }
                        break;
                default :
                        // Do what you want
                        break;  
            }
        } else if (Constant::getExecMethod() == launch_system ){
            if (system(comando.c_str()) != 0){
                errorLaunch = true;
                Traza::print( "Error al lanzar el comando", W_ERROR);
            }
        }
        
        if(errorLaunch){
            return false;
        } else {
            if (emulInfo->descomprimir && isZipped){
                Traza::print( "Borrando el fichero: " + buffer, W_DEBUG);
                deleteUnzipedRom(buffer);
            }
            return true;
        }
        
    #elif WIN
        Traza::print( "*************Launcher::lanzarProgramaUNIX - En Windows*************", W_DEBUG);
        string buffer = "";

        int extPos = rom.rfind(".");
        bool isZipped = false;
        if (extPos > 0){
            string extension = rom.substr(extPos);
            Constant::lowerCase(&extension);
            Traza::print( "extension: " + extension, W_DEBUG);
            if (extension.compare(".zip") == 0){
                isZipped = true;
            }
        }

        if (emulInfo->descomprimir){
            buffer = descomprimirZIP(rom);
        } else {
            buffer = rom;
        }

        Traza::print("Launcher::launch. Lanzando ejecutable: " + ejecutable, W_INFO);
        Traza::print("Launcher::launch. Lanzando parmsexe: " + param, W_INFO);
        Traza::print("Launcher::launch. Lanzando rom: " + buffer, W_INFO);

        Dirutil dir;
        dir.changeDirAbsolute(emulInfo->rutaexe.c_str());
        Executable execCmd = rutaEspecial(ejecutable, param, buffer);
        bool errorLaunch = false;

        /*
        P_OVERLAY 	Overlays parent process with child, which destroys the parent. This has the same effect as the exec* functions.
        P_WAIT 	Suspends parent process until the child process has finished executing (synchronous spawn).
        P_NOWAIT, P_NOWAITO 	Continues to execute calling process concurrently with new process (asynchronous spawn).
        P_DETACH 	the child is run in background without access to the console or keyboard. Calls to _cwait upon the new process will fail (asynchronous spawn)
        */
        if (Constant::getExecMethod() == launch_spawn){
            int pid = -1;
            if (execCmd.filenameinparms){
                char* argv[] = { (char *)execCmd.ejecutable.c_str(),(char *)execCmd.param.c_str(), 0 };
                pid = spawnv(P_WAIT, argv[0], argv);
            } else {
                char* argv[] = { (char *)execCmd.ejecutable.c_str(),(char *) execCmd.param.c_str(), (char *)execCmd.filerompath.c_str(), 0 };
                pid = spawnv(P_WAIT, argv[0], argv);
            }
            if (pid == -1){
                Traza::print("Launcher::launch. error" + string(strerror(errno)), W_ERROR);
                errorLaunch = true;
            }
        } else if (Constant::getExecMethod() == launch_system){
            string comando = execCmd.comandoFinal;
            Traza::print( "Vamos a lanzar el comando: " + comando, W_INFO);
            if (system(comando.c_str()) != 0){
                errorLaunch = true;
                Traza::print( "Error al lanzar el comando", W_ERROR);
            }
        } else if (Constant::getExecMethod() == launch_create_process){
            errorLaunch = createProcess(execCmd);
        }

        if(errorLaunch){
            return false;
        } else {
            if (emulInfo->descomprimir && isZipped){
                Traza::print( "Borrando el fichero: " + buffer, W_DEBUG);
                deleteUnzipedRom(buffer);
            }
            return true;
        }

    #endif
    // executed only by parent
    Traza::print("lanzarProgramaUNIX Fin", W_DEBUG);
    return true;
}

#ifdef WIN
/**
*
*/
bool Launcher::createProcess(Executable execCMD){

    Dirutil dir;
    string parm = execCMD.param;
    if (!execCMD.filenameinparms && !execCMD.filerompath.empty()){
        parm += " " + execCMD.filerompath;
    }
    
    string command = dir.getFileNameNoExt(execCMD.ejecutable) + " " +  parm;

    char *AppName = new char[execCMD.ejecutable.length() + 1];
    char *Command = new char[execCMD.comandoFinal.length() + 1];
    strcpy(AppName, execCMD.ejecutable.c_str());
    strcpy(Command, command.c_str());
    bool salida = false;

    Traza::print("AppName: " + execCMD.ejecutable, W_DEBUG);
    Traza::print("Command: " + command, W_DEBUG);;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    string log = Constant::getAppDir() + tempFileSep + "out.log";

    HANDLE h = CreateFile(log.c_str(),
        FILE_WRITE_DATA,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        &sa,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL );

    STARTUPINFO si = {sizeof(STARTUPINFO), 0};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
//    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
//    si.hStdOutput =  GetStdHandle(STD_OUTPUT_HANDLE);
//    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdInput = NULL;
    si.hStdOutput =  h;
    si.hStdError = h;

    si.wShowWindow = SW_HIDE;
    DWORD flags = CREATE_NO_WINDOW;
    PROCESS_INFORMATION pi = {0};

    bool res = CreateProcess(AppName, Command,  NULL, NULL, TRUE, flags, NULL, NULL, &si, &pi);
    if (res) {
        Traza::print("Esperando a que termine el proceso", W_DEBUG);
        DWORD exitCode;
        // Successfully created the process.  Wait for it to finish.
        WaitForSingleObject( pi.hProcess, INFINITE );
        Traza::print("Comando terminado", W_DEBUG);
        // Read the message from the child process
        Fileio file;
        file.loadFromFile(log);
        childStdout = file.getFile();
        // Get the exit code.
        res = GetExitCodeProcess(pi.hProcess, &exitCode);
        Traza::print("exitCode", exitCode, W_DEBUG);

        salida = true;
    } else {
        cerr<<"error..." << GetLastError() <<endl;
        salida = false;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(h);

    return salida;
}

/**
*
*/
void Launcher::DisplayError(LPCTSTR errorDesc, DWORD errorCode)
{
    TCHAR errorMessage[1024] = TEXT("");

    DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM
                | FORMAT_MESSAGE_IGNORE_INSERTS
                | FORMAT_MESSAGE_MAX_WIDTH_MASK;

    FormatMessage( flags,
                   NULL,
                   errorCode,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   errorMessage,
                   sizeof(errorMessage)/sizeof(TCHAR),
                   NULL );

#ifdef _UNICODE
    wcerr << L"Error : "   << errorDesc    << endl;
    wcerr << L"Code    = " << errorCode    << endl;
    wcerr << L"Message = " << errorMessage << endl;
#else
    cerr << "Error : "   << errorDesc    << endl;
    cerr << "Code    = " << errorCode    << endl;
    cerr << "Message = " << errorMessage << endl;
#endif
}
#endif

/**
*
*/
Executable Launcher::rutaEspecial(string ejecutable, string param, string filerompath){
    Executable cmdExec;
    cmdExec.filenameinparms = false;
    Dirutil dir;

    if (param.find("%") != std::string::npos){

        if (param.find("%ROMNAME%") !=std::string::npos || param.find("%ROMNAMEXT%") !=std::string::npos
            || param.find("%ROMFULLPATH%") !=std::string::npos){
            cmdExec.filenameinparms = true;
        }

        param = Constant::replaceAll(param, "%ROMPATH%", dir.getFolder(filerompath));
        param = Constant::replaceAll(param, "%ROMNAME%", dir.getFileNameNoExt(filerompath));
        param = Constant::replaceAll(param, "%ROMNAMEXT%", dir.getFileName(filerompath));
        param = Constant::replaceAll(param, "%ROMFULLPATH%", filerompath);

        if (cmdExec.filenameinparms){
            cmdExec.comandoFinal = ejecutable + " " + param;
        } else {
            cmdExec.comandoFinal = ejecutable + " " + param + " " + filerompath;
        }
    } else {
        if (filerompath.empty())
            filerompath = "";
        else
            filerompath = "\"" + filerompath + "\"";

        cmdExec.comandoFinal = ejecutable + " " + param + " " + filerompath ;
    }
    Traza::print("Launcher::launch. comandoFinal: " + cmdExec.comandoFinal, W_DEBUG);

    cmdExec.ejecutable = ejecutable;
    cmdExec.param = param;
    cmdExec.filerompath = filerompath;

    return cmdExec;
}

/**
*
*/
void Launcher::log(string msg){
    cout << Constant::fecha() << string(" - ") << msg << endl;
}

/**
*
*/
void Launcher::log(string ruta, string msg){
    ofstream myfile (ruta.c_str(), fstream::out | fstream::app);
    if (myfile.is_open()){
        myfile << Constant::fecha() << string(" - ") << msg << VBCRLF;
        myfile.close();
    }
}

/**
*
*/
void Launcher::deleteUnzipedRom(string romfile){
    if (romfile.compare("") != 0 && romfile.find(".zip") == string::npos && romfile.find(".ZIP") == string::npos){
        Dirutil *dirutil=new Dirutil();
        //dirutil->borrar(romfile);
        if (dirutil->borrarArchivo(romfile))
            log( "deleteUnzipedRom Fichero eliminado: " + romfile);
        else
            log( "No se ha encontrado el fichero a eliminar: " + romfile);

        delete dirutil;
    }
}

/**
*
*/
void Launcher::writeToFile(string ruta, string msg){
    FILE *fout = fopen(ruta.c_str(),"at+");
    msg = msg + VBCRLF;

    fwrite(msg.c_str(), sizeof(char) , msg.length(), fout);

    if (fout != NULL){
        fclose(fout);
        fout = NULL;
    }
}


/**
*
*/
std::string Launcher::getErrMsg(int errnum){

        switch ( errnum ) {

    #ifdef EACCES
            case EACCES :
            {
                return "EACCES Permission denied";
            }
    #endif

    #ifdef EPERM
            case EPERM :
            {
                return "EPERM Not super-user";
            }
    #endif

    #ifdef E2BIG
            case E2BIG :
            {
                return "E2BIG Arg list too long";
            }
    #endif

    #ifdef ENOEXEC
            case ENOEXEC :
            {
                return "ENOEXEC Exec format error";
            }
    #endif

    #ifdef EFAULT
            case EFAULT :
            {
                return "EFAULT Bad address";
            }
    #endif

    #ifdef ENAMETOOLONG
            case ENAMETOOLONG :
            {
                return "ENAMETOOLONG path name is too long     ";
            }
    #endif

    #ifdef ENOENT
            case ENOENT :
            {
                return "ENOENT No such file or directory";
            }
    #endif

    #ifdef ENOMEM
            case ENOMEM :
            {
                return "ENOMEM Not enough core";
            }
    #endif

    #ifdef ENOTDIR
            case ENOTDIR :
            {
                return "ENOTDIR Not a directory";
            }
    #endif

    #ifdef ELOOP
            case ELOOP :
            {
                return "ELOOP Too many symbolic links";
            }
    #endif

    #ifdef ETXTBSY
            case ETXTBSY :
            {
                return "ETXTBSY Text file busy";
            }
    #endif

    #ifdef EIO
            case EIO :
            {
                return "EIO I/O error";
            }
    #endif

    #ifdef ENFILE
            case ENFILE :
            {
                return "ENFILE Too many open files in system";
            }
    #endif

    #ifdef EINVAL
            case EINVAL :
            {
                return "EINVAL Invalid argument";
            }
    #endif

    #ifdef EISDIR
            case EISDIR :
            {
                return "EISDIR Is a directory";
            }
    #endif

    #ifdef ELIBBAD
            case ELIBBAD :
            {
                return "ELIBBAD Accessing a corrupted shared lib";
            }
    #endif

            default :
            {
                std::string errorMsg(strerror(errnum));
                if ( errnum ) return errorMsg;
                else return "";
            }
         }
}

//bool Launcher::lanzarProgramaUNIXFork(FileLaunch *emulInfo){
//
//    string ejecutable = emulInfo->rutaexe + FILE_SEPARATOR + emulInfo->fileexe;
//    string param = emulInfo->parmsexe;
//    string rom = emulInfo->rutaroms + FILE_SEPARATOR + emulInfo->nombrerom;
//
//    #ifdef UNIX
//        log( "*************Launcher::lanzarProgramaUNIX*************");
//        /* Buffer para escribir o leer caracteres de la tuberia */
//        string buffer = "";
//
//        if (emulInfo->descomprimir){
//            buffer = descomprimirZIP(rom);
//        } else {
//            buffer = rom;
//        }
//
//        char *cEjecutable = new char(ejecutable.length()+1);
//        char *cParam = new char(param.length()+1);
//        char *cRom = new char(buffer.length()+1);
//        strcpy(cEjecutable,ejecutable.c_str());
//        strcpy(cParam,param.c_str());
//        strcpy(cRom, buffer.c_str());
//
//
//        if (ejecutable.compare("") != 0){
//            int child_status;
//
//            /* Se crea el proceso hijo. En alg�n sitio dentro del fork(), nuestro
//             * programa se duplica en dos procesos. Cada proceso obtendr� una salida
//             * distinta. */
//            pid_t pID = vfork(); //vfork - create a child process and block parent
//
//            /* fork() devuelve 0 al proceso hijo.*/
//            if (pID == 0){
//                // Code only executed by child process
//                char* argv[] = { cEjecutable, cParam, cRom, NULL };
//                int execReturn = execv(argv[0], argv);
//
//                //Aqui no llegamos a no ser que vaya algo mal en la ejecucion
//                if(execReturn == -1){
//                    log(RUTA_TRAZA_HIJO, "ERROR al ejecutar execv: " + getErrMsg(errno));
//                     _exit(-1);
//                }
//                _exit(0); //_exit - - terminate the current process
//            }
//            /* Si fork() devuelve -1, es que hay un error y no se ha podido crear el
//             * proceso hijo. */
//            else if (pID < 0){            // failed to fork
//                cerr << "Failed to fork" << endl;
//                log( "lanzarProgramaUNIX Failed to fork");
//                exit(1);
//                 // Throw exception
//            }
//            /* fork() devuelve un n�mero positivo al padre. Este n�mero es el id del
//             * hijo. */
//            else {   // parent
//
//                /* This is run by the parent.  Wait for the child
//                to terminate. */
//                log("Padre: Esperando a la muerte del hijo");
//                waitpid(pID, &child_status, 0);
//
//                /* Comprueba la salida del hijo */
//                if (WIFEXITED(child_status) != 0){
//                    log("Padre: Proceso Hijo terminado. Recibido: " + buffer);
////                    deleteUnzipedRom(buffer);
//                } else {
//                    log("Padre: ERROR EN LA SALIDA DEL HIJO");
//                }
//            }
//        } else {
//             log( "No se ha encontrado ningun programa que lanzar");
//        }
//
//    #elif WIN
//        log( "*************Launcher::lanzarProgramaUNIX - En Windows*************");
//        string buffer = "";
//
//        if (emulInfo->descomprimir){
//            buffer = descomprimirZIP(rom);
//        } else {
//            buffer = rom;
//        }
//        string filerompath = "\"" + buffer + "\"";
//
//        Traza::print("Launcher::launch. Lanzando ejecutable: " + ejecutable, W_DEBUG);
//        Traza::print("Launcher::launch. Lanzando parmsexe: " + param, W_DEBUG);
//        Traza::print("Launcher::launch. Lanzando rom: " + filerompath, W_DEBUG);
//
//        /*
//        P_OVERLAY 	Overlays parent process with child, which destroys the parent. This has the same effect as the exec* functions.
//        P_WAIT 	Suspends parent process until the child process has finished executing (synchronous spawn).
//        P_NOWAIT, P_NOWAITO 	Continues to execute calling process concurrently with new process (asynchronous spawn).
//        P_DETACH 	the child is run in background without access to the console or keyboard. Calls to _cwait upon the new process will fail (asynchronous spawn)
//        */
//        const char* argv[] = { ejecutable.c_str(), param.c_str(), filerompath.c_str(), 0 };
//        int pid = spawnv(P_WAIT, argv[0], argv);
//        if (pid == -1) Traza::print("Launcher::launch. error" + string(strerror(errno)), W_DEBUG);
//    #endif
//    // executed only by parent
//    log("lanzarProgramaUNIX Fin");
//    return true;
//}
