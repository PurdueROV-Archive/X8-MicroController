#ifndef X8_SHIFT_REGISTER
#define X8_SHIFT_REGISTER

/* puts all of the #includes and # defines that you need here */
#include "main.h"


class shiftRegister {

public:
    shiftRegister(GPIO_TypeDef* dataB, uint16_t dataP,GPIO_TypeDef* clkB, uint16_t  clkP);
    void control(uint8_t data);

private:
    GPIO_TypeDef*   dataBank;
    uint16_t        dataPin;

    GPIO_TypeDef*   clkBank;
    uint16_t        clkPin;
};

#endif
