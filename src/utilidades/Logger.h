#ifndef LOGGER_H_
#define LOGGER_H_


#include "../ipcs/LockFile.h"
#include <string>
#include <iostream>
#include <map>
#include "Opciones.h"

#define DEBUG "DEBUG"
#define INFO "INFO"
#define WARNING "WARNING"
#define ERROR "ERROR"

class Logger {

private:

    static Logger* instance;

    std::map<std::string, int> levels = {
            {DEBUG, 10},
            {INFO, 20},
            {WARNING, 30},
            {ERROR, 40}
    };

    LockFile *lock = NULL;

    bool timestamp = true;

    bool pid = true;

    std::string logLevel = INFO;

    std::string generarMensaje(std::string mensaje, std::string nivel);

    void imprimirMensaje(std::string mensaje, std::string nivel);

    void escribirAArchivo(std::string mensaje);

    void escribirAConsola(std::string mensaje);

    bool cout = false;

public:

    Logger ( std::string nombreArchivo, std::string nivel);

    ~Logger();

    static Logger* getInstance();

    static void setInstance(Opciones opciones);

    static void deleteInstance();

    void info(std::string mensaje);

    void debug(std::string mensaje);

    void warning(std::string mensaje);

    void error(std::string mensaje);

};


#endif //LOGGER_H_
