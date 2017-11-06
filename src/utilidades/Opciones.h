#ifndef OPCIONES_H_
#define OPCIONES_H_
#include <string>

using std::string;

typedef struct {
    bool debug = false;
    unsigned int jugadores = 15;
    unsigned int partidos = 3;
    string logName = "";
    string logLevel = "info";

} Opciones;

#endif //OPCIONES_H_
