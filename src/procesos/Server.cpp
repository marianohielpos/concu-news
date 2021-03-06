#include "Server.h"
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <SIGINT_Handler.h>
#include <SignalHandler.h>
#include "../ipcs/Cola.h"
#include "protocol.h"
#include "../utilidades/Logger.h"
#include "MicroServicio.h"


void Server::run() {

    SignalHandler :: getInstance()->registrarHandler (SIGINT, &this->sigint_handler);

    this->initialize();

    this->handleRequests();

    this->terminate();

}

void Server::handleRequests() const {

    message m;
    int resultado = 0;

    while (this->sigint_handler.getGracefulQuit() != 1) {

        resultado = colaPublica.leer(REQUEST, &m);

        if (resultado == -1) {
            continue;
        }

        std::stringstream ss;

        ss << "Cliente me envió key: " << m.key
           << " y value: " << m.value;

        Logger::getInstance()->info(ss.str());

        if (m.type == TYPE_SET_CITY || m.type == TYPE_GET_CITY) {
            Logger::getInstance()->info("Server: Enviando mensaje a microservicio de ciudades");

            m.mtype = this->estadoDelTiempoPID;
        } else {
            Logger::getInstance()->info("Server: Enviando mensaje a microservicio de monedas");

            m.mtype = this->cotizacionDeMonedasPID;
        }

        colaMicorServicios.escribir(m);

        colaMicorServicios.leer(RESPONSE, &m);

        m.mtype = m.responsePriority;

        Logger::getInstance()->info("Server: Respondiendo mensaje");

        colaPublica.escribir(m);
    }

    SignalHandler::destruir();

}

void Server::terminate() const {
    Logger::getInstance()->info("Server: Terminando server");

    int exit;

    if(this->cotizacionDeMonedasPID != 0) {
        Logger::getInstance()->info("Server: Terminando microservicio de cotización de monedas");
        kill(this->cotizacionDeMonedasPID, SIGINT);
        wait(&exit);
    }

    if (this->estadoDelTiempoPID != 0) {
        Logger::getInstance()->info("Server: Terminando microservicio de estado del tiempo");
        kill(this->estadoDelTiempoPID, SIGINT);
        wait(&exit);
    }

    Logger::getInstance()->info("Server: Terminando colas");
    this->colaPublica.destruir();
    this->colaMicorServicios.destruir();
}

void Server::initialize() {
    Logger::getInstance()->info("Server: Inicializando server");

    Logger::getInstance()->info("Server: Inicializando servicio de ciudades");

    this->cotizacionDeMonedasPID = fork();

    if (this->cotizacionDeMonedasPID == 0) {

        MicroServicio cotizacionDeMonedas("cotizacion_de_monedas");

        cotizacionDeMonedas.setQueue(&this->colaMicorServicios);

        cotizacionDeMonedas.run();
    }

    Logger::getInstance()->info("Server: Inicializando servicio de monedas");

    this->estadoDelTiempoPID = fork();

    if (this->estadoDelTiempoPID == 0) {

        MicroServicio estadoDelTiempo("estado_del_tiempo");

        estadoDelTiempo.setQueue(&this->colaMicorServicios);

        estadoDelTiempo.run();
    }
}
