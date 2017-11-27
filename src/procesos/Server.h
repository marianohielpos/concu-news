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

    Cola<message> colaPublica = Cola<message>("/bin/bash", 'a');
    Cola<message> colaMicroServicios = Cola<message>("/bin/bash", 'b');

public:

    void run();

    void initialize() const;
};

#endif
