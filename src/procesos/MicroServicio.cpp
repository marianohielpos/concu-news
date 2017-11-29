#include "MicroServicio.h"
#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>
#include <unistd.h>
#include <SignalHandler.h>
#include <Logger.h>
#include "protocol.h"
#include "../ipcs/SIGINT_Handler.h"

void MicroServicio::hidrate() {

    std::string line;
    std::ifstream file (this->file);

    if (!file.is_open()) {
        Logger::getInstance()->error("Error abriendo el archivo de datos. Inicilizando sin datos");
        return;
    }

    while (std::getline(file, line)) {

        size_t separator_position = line.find(":");

        std::string key = line.substr(0, separator_position);

        std::string value = line.substr(separator_position + 1);

        this->data[key] = value;
    }

    file.close();
}


void MicroServicio::persist() const {

    std::ofstream file (this->file);

    if (!file.is_open()) {
        Logger::getInstance()->error("Error abriendo el archivo de datos. No se persisten los datos");
        return;
    }

    for (std::map<std::string, std::string>::const_iterator it = this->data.begin(); it != this->data.end() ; it++) {

        file << it->first.c_str() << ":" << it->second.c_str() << std::endl;
        
    }

    file.close();
}

void MicroServicio::run() {

    SIGINT_Handler sigint_handler;

    SignalHandler :: getInstance()->registrarHandler (SIGINT, &sigint_handler);

    this->hidrate();

    while (sigint_handler.getGracefulQuit() != 1) {
        this->handleRequest();
    }

    this->persist();

    SignalHandler :: destruir();

    exit(0);
}

MicroServicio::MicroServicio(std::string file) {
    this->file = file;
    this->queuePriority = getpid();
}

void MicroServicio::handleRequest() {

    message mensaje;

    this->cola->leer(this->queuePriority, &mensaje);

    if (mensaje.type == TYPE_SET_CITY || mensaje.type == TYPE_SET_CURRENCY) {
        this->set(std::string(mensaje.key), std::string(mensaje.value));
        strcpy(mensaje.value, "Seteo correctamente del valor");
    } else {
        strcpy(mensaje.value, this->get(mensaje.key).c_str());
    }

    mensaje.mtype = RESPONSE;
    mensaje.type = TYPE_SUCCESS;

    this->cola->escribir(mensaje);

}

void MicroServicio::set(std::string key, std::string value) {
    this->data[key] = value;
}

std::string MicroServicio::get(std::string key) {

    std::map<std::string, std::string>::iterator it = this->data.find(key);

    if (it == this->data.end()) {
        return "Elemento no encontrado";
    }

    return this->data[key];
}

void MicroServicio::setQueue(const Cola<message> *cola) {

    this->cola = cola;

}
