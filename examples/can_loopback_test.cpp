#include "mbed.h"


CAN (PinName rd, PinName td); //STILL NEED TO GET THE CORRECT PINS FOR THIS

#define TEST_ITERATIONS     127

int main() {


	can1.mode(CAN::Reset);

	if (!can1.mode(CAN::LocalTest)) {
		//printf("Mode change failed\n");
	}

	char success_count = 0;
	for (char i=0; i < TEST_ITERATIONS; i++) {
		unsigned int id = 1337;
		CANMessage tx_msg(id, &i, sizeof(i));
		bool sent = false;
		if (can1.write(tx_msg)) {
		//printf("Sent %u: %d\n", id, i);
		sent = true;
        }
        wait_ms(50);

        bool read = false;
        CANMessage rx_msg;
	
        if (can1.read(rx_msg)) {
		//printf("Read %u: %d\n", rx_msg.id, rx_msg.data[0]);
		read = (rx_msg.id == id) && (rx_msg.data[0] == i);
        }

        bool success = sent && read;

        if (success) {
            success_count++;
        }
    }

}
