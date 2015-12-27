/* STM32F4-CANBUS leds.h
 * http://www.eraycanli.com */

#pragma once


void InitializeLEDS();


#define LED_GREEN GPIO_PIN_12
#define LED_ORANGE GPIO_PIN_13
#define LED_RED GPIO_PIN_14
#define LED_BLUE GPIO_PIN_15

void LedState(uint16_t led, uint8_t state);

/* leds.h */
