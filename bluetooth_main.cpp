#include "BrickPi3.hpp"
using namespace std;

void setup_motors_sensors() {
	BrickPi3 BP;
	BP.detect();	// Make sure that the BrickPi3 is communicating and that the
					// firmware is compatible with the drivers.

	// Motors & sensors.
	BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));
	BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC);

}

int main() {
	setup_motors_sensors();
	sensor_ultrasonic_t Ultrasonic4;
	BluetoothServerSocket serversock(2, 1);  //2 is het channel-number
	cout << "listening" << endl;
	while(true) {
		BluetoothSocket* clientsock = serversock.accept();
		cout << "accepted from " << clientsock->getForeignAddress().getAddress() << endl;
		MessageBox& mb = clientsock->getMessageBox();

		string input;
		int speed = 0;
		while(mb.isRunning()) {
			if(BP.get_sensor(PORT_4, &Ultrasonic4) == 0){
				cout << Ultrasonic4.cm << "\n";
				while(Ultrasonic4.cm < 10){
					BP.set_motor_dps(PORT_B, 0);
					BP.set_motor_dps(PORT_C, 0);
				}
			}
			BP.set_motor_dps(PORT_B, speed);
			BP.set_motor_dps(PORT_C, speed);
			input = mb.readMessage();  //blokkeert niet
			if(input != "") cout << endl << input << endl;
				if(strcmp(input.c_str(), "UP") == 0){
					if(speed < 4000){
						speed += 200;
					}
					BP.set_motor_dps(PORT_B, speed);
					BP.set_motor_dps(PORT_C, speed);
				}
				if(strcmp(input.c_str(), "LEFT") == 0){
					BP.set_motor_dps(PORT_B, -500);
					BP.set_motor_dps(PORT_C, 500);
					usleep(500000);
					BP.set_motor_dps(PORT_B, speed);
					BP.set_motor_dps(PORT_C, speed);
				}
				if(strcmp(input.c_str(), "RIGHT") == 0){
					BP.set_motor_dps(PORT_B, 500);
					BP.set_motor_dps(PORT_C, -500);
					usleep(500000);
					BP.set_motor_dps(PORT_B, speed);
					BP.set_motor_dps(PORT_C, speed);
				}
				if(strcmp(input.c_str(), "DOWN") == 0){
					speed = -240;
					BP.set_motor_dps(PORT_B, -240);
					BP.set_motor_dps(PORT_C, -240);
				}
				if(strcmp(input.c_str(), "FIRE") == 0){
					speed = 0;
					BP.set_motor_dps(PORT_B, 0);
					BP.set_motor_dps(PORT_C, 0);
				}
			//doe andere dingen.
			cout.flush();
			usleep(500000); // wacht 500 ms
		}

		clientsock->close();

	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
