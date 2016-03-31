#include "shiftRegister.h"

//input the pins that will be used on the shift register
shiftRegister::shiftRegister(GPIO_TypeDef* dataB, uint16_t dataP, GPIO_TypeDef* clkB, uint16_t clkP, GPIO_TypeDef* latB, uint16_t latP)
{
    dataBank = dataB; //data pin - sends data
    dataPin = dataP;

    clkBank = clkB; //clock pin -
    clkPin = clkP;

    latBank = latB; //latch pin - keep on low while data is sent and on high to stop sending
    latPin = latP;
}

/* this is where you will write the shift register code that outputs the controls.
 * Each bit in the variable will represent one of the pins on the shift register
 * */

/*Shifts out a byte of data at a time starting from the most(leftmost) significant bit.
 * */
void shiftRegister::shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t data)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        dataPin = ( !!(data & (1 << i))); //data is sent

        //clock pin is taken high and then low to indicate that bit is available
        clockPin = 1;
        wait(.1);
        clockPin = 0;
    }
void shiftRegister::control(uint8_t data)
{

}


