#include "mbed.h"

Ticker tick;
DigitalOut led(LED1);

//function to toggle (or switch the state to the other state) the led
void togglePin(void)  
{
        led = !led; // Blink
} 

int main()
{

    tick.attach_us(togglePin, 100000);  //sets up the ticker to call togglePin every 500000 micro seconds, or half second

    while (1);
}
