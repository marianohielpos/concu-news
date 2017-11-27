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

    std::cout << "Cliente me envió key: " << m.key
              << " y value: " << m.value << std::endl;

    if (m.type == TYPE_SET_CITY || m.type == TYPE_GET_CITY) {
        this->colaCiudades.escribir(m);

        this->colaCiudades.leer(RESPONSE,&m);
    } else {
        this->colaMonedas.escribir(m);

        this->colaMonedas.leer(RESPONSE,&m);
    }

    m.mtype = m.responsePriority;
    m.type = TYPE_SUCCESS;

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

        cotizacionDeMonedas.setQueue(&this->colaCiudades);

        cotizacionDeMonedas.run();
    }

    Logger::getInstance()->info("Inicializando servicio de monedas");

    pid_t estadoDelTiempoPID = fork();

    if (estadoDelTiempoPID == 0) {

        MicroServicio estadoDelTiempo("estado_del_tiempo.txt");

        estadoDelTiempo.setQueue(&this->colaMonedas);

        estadoDelTiempo.run();
    }
}
