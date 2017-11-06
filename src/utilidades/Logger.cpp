#include "Logger.h"
#include <unistd.h>
#include <sstream>

Logger* Logger::instance = NULL;

Logger* Logger::getInstance() {

    if (Logger::instance == nullptr)
        throw std::exception();

    return Logger::instance;
}


void Logger::setInstance(Opciones opciones) {
    Logger::instance = new Logger(opciones.logName, opciones.logLevel);
}


void Logger::deleteInstance() {
    if ( Logger::instance != NULL ) {
        delete ( Logger::instance );
        Logger::instance = NULL;
    }
}

Logger::Logger(std::string nombreArchivo, std::string nivel) {

    if( this->levels.find(nivel) != this->levels.end() )
        this->logLevel = nivel;

    if(nombreArchivo.empty()) {
        this->lock = new LockFile("tmp");
        this->cout = true;
        return;
    }

    this->lock = new LockFile(nombreArchivo);
}

Logger::~Logger() {
    delete this->lock;
}

std::string Logger::generarMensaje(std::string mensaje, std::string nivel){

    std::stringstream mensajeFormateado;

    mensajeFormateado << nivel;

    if (this->pid) {

        mensajeFormateado << " PID: ";

        mensajeFormateado << std::to_string(getpid());

    }

    if (this->timestamp)
    {
        mensajeFormateado << " TS: ";

        mensajeFormateado << std::to_string(time(nullptr));
    }

    mensajeFormateado << " Mensaje: ";

    mensajeFormateado <<  mensaje;

    mensajeFormateado <<  std::endl;

    return mensajeFormateado.str();
}

void Logger::escribirAArchivo(std::string mensaje) {
    this->lock->tomarLock();

    this->lock->escribir(mensaje.c_str(), mensaje.size());

    this->lock->liberarLock();
}

void Logger::escribirAConsola(std::string mensaje) {
    this->lock->tomarLock();

    std::cout << mensaje;

    this->lock->liberarLock();

}

void Logger::info(std::string mensaje) {

    this->imprimirMensaje(mensaje, INFO);

}

void Logger::debug(std::string mensaje) {

    this->imprimirMensaje(mensaje, DEBUG);

}

void Logger::warning(std::string mensaje) {

    this->imprimirMensaje(mensaje, WARNING);

}

void Logger::error(std::string mensaje) {

    this->imprimirMensaje(mensaje, ERROR);

}


void Logger::imprimirMensaje(std::string mensaje, std::string nivel) {

    if( this->levels[nivel] < this->levels[this->logLevel] ) return;

    std::string mensajeFormateado = this->generarMensaje(mensaje, nivel);

    if( this->cout )
        return this->escribirAConsola(mensajeFormateado);

    return this->escribirAArchivo(mensajeFormateado);
}