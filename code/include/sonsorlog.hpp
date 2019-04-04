#ifndef SENSORLOG_HPP
#define SENSORLOG_HPP

#include <fstream>
#include <string>

class slog {
    std::string fileName = "log.txt";

    void write(std::string s1, bool vs1);
};

#endif