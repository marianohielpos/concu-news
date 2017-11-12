#include "Server.h"

#include <iostream>

#include "../ipcs/Cola.h"
#include "protocol.h"
#include "../utilidades/Logger.h"



void Server::run() {

    Logger::getInstance()->info("Inicializando server");

    Logger::getInstance()->info("Inicializando servicio de ciudades");

    Logger::getInstance()->info("Inicializando servicio de monedas");

    Cola<message> cola("/bin/bash", 'a');

    message m;

    cola.leer(REQUEST, &m);

    std::cout << "Cliente me envió key: " << m.key << std::endl;

    m.mtype = RESPONSE;
    std::string value("respuestaza");
    strcpy(m.value, value.c_str());
    cola.escribir(m);

    std::cout << "Terminandome! " << std::endl;

    cola.destruir();
}
