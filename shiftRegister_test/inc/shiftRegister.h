#ifndef X8_SHIFT_REGISTER
#define X8_SHIFT_REGISTER

/* puts all of the #includes and # defines that you need here */
#include "main.h"


class shiftRegister {


public:

    shiftRegister(GPIO_TypeDef* dataB, uint16_t dataP,GPIO_TypeDef* clkB, uint16_t  clkP, GPIO_TypeDef* latB, uint16_t  latP);

    void control(uint8_t data);

    void UpdateHydraulics(void);

    void SetClaw(void);

private:

    GPIO_TypeDef*   dataBank;
    uint32_t        dataPin;

    GPIO_TypeDef*   clkBank;
    uint32_t        clkPin;

    GPIO_TypeDef*   latchBank;
    uint32_t        latchPin;

    uint64_t    clawSetTime;
    uint16_t    timeDelay;
    uint16_t    updateTimeDelay;

    uint8_t clawFlag;

    uint8_t clawOn;
    uint8_t clawOff;
};

#endif
