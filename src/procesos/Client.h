#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>

typedef struct {
    std::string logName = "";
    std::string logLevel = "info";
    bool debug = false;

    bool admin = false;

} Opciones;


class Client {

public:

  void run();

};

#endif
