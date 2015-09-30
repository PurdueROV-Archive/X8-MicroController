#include "mbed.h"


DigitalOut out(PD_12);
DigitalIn in(PC_12);


int main() {

	while(1)          
		 
	{
		out = 0;
		wait(0.5);

		out = 1;
		wait(0.5);        
	} 

}
