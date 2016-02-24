/* main.cpp				test for pressure library   */
/*==================================================*/
/* @version 1.01            first-working-version   */
/* @author bmaxfie                                  */

// INCLUDES:
#include "main.h"
#include "print.h"


// DEFINITIONS:

/* structure used to initialize the gpio pin */
static GPIO_InitTypeDef  GPIO_InitStruct;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;



// MAIN:
int main(void)  {
	  

    //must be included to initially configure the library
	HAL_Init();
	SystemClock_Config();
	initPrint();
	  
	//enable the led clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

  
	//configures the led pin  
	GPIO_InitStruct.Pin = GPIO_PIN_5;  //pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin A5

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	HAL_I2C_MspInit(&hi2c1);

	// Begin pressure sensor code:

	pressure sensor(ADDRESS_HIGH, &hi2c1);

	float temperature_c;
    float temperature_f;
    double pressure_abs;
    double pressure_relative;
    double altitude_delta;
    double pressure_baseline;
    uint32_t lastTime;

    sensor.reset();
    sensor.begin();

    pressure_baseline = sensor.getPressure(ADC_4096);
    double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)
    // Change this value:  ^^^^^^


	while (1) {
		// Health Monitor LED, period should be ~83ms
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		// To measure to higher degrees of precision use the following sensor settings:
        // ADC_256 
        // ADC_512 
        // ADC_1024
        // ADC_2048
        // ADC_4096

		// Collect time before polling sensor:
        lastTime = HAL_GetTick();
          
        // Read temperature from the sensor in deg C. This operation takes about 
        temperature_c = sensor.getTemperature(CELSIUS, ADC_512);
          
        // Read temperature from the sensor in deg F. Converting
        // to Fahrenheit is not internal to the sensor.
        // Additional math is done to convert a Celsius reading.
        temperature_f = sensor.getTemperature(FAHRENHEIT, ADC_512);
          
        // Read pressure from the sensor in mbar.
        pressure_abs = sensor.getPressure(ADC_4096);
          
        // Let's do something interesting with our data.
          
        // Convert abs pressure with the help of altitude into relative pressure
        // This is used in Weather stations.
        pressure_relative = sensor.sealevel(pressure_abs, base_altitude);
          
        // Taking our baseline pressure at the beginning we can find an approximate
        // change in altitude based on the differences in pressure.   
        altitude_delta = sensor.altitude(pressure_abs , pressure_baseline);
          
          
        // Report values via UART, COM port monitor.
        printString("\r\nTemperature C = ");
        printDouble(temperature_c);
          
        printString("\r\nTemperature F = ");
        printDouble(temperature_f);
          
        printString("\r\nPressure abs (mbar)= ");
        printDouble(pressure_abs);
           
        printString("\r\nPressure relative (mbar)= ");
        printDouble(pressure_relative); 
          
        printString("\r\nAltitude change (m) = ");
        printDouble(altitude_delta);

        // Display time (ms) it takes to do 3 full sensor pulls and calculations
        printString("\r\nTicks = ");
        printInt(HAL_GetTick() - lastTime);

		// 500ms delay
		HAL_Delay(1);

	}
}



