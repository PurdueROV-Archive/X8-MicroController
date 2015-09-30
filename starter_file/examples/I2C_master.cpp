#include "mbed.h"


#define SIZE (10)
#define ADDR (0x90)




I2C i2c(p28, p27);


int main() {
    
	
	bool success = true;
	char buf[] = {3, 2, 1, 4, 5, 6, 7, 8, 9, 10};
	char res[SIZE];

	i2c.write(ADDR, buf, SIZE);
	i2c.read(ADDR, res, SIZE);

	// here should be buf[all]++
	i2c.write(ADDR, res, SIZE);
	i2c.read(ADDR, res, SIZE);

	// here should be buf[all]+=2
	i2c.write(ADDR, res, SIZE);
	i2c.write(ADDR, res, SIZE);

	// here should be buf[all]+=3
	i2c.read(ADDR, res, SIZE);
	i2c.read(ADDR, res, SIZE);

	for(int i = 0; i < SIZE; i++) {
		if (res[i] != (buf[i] + 3)) {
		success = false;
		break;
		}
	}

}
