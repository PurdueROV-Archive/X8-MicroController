#include "shiftRegister.h"

//input the pins that will be used on the shift register
shiftRegister::shiftRegister(GPIO_TypeDef* dataB, uint16_t dataP,GPIO_TypeDef* clkB, uint16_t  clkP,GPIO_TypeDef* latB, uint16_t  latP)
{

    GPIO_InitTypeDef  GPIO_InitStruct;

    dataBank = dataB;
    dataPin = dataP;

    clkBank = clkB;
    clkPin = clkP;

    latchBank = latB;
    latchPin = latP;

    GPIO_InitStruct.Pin = dataPin;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(dataBank, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = clkPin;
    HAL_GPIO_Init(clkBank, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = latchPin;
    HAL_GPIO_Init(latchBank, &GPIO_InitStruct);

    timeDelay = 1000;
    updateTimeDelay = 2000;
    clawFlag = 1;

    clawOn = 0x11;
    clawOff = 0x00;
}

void wait(__IO uint32_t nCount) {
    while(nCount--) {
    }
}



/* this is where you will write the shift register code that outputs the controls.
 * Each bit in the variable will represent one of the pins on the shift register
 * */

void shiftRegister::control(uint8_t data)
{
    //set latch pin low, prepare to input data
    HAL_GPIO_WritePin(latchBank, latchPin, GPIO_PIN_RESET);

    int temp;

    int delay = 0x0f0;  //small delay to give the shift register time to set

    for(int j = 0; j < 8; j++)
    {
        //set the clock pin low
        HAL_GPIO_WritePin(clkBank, clkPin, GPIO_PIN_RESET);

        wait(delay);

        //read the next bit to read in
        temp = data & (1 << (7 - j));

        //if the next bit is a 1, turn on the data pin, else turn of the data pin
        if(temp)
          HAL_GPIO_WritePin(dataBank, dataPin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(dataBank, dataPin, GPIO_PIN_RESET);

        wait(delay);

        //turn the clock line high to shift in the next piece of data
        HAL_GPIO_WritePin(clkBank, clkPin, GPIO_PIN_SET);

        wait(delay);
    }

    wait(delay);


    //set latch pin high, prepare to load data to pins
    HAL_GPIO_WritePin(latchBank, latchPin, GPIO_PIN_SET);

    //HAL_Delay(10);
}

void shiftRegister::SetClaw(void)
{
    clawSetTime = HAL_GetTick();
    //turn on claw
    clawFlag = 1;
    control(clawOn);
}

void shiftRegister::UpdateHydraulics(void)
{
    if (HAL_GetTick() - clawSetTime > timeDelay)
    {
        if(clawFlag) {
            clawFlag = 0;
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8); //toggle pin works so f
            control(clawOff);
        }
    }
    if (HAL_GetTick() - clawSetTime > updateTimeDelay)
        SetClaw();
}


