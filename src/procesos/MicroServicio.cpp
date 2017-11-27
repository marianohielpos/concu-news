#include "MicroServicio.h"
#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>
#include <unistd.h>
#include "protocol.h"
#include "../ipcs/SIGINT_Handler.h"

void MicroServicio::hidrate() {

    std::string line;
    std::ifstream file (this->file);

    if (!file.is_open())
        throw std::exception();

    while (std::getline(file, line)) {

        size_t separator_position = line.find(":");

        std::string key = line.substr(0, separator_position);

        std::string value = line.substr(separator_position + 1);

        this->data[key] = value;
    }

    file.close();
}


void MicroServicio::persist() {

    std::ofstream file (this->file);

    if (!file.is_open())
        throw std::exception();

    for (std::map<std::string, std::string>::iterator it = this->data.begin(); it != this->data.end() ; it++) {

        file << it->first.c_str() << ":" << it->second.c_str();
        
    }

    file.close();
}

void MicroServicio::run() {

    SIGINT_Handler s;

    this->hidrate();

    while (s.getGracefulQuit() != 1) {

        this->handleRequest();

    }

    exit(0);
}

MicroServicio::MicroServicio(std::string file) {
    this->file = file;
}

void MicroServicio::handleRequest() {

    message mensaje;

    this->cola->leer(REQUEST, &mensaje);

    mensaje.mtype = RESPONSE;
    mensaje.type = TYPE_SUCCESS;

    if (mensaje.type == TYPE_SET_CITY) {
        this->set(std::string(mensaje.key), std::string(mensaje.value));
        std::string value("Seteo correctamente del valor");
        strcpy(mensaje.value, value.c_str());

    } else {
        strcpy(mensaje.value, this->get(mensaje.key).c_str());
    }

    this->cola->escribir(mensaje);

}

void MicroServicio::set(std::string key, std::string value) {
    this->data[key] = value;
}

std::string MicroServicio::get(std::string key) {
    return this->data[key];
}

void MicroServicio::setQueue(const Cola<message> *cola) {

    this->cola = cola;

}
