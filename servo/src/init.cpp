/*This folder will be used to hold all code used to initialize pins */

#include "init.h"
#include "stm32f4xx_hal.h"

TIM_OC_InitTypeDef sConfig;/* Timer Output Compare (technical name for PWM) */
TIM_HandleTypeDef    TimHandle;/* Timer handler declaration */

uint32_t uhPrescalerValue;

void initPWM(void)
{
    /*configures how the pwm settings and it's duty cycle */
    sConfig.OCMode        = TIM_OCMODE_PWM1;  //puts it into generic pwm mode
    sConfig.OCPolarity     = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode  = TIM_OCFAST_DISABLE;
    sConfig.OCNPolarity   = TIM_OCNPOLARITY_HIGH;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

    sConfig.Pulse = 0; /* Set the pulse value for channel 1 */

    //checks if these settings initialize correctly
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }

    /*checks if the pwm starts correctly*/
    if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
}

void initGPIO(void)
{
    static GPIO_InitTypeDef  GPIO_InitStruct;

    //must be included to initially configure the library
    HAL_Init();

    /* Configure the system clock to 100 MHz */
    SystemClock_Config();

    //enable the led clock
    __HAL_RCC_GPIOA_CLK_ENABLE();


    //enables the clock used by the pwm timer
    __TIM2_CLK_ENABLE();


    /*this initializes the pin A5 to be act as its alternate functions
     * which is pwm (GPIO_AF1_TIM2) */
    GPIO_InitStruct.Pin = GPIO_PIN_5; //pin 5
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  //puts it in Alterate Fucntion mode (AF mode)
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;  //puts it in AF1_TIM2 mode (pwm mode)
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin as an alternate function
    //you still have to initialize the pwm portion
}

//configures the system clcok
void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();


    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* Enable HSI Oscillator and activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    while(1)
    {

    }
}