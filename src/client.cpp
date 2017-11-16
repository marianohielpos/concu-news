
#include "procesos/Client.h"
#include "utilidades/Logger.h"
#include <iostream>
#include <getopt.h>


Opciones parsearParametros (int argc, char* const argv[]) {
    Opciones opciones;

    int opt;
    int mode = 0;
    while ((opt = getopt(argc, argv, "hdacm:l:n:k:v:")) != -1) {
        switch (opt) {
            case 'l':
                opciones.logName = std::string(optarg);
                break;
            case 'n':
                opciones.logLevel = std::string(optarg);
                break;
            case 'd':
                opciones.debug = true;
                break;
            case 'a':
                opciones.admin = true;
                break;
            case 'c':
                mode++;
                opciones.mode = CLIENT_MODE_CITY;
                break;
            case 'm':
                mode++;
                opciones.mode = CLIENT_MODE_CURRENCY;
                break;
            case 'k':
                opciones.key = std::string(optarg);
                break;
            case 'v':
                opciones.value = std::string(optarg);
                break;
            case 'h':
                std::cout << "Uso:\n"
                          << "\n"
                          << "cliente [Opciones]\n"
                          << "\n"
                          << "Opciones:\n"
                          << "-l      Archivo de log (default: ninguno, se escribe a stdout)\n"
                          << "-n      Nivel de log (default: info)\n"
                          << "\n"
                          << "-a      Para hacer una actualización como admin\n"
                          << "-c/-m   Para hacer consulta/actualización sobre ciudad/moneda\n"
                          << "\n"
                          << "-k      La clave a consultar/setear.\n"
                          << "        Por ejemplo, si el modo es -c (ciudad), la clave puede ser \"Londres\"\n"
                          << "-v      El valor a guardar en la clave especificada.\n"
                          << "        Solo permitido en consulta de admin."
                          << std::endl;
                exit(0);

        }
    }

    if (mode != 1) {
       std::cout << "Debe especificar parámetro -c para hacer consulta "
        << "sobre el tiempo de una ciudad o -m para la cotización de "
        << "una moneda!" << std::endl;
       exit(1);
    }
    return opciones;
}



int main(int argc, char* const argv[]) {

    Opciones opciones = parsearParametros(argc, argv);

    Logger::setInstance(opciones.logName, opciones.logLevel);

    Client c(opciones);

    c.run();

    return 0;
}
