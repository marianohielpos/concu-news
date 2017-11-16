#include "Client.h"
#include "../ipcs/Cola.h"
#include "../utilidades/Logger.h"

#include <iostream>


Client::Client(Opciones opts) : opts_(opts) {

}


message Client::composeRequest() {
  message m;
  m.mtype = REQUEST;

  if (opts_.admin) {
    m.type = (opts_.mode == CLIENT_MODE_CITY) ? TYPE_SET_CITY : TYPE_SET_CURRENCY;
    strcpy(m.value, opts_.value.c_str());
  } else {
    m.type = (opts_.mode == CLIENT_MODE_CITY) ? TYPE_GET_CITY : TYPE_GET_CURRENCY;
  }

  strcpy(m.key, opts_.key.c_str());

  return m;
}


void Client::run() {
  Logger::getInstance()->info("Inicializando cliente...");

  Cola<message> cola("/bin/bash", 'a');

  message m = composeRequest();
  cola.escribir(m);

  cola.leer(RESPONSE, &m);

  std::cout << "Servidor me respondió " << m.value << std::endl;

}
