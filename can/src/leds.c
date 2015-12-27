/* STM32F4-CANBUS leds.c
 * http://www.eraycanli.com */

#include "commons.h"
#include "leds.h"


void InitializeLEDS()
{
    __GPIOD_CLK_ENABLE(); /* Enable GPIOD clock for leds */

    GPIO_InitTypeDef GPIO_Leds;

    GPIO_Leds.Mode = GPIO_MODE_OUTPUT_PP; /* Output push-pull mode */
    GPIO_Leds.Pull = GPIO_NOPULL; /* No resistor */
    GPIO_Leds.Speed = GPIO_SPEED_HIGH;
    GPIO_Leds.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; /* PD12(Green) PD13(Orange) PD14(Red) PD15(Blue) */
    HAL_GPIO_Init(GPIOD, &GPIO_Leds); /* Init GPIOD registers */


    return;
}


void LedState(uint16_t led, uint8_t state)
{
    switch(state)
    {
        case 1: /* ON */
            HAL_GPIO_WritePin(GPIOD, led, GPIO_PIN_SET);
            break;
        case 0: /* OFF */
            HAL_GPIO_WritePin(GPIOD, led, GPIO_PIN_RESET);
            break;
        case 2: /* Toggle */
            HAL_GPIO_TogglePin(GPIOD, led);
            break;
    }

    return;
}


/* leds.c */
