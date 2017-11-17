#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>
#include "protocol.h"

#define CLIENT_MODE_UNSET 0
#define CLIENT_MODE_CITY 1
#define CLIENT_MODE_CURRENCY 2

typedef struct {
    std::string logName = "";
    std::string logLevel = "info";
    bool debug = false;

    bool admin = false;
    int mode = CLIENT_MODE_UNSET;
    std::string key = "";
    std::string value = "";

} Opciones;


class Client {

Opciones opts_;

void checkOptions();
message composeRequest();
void logRequest(message& m);

public:
  Client(Opciones opts);

  void run();

};

#endif
