#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <signal.h>  // for catching exit signals
#include <stdio.h>   // for printf
#include <unistd.h>  // for usleep
#include <iostream>
#include "BrickPi3.cpp"

class sensors
{
    public:

    int black, white average;
    int lightValue;

    sensors();

    void update();
    
    void calibrate();
};

#endif
