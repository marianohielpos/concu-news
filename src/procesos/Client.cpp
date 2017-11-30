#include "Client.h"
#include "../ipcs/Cola.h"
#include "../utilidades/Logger.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>


Client::Client(Opciones opts) : opts_(opts) {
}

void Client::checkOptions() {

  if (opts_.key.empty()) {
    std::cout << "Especifique una clave!" << std::endl;
    exit(1);
  }

  if (opts_.admin && opts_.value.empty()) {
    if (opts_.mode == CLIENT_MODE_CITY) {
      if (opts_.temperature != VALUE_UNSET &&
          opts_.pressure != VALUE_UNSET &&
          opts_.humidity != VALUE_UNSET) {
        std::stringstream ss;

        ss << opts_.temperature << ';' << opts_.pressure << ';' << opts_.humidity;
        opts_.value = ss.str();
      } else {
        std::cout << "Tiene que especificar los 3 valores: "
                  << "temperatura, presión y humedad!" << std::endl;
        exit(1);
      }
    } else {
      std::cout << "Especifique un valor!" << std::endl;
      exit(1);
    }
  }
}


message Client::composeRequest() {
  message m;
  m.mtype = REQUEST;

  strcpy(m.key, opts_.key.c_str());

  if (opts_.admin) {
    strcpy(m.value, opts_.value.c_str());

    m.type = (opts_.mode == CLIENT_MODE_CITY) ? TYPE_SET_CITY : TYPE_SET_CURRENCY;
  } else {
    m.type = (opts_.mode == CLIENT_MODE_CITY) ? TYPE_GET_CITY : TYPE_GET_CURRENCY;
  }

  m.responsePriority = getpid();

  return m;
}


void Client::logRequest(message& m) {
  std::stringstream ss;
  ss << "Enviando un request para ";

  switch(m.type) {
    case TYPE_GET_CITY: ss << "obtener tiempo de "; break;
    case TYPE_GET_CURRENCY: ss << "obtener cotización de "; break;
    case TYPE_SET_CITY: ss << "setear tiempo de "; break;
    case TYPE_SET_CURRENCY: ss << "setear cotización de "; break;
  }

  ss << m.key;
  if (opts_.admin) {
    ss << " con valor " << m.value;
  }

  Logger::getInstance()->info(ss.str());
}


void Client::printResponse(message& m, int type) {
  if (m.type == TYPE_SUCCESS) {
    if (type == TYPE_GET_CITY) {
      float temperature; int pressure, humidity;
      std::sscanf(m.value, "%f;%d;%d", &temperature, &pressure, &humidity);
      std::cout << "El tiempo de " << m.key << " es "
        << "\n\tTemperatura: " << temperature << "°C"
        << "\n\tPresión atmosférica: " << pressure << "mm"
        << "\n\tHumedad: " << humidity << '%' << std::endl;
    } else if (type == TYPE_GET_CURRENCY) {
      std::cout << "La cotización de " << m.key << " es " << m.value << std::endl;
    } else {
      std::cout << "La respuesta del servidor es \n\t" << m.value << std::endl;
    }
  } else if (m.type == TYPE_ERROR) {
    std::cout << "Oops, hubo un problema en el servidor. "
              << "Su respuesta es \n\t" << m.value << std::endl;
  }
}


void Client::run() {
  Logger::getInstance()->info("Inicializando cliente...");

  Cola<message> cola("/bin/bash", 'a');

  checkOptions();
  message m = composeRequest();
  logRequest(m);

  int type = m.type;
  cola.escribir(m);

  cola.leer(getpid(), &m);
  printResponse(m, type);
}
