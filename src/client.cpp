
#include "procesos/Client.h"
#include "utilidades/Logger.h"
#include <iostream>
#include <getopt.h>


Opciones parsearParametros (int argc, char* const argv[]) {
    Opciones opciones;

    int opt;
    while ((opt = getopt(argc, argv, "hdj:s:p:l:k:a:b:c:e:f:g:m:")) != -1) {
        switch (opt) {
            case 'l':
                opciones.logName = std::string(optarg);
                break;
            case 'k':
                opciones.logLevel = std::string(optarg);
                break;
            case 'd':
                opciones.debug = true;
                break;
            case 'h':
                std::cout << "Uso:\n"
                          << "\n"
                          << "cliente [Opciones]\n"
                          << "\n"
                          << "Opciones:\n"
                          << "-l      Archivo de log (default: ninguno, se escribe a stdout)\n"
                          << "-k      Nivel de log (default: info)\n"
                          << std::endl;
                exit(0);

        }
    }
    return opciones;
}



int main(int argc, char* const argv[]) {

    Opciones opciones = parsearParametros(argc, argv);

    Logger::setInstance(opciones.logName, opciones.logLevel);

    Client c;

    c.run();

    return 0;
}
