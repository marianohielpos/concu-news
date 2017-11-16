#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>

typedef struct {
    std::string logName = "";
    std::string logLevel = "info";
    bool debug = false;
} Opciones;


class Server {

public:

  void run();

};

#endif
