/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>  // for catching exit signals
#include <stdio.h>   // for printf
#include <termios.h>
#include <unistd.h>  // for usleep

// Class headers
#include "../include/brick_io.hpp"    // IO input Class
#include "../include/controller.hpp"  // Controller input Class

void exit_signal_handler(int signo);
// classControl controller;
IO dotIO;

int main() {
    signal(SIGINT, exit_signal_handler);  // register exit for Ctrl+C

    while (true) {
        dotIO.update();
        std::cout << "\r" << dotIO.speedA << "\t" << dotIO.speedB << "\t"
                  << dotIO.speedC << dotIO.redValue << "\t" << dotIO.greenValue
                  << "\t" << dotIO.blueValue << std::endl;
        usleep(50 * 1000);
    }

    dotIO.dpsB(0);
    dotIO.dpsC(0);
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    printf("signal: %d\n", signo);
    dotIO.dpsA(0);
    dotIO.dpsB(0);
    dotIO.dpsC(0);  // Reset everything so there are no run-away motors
    exit(-2);
}
