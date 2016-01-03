#ifndef X8_ESC
#define X8_ESC

#include "main.h"

class servo {


public:

    /*constructor */
    servo(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel);

    void set(uint8_t dutyCycle);


private:

    TIM_HandleTypeDef *timerHandler;
    TIM_OC_InitTypeDef* sConfig;
    uint32_t channel;

};

#endif
