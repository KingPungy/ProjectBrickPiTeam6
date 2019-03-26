#ifndef IN_OUT_HPP
#define IN_OUT_HPP

#include "BrickPi3.h"

class IO {

   public:
    // Light Sensor variables
    int black = 2200;
    int white = 1200;
    int average = 1700;
    int lightValue;
    // Ultra-Sonic-Sensor Variables
    int distance;
    // Touch Sensors Varables
    bool touchSensor1, touchSensor2;

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

    void calibrate();

    int calcSpeed();

    void resetEncoders();

    void dpsA(int speed);
    void dpsB(int speed);
    void dpsC(int speed);
};

#endif
