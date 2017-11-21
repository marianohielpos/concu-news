#include "MicroServicio.h"
#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>

void MicroServicio::hidrate() {

    std::string line;
    std::ifstream file (this->file);

    if (!file.is_open())
        throw std::exception();

    while (std::getline(file, line)) {

        size_t separator_position = line.find(":");

        std::string key = line.substr(0, separator_position);

        std::string value = line.substr(separator_position + 1);

        this->data[key] = value;
    }

    file.close();
}


void MicroServicio::persist() {

    std::ofstream file (this->file);

    if (!file.is_open())
        throw std::exception();

    for (std::map<std::string, std::string>::iterator it = this->data.begin(); it != this->data.end() ; it++) {

        file << it->first.c_str() << ":" << it->second.c_str();
        
    }

    file.close();
}