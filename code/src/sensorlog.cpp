#include "../include/sensorlog.hpp"

void slog::write(int vs1, int vs2, int vs3, int vs4, int vs5, int vs6) {
    std::ofstream outputFile;
    outputFile.open(fileName);
    outputFile << "<meta http-equiv=\"Content-type\" content=\"text/html\" "
                  "charset=\"utf8\"/>"
               << "Kleursensor: <br>"
               << "rood: " << vs1 << "<br>"
               << "groen: " << vs2 << "<br>"
               << "blauw: " << vs3 << "<br><br>"
               << "Motoren: <br>"
               << "Stuur: " << vs4 << "<br>"
               << "Linksachter: " << vs5 << "<br>"
               << "Rechtsacter: " << vs6;
    outputFile.close();
}