//turns on the triggering pin and the led
#define IN_OUT_SET      out = 1; myled = 1;
#define IN_OUT_CLEAR    out = 0; myled = 0;

//defines the pins to be used
#define PIN_OUT    PD_12
#define PIN_IN     PD_0

//Sets D0 to be a pin that has an interrupt connected to it
DigitalOut out(PIN_OUT);
InterruptIn in(PIN_IN);

DigitalOut myled(LED1);
DigitalOut led2(LED2);


//turns on and off the interupt pin five times
void flipper() {
    for (int i = 0; i < 5; i++) {
        IN_OUT_SET;
        wait(0.2);
        IN_OUT_CLEAR;
        wait(0.2);
    }
}

volatile int checks = 0;
void in_handler() {
    checks++;
    led2 = !led2;
}

int main() {

	while(1)
	{
		IN_OUT_CLEAR;  //turns off the pins
	   
		//Test falling edges first
		in.rise(NULL);
		in.fall(in_handler);
		flipper();
		
		if(checks != 5) {
			//if the interrupt did not read in the five falling edges caused by the out pin
		}
		else
		{
			//if the interrupt worked
		}

		//Now test rising edges
		in.rise(in_handler);
		in.fall(NULL);
		flipper();  //toggles the trigger pin five times
		
		if (checks != 10) {
			//if the interrupt did not read in another 5 changes
		}
		else
		{
			//if the interrupt worked
		}

		//Now test switch off edge detection
		in.rise(NULL);
		in.fall(NULL);
		flipper();

		if (checks != 10) {
			//if the interrupt did not turn off, then this code runs
		}
		else
		{
			//if the interrupt turn off worked
		}

		//turns on an interrupt handler on both the falling edge and rising edge
		in.rise(in_handler);
		in.fall(in_handler);
		flipper();

		if (checks != 20) {
			//if both interrupts did not work
		}
		else
		{
			//if both interrupts workeds
		}
		
		checks = 0;  //resets check
	}
}
