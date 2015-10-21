#include "mbed.h"

DigitalOut piston1(D1);
DigitalOut piston2(D2);

DigitalOut led(LED1);

DigitalIn button(USER_BUTTON);

//put non-mbed libraries #defiens here

int main()
{
	float delay = 0;
	int buttonState = 0;
	bool prevPressed = false;
	
	 
	while (1)
	{
		//if button is pressed
		if(button == 0)
		{
			wait(0.05);  //delay for debounce
			
			if(button == 0 && prevPressed == false)
			{
				if(buttonState == 0)
				{
					buttonState  = 1;
					
					led = 1;
					
					piston2 = 0;
					
					wait(delay);
					
					piston1 = 1;
				}
				else
				{
					buttonState = 0;
					
					led = 0;
					
					
					piston1 = 0;
					
					wait(delay);
					
					piston2 = 1;
				}
				
				prevPressed = true;
			}
			
		}
		else
			prevPressed = false;
		
	}
}
