#ifndef CONCU_NEWS_MICROSERVICIO_H
#define CONCU_NEWS_MICROSERVICIO_H

#include <map>
#include <string>
#include <Cola.h>
#include "protocol.h"

class MicroServicio {

private:
    std::map<std::string, std::string> data;

    void persist();
    
    void hidrate();

    void set();

    std::string get(std::string key);

    void send();

    void handleRequest();

    std::string file;

    Cola<message> *cola = nullptr;

public:

    MicroServicio(std::string file);

    void run();

    void setQueue(Cola<message> *cola);
};


#endif //CONCU_NEWS_MICROSERVICIO_H
