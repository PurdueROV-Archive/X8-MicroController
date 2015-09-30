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

    tick.attach_us(togglePin, 1000000);  //sets up the ticker to call togglePin every 1000000 micro seconds, or one second

    while (1);
}
