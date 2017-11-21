#ifndef CONCU_NEWS_MICROSERVICIO_H
#define CONCU_NEWS_MICROSERVICIO_H

#include <map>
#include <string>

class MicroServicio {

private:
    std::map<std::string, std::string> data;

    void persist();
    
    void hidrate();

    std::string file;

};


#endif //CONCU_NEWS_MICROSERVICIO_H
