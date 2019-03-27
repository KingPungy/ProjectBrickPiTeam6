#include "BrickPi3.hpp"

using namespace std;

int main(){

	signal(SIGINT, exit_signal_handler);

	BP.detect();
	int error;
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);
	sensor_ultrasonic_t Ultrasonic4;
	while(true){
		if(BP.get_sensor(PORT_4, &Ultrasonic4) == 0){
			cout << "Ultrasonic sensor (S4): "   << Ultrasonic4.cm << "cm" << endl;
		}
	usleep(500000);
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
  if(signo == SIGINT){
    BP.reset_all();    // Reset everything so there are no run-away motors
    exit(-2);
  }
}
