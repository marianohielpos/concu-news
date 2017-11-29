#ifndef CONCU_NEWS_MICROSERVICIO_H
#define CONCU_NEWS_MICROSERVICIO_H

#include <map>
#include <string>
#include <Cola.h>
#include "protocol.h"

class MicroServicio {

private:
    std::map<std::string, std::string> data;

    void persist() const;
    
    void hidrate();

    void set(std::string key, std::string value);

    std::string get(std::string key);

    void handleRequest();

    std::string file;

    const Cola<message> *cola = nullptr;

    pid_t queuePriority;

public:

    MicroServicio(std::string file);

    void run();

    void setQueue(const Cola<message> *cola);
};


#endif //CONCU_NEWS_MICROSERVICIO_H
