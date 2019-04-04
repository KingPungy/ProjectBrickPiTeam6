#ifndef BRICK_IO_HPP
#define BRICK_IO_HPP

#include "../include/BrickPi3.h"

class IO {
   public:
    // Light Sensor variables
    int black = 2200;
    int white = 1300;
    float average = 1750;
    int lightValue;
    // Ultra-Sonic-Sensor Variables
    int distance;
    // Touch Sensors Varables
    bool touchSensor1, touchSensor2;

    // variables for motor speed
    int speedA, speedB, speedC;

    int maxSteering = 100;

    // Sensor write variables
    sensor_touch_t Touch1, Touch2;
    sensor_light_t Light3;
    sensor_ultrasonic_t Ultrasonic4;

    // Motor Variables
    bool drive = true;

    int MAX_SPEED = 360;

    BrickPi3 BP;

    IO();

    void update();

    int calcSpeed();

    void resetEncoders();

    void dpsA(int speed);
    void dpsB(int speed);
    void dpsC(int speed);

    void setForward(int speed);
    void steerPosition(int pos);
};

#endif
