/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>      // for catching exit signals
#include <stdio.h>       // for printf
#include <unistd.h>      // for usleep
//#include "BrickPi3.h"  // for BrickPi3

// Class headers
#include "controller.hpp" // Controller input Class
#include "In_Out.hpp"     // IO input Class

void exit_signal_handler(int signo);
// classControl controller;
IO dotIO;
    
int main() {
    signal(SIGINT, exit_signal_handler);  // register exit for Ctrl+C

    // usleep(5 * 1000 * 1000);

    for (uint8_t i = 0; i < 1000000000; i++) {
        dotIO.update();
        dotIO.calcSpeed();
        usleep(5 * 1000);
    }

    dotIO.dpsB(0);
    dotIO.dpsC(0);
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo) {
    if (signo == SIGINT) {
        dotIO.dpsA(0);
        dotIO.dpsB(0);
        dotIO.dpsC(0);  // Reset everything so there are no run-away motors
        exit(-2);
    }
}