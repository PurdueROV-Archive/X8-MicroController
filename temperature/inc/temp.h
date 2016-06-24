#ifndef __TEMP_H
#define __TEMP_H

/* puts all of the #includes and # defines that you need here */
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_i2c.h"

#include "main.h"
#include <math.h>

#define TSYS01_ADDR                        0x77 << 1
#define TSYS01_RESET                       0x1E
#define TSYS01_ADC_READ                    0x00
#define TSYS01_ADC_TEMP_CONV               0x48
#define TSYS01_PROM_READ                   0XA0


class Temp {

public:
	/* Put function declarations that the user will
	   want to use from the class here */

    /*constructor */
    Temp(I2C_HandleTypeDef* handler);
    void init();
    void read();
    void ADC_begin();
    float getTemp();

    uint32_t rawInfo;
    uint16_t Constants[8];
uint8_t data[8];
uint8_t raw_temp[4];

private:

    I2C_HandleTypeDef* I2C_handler;
    
    //uint8_t data[8];
    
    uint32_t adc;
    float TEMP;

    void calculate();

};

#endif
