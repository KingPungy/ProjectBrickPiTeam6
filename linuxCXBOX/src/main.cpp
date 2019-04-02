/*
Authors: Sander Boot, Scott Timmermans, Tobias van den Hoogen, Mike Hoogendoorn,
Ruben Zwietering

*/
#include <signal.h>  // for catching exit signals
#include <stdio.h>   // for printf
#include <unistd.h>  // for usleep
#include <bitset>    // for bitset

// Class headers
#include "../include/controller.hpp"  // Controller input Class

int main() {
    classControl controller;
    while(true){
        
        controller.update();
        controller.printInput();
        usleep(20*1000);
    }
}

