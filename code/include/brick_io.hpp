#ifndef BRICK_IO_HPP
#define BRICK_IO_HPP

#include "../include/BrickPi3.h"
#include "../include/sensorlog.hpp"

class IO {
   public:
    // Light Sensor variables
    int black = 2200;
    int white = 1300;
    float average = 1750;
    int lightValue;
    int redValue, greenValue, blueValue;

    // Ultra-Sonic-Sensor Variables
    int distance;
    // Touch Sensors Varables
    bool touchSensor1, touchSensor2;

    // variables for motor speed
    int speedA, speedB, speedC;

    int maxSteering = 75;

    // Sensor write variables
    sensor_touch_t Touch1, Touch2;

    sensor_color_t Color1;

    sensor_light_t Light3;
    sensor_ultrasonic_t Ultrasonic4;

    // Motor Variables
    bool drive = true;

    int MAX_SPEED = 360;

    BrickPi3 BP;

    slog logClass;

    IO();

    void update();

    int calcSpeed();

    void resetEncoders();

    void dpsD(int speed);
    void dpsB(int speed);
    void dpsC(int speed);
    void setLeft(int speed);
    void setRight(int speed);

    void steerPosition(int pos);
};

#endif
