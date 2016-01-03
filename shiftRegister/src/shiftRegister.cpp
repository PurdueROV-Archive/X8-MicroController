#include "shiftRegister.h"


//input the pins that will be used on the shift register
shiftRegister::shiftRegister(GPIO_TypeDef* dataB, uint16_t dataP,GPIO_TypeDef* clkB, uint16_t  clkP)
{
    dataBank = dataB;
    dataPin = dataP;

    clkBank = clkB;
    clkPin = clkP;
}

/* this is where you will write the shift register code that outputs the controls.
 * Each bit in the variable will represenst one of the pins on the shift register
 * */

void shiftRegister::control(uint8_t data)
{

}


