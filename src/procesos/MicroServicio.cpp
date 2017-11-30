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
        Logger::getInstance()->error(this->name + ": Error abriendo el archivo de datos. Inicilizando sin datos");
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

    std::ofstream file (this->file, std::ofstream::trunc);

    if (!file.is_open()) {
        Logger::getInstance()->error(this->name + ": Error abriendo el archivo de datos. No se persisten los datos");
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

    Logger::getInstance()->info(this->name + ": Hidratando datos");

    this->hidrate();

    Logger::getInstance()->info(this->name + ": Escuchando pedidos");
    while (sigint_handler.getGracefulQuit() != 1) {
        this->handleRequest();
    }

    Logger::getInstance()->info(this->name + ": Persistiendo datos");
    this->persist();

    SignalHandler :: destruir();

    Logger::getInstance()->info(this->name + ": Saliendo");
    exit(0);
}

MicroServicio::MicroServicio(std::string name) {
    this->file = name + ".txt";
    this->queuePriority = getpid();
    this->name = name;
}

void MicroServicio::handleRequest() {

    message mensaje;

    int resultado = this->cola->leer(this->queuePriority, &mensaje);

    if (resultado == -1) {
        return;
    }

    Logger::getInstance()->info(this->name + ": Recibiendo mensaje");
    Logger::getInstance()->info(this->name + ": Key: " + mensaje.key);

    mensaje.mtype = RESPONSE;

    if (mensaje.type == TYPE_SET_CITY || mensaje.type == TYPE_SET_CURRENCY) {
        Logger::getInstance()->info(this->name + ": Guardando valor");
        Logger::getInstance()->info(this->name + ": Value: " + mensaje.value);
        this->set(std::string(mensaje.key), std::string(mensaje.value));
        strcpy(mensaje.value, "OK");
        mensaje.type = TYPE_SUCCESS;
    } else {
        if (keyIsPresent(mensaje.key)) {
            Logger::getInstance()->info(this->name + ": Valor encontrado");
            strcpy(mensaje.value, this->get(mensaje.key).c_str());
            mensaje.type = TYPE_SUCCESS;
        } else {
            Logger::getInstance()->info(this->name + ": Valor no encontrado");
            strcpy(mensaje.value, "Valor no encontrado");
            mensaje.type = TYPE_ERROR;
        }
    }

    Logger::getInstance()->info(this->name + ": Respondiendo mensaje");

    this->cola->escribir(mensaje);

    return;

}

void MicroServicio::set(std::string key, std::string value) {
    this->data[key] = value;
}

bool MicroServicio::keyIsPresent(std::string key) {
    std::map<std::string, std::string>::iterator it = this->data.find(key);

    return it != this->data.end();
}

std::string MicroServicio::get(std::string key) {
    return this->data[key];
}

void MicroServicio::setQueue(const Cola<message> *cola) {

    this->cola = cola;

}
