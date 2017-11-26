#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>

typedef struct {
    std::string logName = "";
    std::string logLevel = "info";
    bool debug = false;
} Opciones;


#include <Cola.h>
#include "protocol.h"

class Server {

private:

    Cola<message> colaPublica = Cola("/bin/bash", 'a');
    Cola<message> cotizacionDeMonedasCola = Cola("/bin/bash", 'b');
    Cola<message> estadoDeTiempoCola = Cola("/bin/bash", 'c');

public:

    void run();

    void initialize() const;
};

#endif
