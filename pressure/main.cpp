//put all of your #includes into main.h file
#include "main.h"
#include "print.h"

/* structure used to initialize the gpio pin */
static GPIO_InitTypeDef  GPIO_InitStruct;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;


int main(void)  {
	  

    //must be included to initially configure the library
	HAL_Init();
	SystemClock_Config();
	initPrint();
	
	printString("\r\n");
	printString("1");
	printString("2");
	  
	//enable the led clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	printString("3");

  
	//configures the led pin  
	GPIO_InitStruct.Pin = GPIO_PIN_5;  //pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin A5 
	printString("4");

	MX_GPIO_Init();
	printString("5");
	MX_DMA_Init();
	printString("6");
	MX_I2C1_Init();
	printString("7");

	HAL_I2C_MspInit(&hi2c1);

	// Begin pressure sensor code:

	pressure sensor(ADDRESS_HIGH, &hi2c1);
	printString("8");

	float temperature_c;
    float temperature_f;
    double pressure_abs;
    double pressure_relative;
    double altitude_delta;
    double pressure_baseline;

    sensor.reset();
	printString("9");
    sensor.begin();
	printString("10");

    pressure_baseline = sensor.getPressure(ADC_4096);
	printString("11");
    double base_altitude = 1655.0; // Altitude of SparkFun's HQ in Boulder, CO. in (m)


	while (1) {

		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		// To measure to higher degrees of precision use the following sensor settings:
        // ADC_256 
        // ADC_512 
        // ADC_1024
        // ADC_2048
        // ADC_4096
          
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
          
          
        // Report values via UART
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

		// 500ms delay
		HAL_Delay(2000);
	}
}



