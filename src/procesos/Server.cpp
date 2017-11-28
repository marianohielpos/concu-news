#include "Server.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <SIGINT_Handler.h>
#include "../ipcs/Cola.h"
#include "protocol.h"
#include "../utilidades/Logger.h"
#include "MicroServicio.h"


void Server::run() {

    this->initialize();

    this->handleRequests();

    this->terminate();

}

void Server::handleRequests() const {

    SIGINT_Handler s;

    message m;

    while (s.getGracefulQuit() != 1) {

        colaPublica.leer(REQUEST, &m);

        std::stringstream ss;

        ss << "Cliente me envió key: " << m.key
           << " y value: " << m.value;

        Logger::getInstance()->info(ss.str());

        if (m.type == TYPE_SET_CITY || m.type == TYPE_GET_CITY) {
            Logger::getInstance()->info("Enviando mensaje a microservicio de ciudades");

            colaCiudades.escribir(m);

            colaCiudades.leer(RESPONSE, &m);
        } else {
            Logger::getInstance()->info("Enviando mensaje a microservicio de monedas");

            colaMonedas.escribir(m);

            colaMonedas.leer(RESPONSE, &m);
        }

        m.mtype = m.responsePriority;
        m.type = TYPE_SUCCESS;

        Logger::getInstance()->info("Respondiendo mensaje");

        colaPublica.escribir(m);
    }
}

void Server::terminate() const {
    Logger::getInstance()->info("Terminando server");

    int exit;

    Logger::getInstance()->info("Terminando microservicio de cotización de monedas");
    kill(cotizacionDeMonedasPID, SIGINT);
    wait(&exit);

    Logger::getInstance()->info("Terminando microservicio de estado del tiempo");
    kill(estadoDelTiempoPID, SIGINT);
    wait(&exit);

    Logger::getInstance()->info("Terminando colas");
    colaPublica.destruir();
    colaCiudades.destruir();
    colaMonedas.destruir();
}

void Server::initialize() {
    Logger::getInstance()->info("Inicializando server");

    Logger::getInstance()->info("Inicializando servicio de ciudades");

    this->cotizacionDeMonedasPID = fork();

    if (this->cotizacionDeMonedasPID == 0) {

        MicroServicio cotizacionDeMonedas("cotizacion_de_monedas.txt");

        cotizacionDeMonedas.setQueue(&this->colaCiudades);

        cotizacionDeMonedas.run();
    }

    Logger::getInstance()->info("Inicializando servicio de monedas");

    this->estadoDelTiempoPID = fork();

    if (this->estadoDelTiempoPID == 0) {

        MicroServicio estadoDelTiempo("estado_del_tiempo.txt");

        estadoDelTiempo.setQueue(&this->colaMonedas);

        estadoDelTiempo.run();
    }
}
