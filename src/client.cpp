
#include "procesos/Client.h"
#include "utilidades/Opciones.h"
#include "utilidades/Logger.h"

int main(int argc, char const *argv[])
{
    Opciones opciones;

    Logger::setInstance(opciones);

    Client c;

    c.run();

    return 0;
}
