#include "mbed.h"

#define SIZE (10)
#define ADDR (0x90)


I2CSlave slave(p28, p27);  //initializes the I2C slave

 int main() {
	char buf[SIZE];

	slave.address(ADDR);  //sets the slave address

	while (1) {
		int i = slave.receive();
	
		switch (i) {
			case I2CSlave::ReadAddressed:
				slave.write(buf, SIZE);
				for(int i = 0; i < SIZE; i++){
					
					//read through the information being read in and do something
				}
				break;
			
			case I2CSlave::WriteAddressed:
				slave.read(buf, SIZE);
				for(int i = 0; i < SIZE; i++){
					buf[i]++;
					//write code back to the master
				}
				break;
		}
	}
 }
