/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>  // for catching exit signals
#include <termios.h>
#include <unistd.h>  // for usleep
#include <cmath>
#include <cstdio>  // for printf
#include <cstdlib>

// Class headers
#include "../include/brick_io.hpp"    // IO input Class
#include "../include/controller.hpp"  // Controller input Class
                                      // hulp functies

#include "../include/BrickPi3.h"
#include "../include/server.h"
#define DEBUG

#ifdef DEBUG
#define DEBUGP(a...) printf(a)
#endif

void exit_signal_handler(int signo);
// classControl controller;
IO dotIO;
classControl controller;

int main(int argc, char* argv[]) {
    dotIO.dpsB(0);
    dotIO.dpsC(0);

    while (true) {
        dotIO.update();

        if ((dotIO.redValue > 420 and dotIO.redValue < 460) and
            (dotIO.greenValue > 400 and dotIO.greenValue < 440) and
            (dotIO.blueValue > 230 and dotIO.blueValue < 270)) {
            std::cout << "Insitutieplein!" << std::endl;
        } else if ((dotIO.redValue > 380 and dotIO.redValue < 420) and
                   (dotIO.greenValue > 370 and dotIO.greenValue < 410) and
                   (dotIO.blueValue > 280 and dotIO.blueValue < 320)) {
            std::cout << "Donkergrijs" << std::endl;
        } else if ((dotIO.redValue > 430 and dotIO.redValue < 470) and
                   (dotIO.greenValue > 420 and dotIO.greenValue < 460) and
                   (dotIO.blueValue > 330 and dotIO.blueValue < 370)) {
            std::cout << "Lichtgrijs" << std::endl;
            // if sensor detects red
        } else if ((dotIO.redValue > 570 and dotIO.redValue < 650) and
                   (dotIO.greenValue > 320 and dotIO.greenValue < 400) and
                   (dotIO.blueValue > 170 and dotIO.blueValue < 250)) {
            std::cout << "Rode lijn stop nu!!!" << std::endl;
            // dotIO.dpsB(60);
            // dotIO.dpsC(60);
            // usleep(1000 * 1000);
            dotIO.dpsB(0);
            dotIO.dpsC(0);
        }

        if (dotIO.lightValue > 2400 && dotIO.lightValue < 2600) {
            std::cout << "trap!!" << std::endl;
            // dotIO.dpsB(60);
            // dotIO.dpsC(60);
            // usleep(1000 * 1000);
            dotIO.dpsB(0);
            dotIO.dpsC(0);
        }

        std::cout << dotIO.speedA << "\t" << dotIO.speedB << "\t"
                  << dotIO.speedC << "\t" << dotIO.redValue << "\t"
                  << dotIO.greenValue << "\t" << dotIO.blueValue << "\t"
                  << dotIO.distance << "\t" << dotIO.lightValue << std::endl;

        usleep(100 * 1000);
    }
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    // printf("signal: %d\n", signo);
    // Reset everything so there are no run-away motors
    dotIO.steerPosition(0);
    dotIO.setLeft(0);
    dotIO.setRight(0);
    exit(-signo);
}
