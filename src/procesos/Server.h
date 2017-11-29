#ifndef __SERVER_H__
#define __SERVER_H__

#include <string>

typedef struct {
    std::string logName = "";
    std::string logLevel = "info";
    bool debug = false;
} Opciones;


#include <Cola.h>
#include <SIGINT_Handler.h>
#include "protocol.h"

class Server {

private:

    Cola<message> colaPublica = Cola<message>("/bin/bash", 'a');
    Cola<message> colaMicorServicios = Cola<message>("/bin/bash", 'b');

    pid_t cotizacionDeMonedasPID = 0;
    pid_t estadoDelTiempoPID = 0;

    SIGINT_Handler sigint_handler;

public:

    void run();

    void initialize();

    void terminate() const;

    void handleRequests() const;
};

#endif
