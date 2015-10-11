#include "mbed.h"

//put non-mbed libraries #defiens here
#define ADDR		0xA4  //the address of the wii nunchuck
#define REG_ADDR	0x40  //the register where will will be reading from
#define DATA_SIZE	6	//the amount of bytes being read in as data
#define ACK		0 	//number that will be returned by read and write if they were successful

//which index of the read_buffer array the joy x and joy y are located in
#define JOY_X		0  
#define JOY_Y		1

PwmOut led(LED1); //initializes the led as pwm

I2C wii(D14, D15); //initialize the SDA (D14) and the SCL(D15) 

char read_buffer[DATA_SIZE];  //this buffer will be used to store in the data being read from the wii nunchuck

void read(void);

int main()
{
	
	unsigned char cmd[] = {REG_ADDR, 0x00};  //initial command that tells the wii numchuck that we want to read from it
	
	if(wii.write(ADDR, (const char*) cmd, sizeof(cmd)) == ACK)
		//add code here if the wii nunchuck is connected and has ACKnowleged us

	
	while (1)
	{
		wait(0.05);  //delay so that the read function is not read to fast
		read();  //read in the data
		
		led.write((read_buffer[JOY_X] - 60) / 100.0);  //write out the x joystick scalled value
	}
}

void read(void)
{
	const unsigned char cmd[] = {0x00};
	
	if(wii.write(ADDR, (const char*) cmd, sizeof(cmd)) == ACK)  //sends a command telling the nunchuck to start sending data to the board
	{
		wait(0.01);  //delay to give the nunchuck time to get set to give data
		
		if(wii.read(ADDR, read_buffer, DATA_SIZE) == ACK)  //reads in 6 bytes from the nunchuck
		{
			//this does some fancy bit shifting magic to get the data correct
			for(int i = 0; i < sizeof(read_buffer); i++)
			{
				read_buffer[i] = (read_buffer[i] ^ 0x17) + 0x17;
			}
		}
		
	}
	
}