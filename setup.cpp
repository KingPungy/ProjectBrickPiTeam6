#include "BrickPi.hpp"

void setup_motors_sensors() {
    BrickPi3 BP;

    BP.detect();  // Make sure that the BrickPi3 is communicating and that the
                  // firmware is compatible with the drivers.

    // Motors
    // BP.offset_motor_encoder(PORT_A, BP.get_motor_encoder(PORT_A));
    BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
    BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));

    // Sensors
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    
    sensor_light_t Light3;
    // BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);
    // sensor_ultrasonic_t Ultrasonic4;
}