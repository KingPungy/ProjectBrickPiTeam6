/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>      // for catching exit signals
#include <stdio.h>       // for printf
#include <unistd.h>      // for usleep
//#include "BrickPi3.h"  // for BrickPi3

// Class headers
#include "../include/controller.hpp" // Controller input Class
#include "../include/brick_io.hpp"     // IO input Class

void exit_signal_handler(int signo);
// classControl controller;
IO dotIO;
    
int main() {
    signal(SIGINT, exit_signal_handler);  // register exit for Ctrl+C
    
    

    while(true) {
        usleep(1 * 1000);
        dotIO.update();
        if (!dotIO.touchSensor1)
        {
            printf("touch sensor 1: %d\n", dotIO.lightValue);
            dotIO.black = dotIO.lightValue;
        }
        if (!dotIO.touchSensor2)
        {
            printf("touch sensor 2: %d\n", dotIO.lightValue);
            dotIO.white = dotIO.lightValue;
        }
        if (!dotIO.touchSensor1 || !dotIO.touchSensor2)
        {
            dotIO.dpsB(0);
            dotIO.dpsC(0);
            continue;
        }

        int speed = dotIO.calcSpeed() / 2;
        
        if (speed < 0){
            speed -= 50;
            dotIO.dpsB(100-abs(speed));
            dotIO.dpsC(abs(speed));
        }else if(speed > 0){
            speed += 50;
            dotIO.dpsB(speed);
            dotIO.dpsC(100-abs(speed));
        }
        else if (speed == 0){
            dotIO.dpsB(50);
            dotIO.dpsC(50);
        }
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