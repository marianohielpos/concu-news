#include "Server.h"

#include <iostream>
#include <unistd.h>
#include "../ipcs/Cola.h"
#include "protocol.h"
#include "../utilidades/Logger.h"
#include "MicroServicio.h"


void Server::run() {

    initialize();

    message m;

    this->colaPublica.leer(REQUEST, &m);

    std::cout << "Cliente me envió key: " << m.key << std::endl;

    m.mtype = RESPONSE;
    std::string value("respuestaza");
    strcpy(m.value, value.c_str());
    this->colaPublica.escribir(m);

    std::cout << "Terminandome! " << std::endl;

    this->colaPublica.destruir();
}

void Server::initialize() const {
    Logger::getInstance()->info("Inicializando server");

    Logger::getInstance()->info("Inicializando servicio de ciudades");

    pid_t cotizacionDeMonedasPID = fork();

    if (cotizacionDeMonedasPID == 0) {

        MicroServicio cotizacionDeMonedas("cotizacion_de_monedas.txt");

        cotizacionDeMonedas.setQueue(&this->cotizacionDeMonedasCola);

        cotizacionDeMonedas.run();
    }

    Logger::getInstance()->info("Inicializando servicio de monedas");

    pid_t estadoDelTiempoPID = fork();

    if (estadoDelTiempoPID == 0) {

        MicroServicio estadoDelTiempo("estado_del_tiempo.txt");

        estadoDelTiempo.setQueue(&this->estadoDeTiempoCola);

        estadoDelTiempo.run();
    }
}
