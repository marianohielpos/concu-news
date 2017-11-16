#include "Client.h"
#include "../ipcs/Cola.h"
#include "protocol.h"

#include <iostream>



void Client::run() {
  std::cout << "Soy cliente" << std::endl;

  Cola<message> cola("/bin/bash", 'a');

  message m;
  m.mtype = REQUEST;
  m.type = TYPE_GET_CITY;

  std::string key("quiero esto");
  strcpy(m.key, key.c_str());

  cola.escribir(m);
  cola.leer(RESPONSE, &m);

  std::cout << "Servidor me respondió " << m.value << std::endl;

}
