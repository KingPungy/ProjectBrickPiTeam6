#ifndef SENSORLOG_HPP
#define SENSORLOG_HPP

#include <fstream>
#include <iostream>
#include <string>

class slog {
   public:
    std::string fileName = "/var/www/html/sensordata.html";

    void write(int RD, int GR, int BL, float distance, int light, int stuur,
               int LA, int LR);
};

#endif