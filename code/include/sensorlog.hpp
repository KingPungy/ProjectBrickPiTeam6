#ifndef SENSORLOG_HPP
#define SENSORLOG_HPP

#include <fstream>
#include <iostream>
#include <string>

class slog {
   public:
    std::string fileName = "/var/www/html/sensordata.html";

    void write(int vs1, int vs2, int vs3, int vs4, int vs5, int vs6);
};

#endif