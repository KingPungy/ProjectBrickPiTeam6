#include "../include/sensorlog.hpp"

void slog::write(std::string s1, bool vs1) {
    std::ofstream outputFile;
    outputFile.open(fileName);
    outputFile << s1 << vs1;
    outputFile.close();
}