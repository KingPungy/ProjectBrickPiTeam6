#include "../include/sensorlog.hpp"

void slog::write(int RD, int GR, int BL, float distance, int light, int stuur,
                 int LA, int LR) {
    std::ofstream outputFile;
    outputFile.open(fileName);
    outputFile << "<meta http-equiv=\"Content-type\" content=\"text/html\" "
                  "charset=\"utf8\"/>"
               << "Kleursensor: <br>"
               << "rood: " << RD << "<br>"
               << "groen: " << GR << "<br>"
               << "blauw: " << BL << "<br>"
               << "afstand: " << distance << "<br>"
               << "licht: " << light << "<br><br>"
               << "Motoren: <br>"
               << "Stuur: " << stuur << "<br>"
               << "Linksachter: " << LA << "<br>"
               << "Rechtsacter: " << LR;
    outputFile.close();
}