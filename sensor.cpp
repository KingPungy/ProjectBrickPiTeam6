
#include "BrickPi.hpp"

class sensors
{
    public:

    int black, white average;
    int lightValue;

    sensors(){
        BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
        BP.set_sensor_type(PORT_1, SENSOR_TYPE_TOUCH);
        sensor_light_t Light3;
        sensor_touch_t Touch1;
    }

    void update(){
        BP.get_sensor(PORT_3, &Light3);
        lightValue = Light3.reflected;
    }
    
    void calibrate(){

        while (true){
            // wait for user input (druk sensor Nxt)
            if (BP.get_sensor(PORT_1, &Touch1)) {
                update();
                white = lightValue;
                break;
            }  
        }
        
        while (true){
        // wait for user input (druk sensor Nxt)
            if (BP.get_sensor(PORT_1, &Touch1)) {
                update();
                black = lightValue;
                break;
            }
        }
        average = black+((white-black)/2);
        printf(average);
    }

};