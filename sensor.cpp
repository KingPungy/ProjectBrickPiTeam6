
#include "sensor.hpp"

sensors::sensors() {
    BP.set_sensor_type(PORT_1, SENSOR_TYPE_TOUCH);
    BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
    BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);
}

void sensors::update() {
    BP.get_sensor(PORT_1, &Touch1);
    touchSensor = !(Touch1.pressed);
    BP.get_sensor(PORT_3, &Light3);
    lightValue = Light3.reflected;
    BP.get_sensor(PORT_4, &Ultrasonic4);
    distance = Ultrasonic4.cm;
}

void sensors::calibrate() {
    while (true) {
        // wait for user input (druk sensor Nxt)
        if (!(BP.get_sensor(PORT_1, &Touch1))) {
            update();
            white = lightValue;
            break;
        }
    }
    
    while (true) {
        // wait for user input (druk sensor Nxt)
        if (!(BP.get_sensor(PORT_1, &Touch1))) {
            update();
            black = lightValue;
            break;
        }
    }
    average = black + ((white - black) / 2);
    printf(average);
}