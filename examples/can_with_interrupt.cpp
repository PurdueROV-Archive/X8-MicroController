#include "mbed.h"

//instances of objects used in the code
Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);

CAN (PinName rd, PinName td); //STILL NEED TO GET THE CORRECT PINS FOR THIS


char counter = 0;


void send() {
	//printf("send()\n");
	CANMessage msg = CANMessage(1337, &counter, 1);  //sets up the can message
	
	if(can1.write(msg)) 
	{
		counter++;
	}
	led1 = !led1;  //toggles the led
}

int main() {
	
    ticker.attach(&send, 1); //attaches the can write command to the ticker, which will
					   //run the function once a second

    while(1) {
        //printf("loop()\n");
        wait(1);
    }
}
