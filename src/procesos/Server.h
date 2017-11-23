

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
