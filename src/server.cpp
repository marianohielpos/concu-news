
#include "procesos/Server.h"
#include "utilidades/Opciones.h"
#include "utilidades/Logger.h"

int main(int argc, char const *argv[])
{
    Opciones opciones;

    Logger::setInstance(opciones);

    Server s;

    s.run();

    return 0;
}
