/* pressure.cpp                                     */
/*==================================================*/
/* @version 1.01            first-working-version   */
/* @author bmaxfie                                  */

// See pressure.h for instructions on the use of this library.


// INCLUDES:
#include "pressure.h"
//#include "main



// CONSTRUCTORS:
pressure::pressure(ms5803_addr addr, I2C_HandleTypeDef* handler)
// Base library type I2C
{
    I2C_handle = handler; // need to change

    _address = addr; //set interface used for communication
}



// CLASS FUNCTIONS:
// ordered and grouped by chronology of calls and relevance respectively.

int pressure::reset(void)
// Reset device I2C
{
   int ack = sendCommand(CMD_RESET);
   sensorWait(3);
   return ack * 3;
}

uint8_t pressure::begin(void)
// Initialize library for subsequent pressure measurements
{  
    uint8_t i;
    char data[2];
    char ack[8];

    for(i = 0; i <= 7; i++){
        ack[i] = sendCommand(CMD_PROM + (i * 2));
        I2Cread( _address, data, 2);
        coefficient[i] = (data[0] << 8)|data[1]; //data[0] highbyte, data[1] lowbyte
        // Uncomment below for debugging output.
        //printString("C");
        //printInt(i);
        //printString("= ");
        //printInt(coefficient[i]);
        //printString("\n");
    }
    for (int i = 0; i <= 7; i++)
    {
        if (ack[i] == 1)
            return 1;
    }
    return 0;
}
    

float pressure::getTemperature(temperature_units units, precision _precision)
// Return a temperature reading in either F or C.
{
    getMeasurements(_precision);
    if (_temperature_actual == 0)
        return 0;
    float temperature_reported;
    // If Fahrenheit is selected return the temperature converted to F
    if(units == FAHRENHEIT) {
        temperature_reported = _temperature_actual / 100.0;
        temperature_reported = (((temperature_reported) * 9) / 5) + 32;
        return temperature_reported;
    }
        
    // If Celsius is selected return the temperature converted to C 
    else {
        temperature_reported = _temperature_actual / 100.0;
        return temperature_reported;
    }
}

float pressure::getPressure(precision _precision)
// Return a pressure reading units Pa.
{
    getMeasurements(_precision);
    if (_pressure_actual == 0 )
        return 0;
    float pressure_reported;
    pressure_reported = _pressure_actual;
    pressure_reported = pressure_reported / 10.0;
    return pressure_reported;
}

void pressure::getMeasurements(precision _precision)
// private function used by getPressure and getTemperature. Performs ~90% of math in library.
{
    
    //Retrieve ADC result
    int32_t temperature_raw = getADCconversion(TEMPERATURE, _precision);
    int32_t pressure_raw = getADCconversion(PRESSURE, _precision);
    
    //Create Variables for calculations
    int32_t temp_calc;
    int32_t pressure_calc;
    
    int32_t dT;
        
    //Now that we have a raw temperature, let's compute our actual.
    dT = temperature_raw - ((int32_t)coefficient[5] << 8);
    temp_calc = (((int64_t)dT * coefficient[6]) >> 23) + 2000;
    
    //Now we have our first order Temperature, let's calculate the second order.
    int64_t T2, OFF2, SENS2, OFF, SENS; //working variables

    if (temp_calc < 2000) 
    // If temp_calc is below 20.0C
    {   
        T2 = 3 * (((int64_t)dT * dT) >> 33);
        OFF2 = 3 * ((temp_calc - 2000) * (temp_calc - 2000)) / 2;
        SENS2 = 5 * ((temp_calc - 2000) * (temp_calc - 2000)) / 8;
        
        if(temp_calc < -1500)
        // If temp_calc is below -15.0C 
        {
            OFF2 = OFF2 + 7 * ((temp_calc + 1500) * (temp_calc + 1500));
            SENS2 = SENS2 + 4 * ((temp_calc + 1500) * (temp_calc + 1500));
        }
    } 
    else
    // If temp_calc is above 20.0C
    { 
        T2 = 7 * ((uint64_t)(((uint64_t)dT) * dT))/pow(2.0,37);
        OFF2 = ((temp_calc - 2000) * (temp_calc - 2000)) / 16;
        SENS2 = 0;
    }
    
    // Now bring it all together to apply offsets 
    
    OFF = ((int64_t)coefficient[2] << 16) + (((coefficient[4] * (int64_t)dT)) >> 7);
    SENS = ((int64_t)coefficient[1] << 15) + (((coefficient[3] * (int64_t)dT)) >> 8);
    
    temp_calc = temp_calc - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Now lets calculate the pressure
    

    pressure_calc = (((SENS * pressure_raw) / 2097152 ) - OFF) / 32768;
    
    _temperature_actual = temp_calc ;
    _pressure_actual = pressure_calc ;
    
}

uint32_t pressure::getADCconversion(measurement _measurement, precision _precision)
// Retrieve ADC measurement from the device.  
// Select measurement type and precision
{   
    uint32_t result;
    
    sendCommand(CMD_ADC_CONV + _measurement + _precision);
    // Wait for conversion to complete
    sensorWait(1); //general delay
    switch( _precision )
    { 
        case ADC_256 : sensorWait(1); break; 
        case ADC_512 : sensorWait(3); break; 
        case ADC_1024: sensorWait(4); break; 
        case ADC_2048: sensorWait(6); break; 
        case ADC_4096: sensorWait(10); break; 
    }   
    
    sendCommand(CMD_ADC_READ);
    char data[3];
    I2Cread(_address, data, 3);

    result = ((uint32_t)data[0] << 16) + ((uint32_t)(data[1] << 8)) + data[2];
    
    return result;

} 

int pressure::sendCommand(uint8_t command)
{
    uint8_t dataOut[1] = {command};
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(I2C_handle, _address, dataOut, 1);

    if (status != HAL_OK)
        while (HAL_I2C_GetState(I2C_handle) != HAL_I2C_STATE_READY) 
        {
            // If receiving a code on terminal, cross-reference code with HAL_StatusTypeDef definition in stm32f4xx_hal_def.h
            //printString("\r\n"); 
            //printString("I2C sending code: ");
            //printInt((uint8_t) HAL_I2C_GetState(I2C_handle));
            sensorWait(1);
        }
    else 
        return I2C_DMA_OK;
    return I2C_DMA_ERROR;
}


void pressure::sensorWait(double time)
// Delay function.  This can be modified to work outside or change time delay's order of magnitude.
// Currently measured in milli-seconds.
{
    HAL_Delay(time);
}


int pressure::I2Cread(int address, char* data, int length)
{
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive_DMA(I2C_handle, address, (uint8_t*) data, length);
    sensorWait(5);

    if (status != HAL_OK)
        while (HAL_I2C_GetState(I2C_handle) != HAL_I2C_STATE_READY) 
        {
            // If receiving a code on terminal, cross-reference code with HAL_StatusTypeDef definition in stm32f4xx_hal_def.h
            //printString("\r\n");
            //printString("read code: ");
            //printInt((uint8_t) HAL_I2C_GetState(I2C_handle));
            sensorWait(1);
        }
    else 
        return I2C_DMA_OK;
    return I2C_DMA_ERROR;
}


double pressure::sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
    return(P/pow(1-(A/44330.0),5.255));
}


double pressure::altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
    return(44330.0*(1-pow(P/P0,1/5.255)));
}





/******************************************************************************
        INIT FUNCTIONS:                     may be removed to save space, 
                                            saved so they are not lost.
******************************************************************************/

/* I2C1 init function */
void pressure::MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c1);

}

/** 
  * Enable DMA controller clock
  */
void pressure::MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

void pressure::HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

void pressure::HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
    /* USER CODE BEGIN I2C1_MspInit 0 */

    /* USER CODE END I2C1_MspInit 0 */

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __I2C1_CLK_ENABLE();

    /* Peripheral DMA init*/

    hdma_i2c1_rx.Instance = DMA1_Stream0;
    hdma_i2c1_rx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_i2c1_rx);

    __HAL_LINKDMA(hi2c,hdmarx,hdma_i2c1_rx);

    hdma_i2c1_tx.Instance = DMA1_Stream1;
    hdma_i2c1_tx.Init.Channel = DMA_CHANNEL_0;
    hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_i2c1_tx);

    __HAL_LINKDMA(hi2c,hdmatx,hdma_i2c1_tx);

    /* USER CODE BEGIN I2C1_MspInit 1 */

    /* USER CODE END I2C1_MspInit 1 */
  }

}