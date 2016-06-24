/* Put all of your #includes and #defines up here   */
#include "temp.h"

/*
 * This library uses I2C to communicate with the Blue Robotics TSYS01 Temperature sensor
 * the sensor must be initialized before it is read the first time, or the result will be unpredictable
 *
 * To read temperature from the sensor run the 'read()' function,
 * followed by the 'getTemp()' function.
 *
 */

Temp::Temp(I2C_HandleTypeDef* handler) {

    /*this variable is what you plug into the send function */
    I2C_handler = handler;
}

// Reset the sensor and read constants
void Temp::init(void) {
	data[0] = TSYS01_RESET;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, TSYS01_ADDR, data, 1);
	HAL_Delay(10);
	for (uint8_t i = 0; i < 8; i++) {
		data[0] = TSYS01_PROM_READ + i * 2;
		HAL_I2C_Master_Transmit_DMA(I2C_handler, TSYS01_ADDR, data, 1);
		while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

		HAL_I2C_Master_Receive_DMA(I2C_handler, TSYS01_ADDR, data, 2);
		Constants[i] = (data[0] << 8) | data[1];
	}
}

void Temp::ADC_begin(void){
	data[0] = TSYS01_ADC_TEMP_CONV;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, TSYS01_ADDR, data, 1);
}

// Read temperature from the sensor, must be run to update temperature value
void Temp::read(void) {

	data[0] = TSYS01_ADC_READ;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, TSYS01_ADDR, data, 1);
	
	// This should probably go away
	// while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY)
	HAL_Delay(1);


	HAL_I2C_Master_Receive_DMA(I2C_handler, TSYS01_ADDR, raw_temp, 3);

	rawInfo = raw_temp[0];
	rawInfo = ((uint32_t)rawInfo << 8) | (uint32_t)raw_temp[1];
	rawInfo = ((uint32_t)rawInfo << 8) | (uint32_t)raw_temp[2];

	calculate();
}

void Temp::calculate(void) {
	adc = rawInfo  / 256;


/*
TEMP = (-2) * float(Constants[1]) / 1000000000000000000000.0f * pow(adc,4) + 
        4 * float(Constants[2]) / 10000000000000000.0f * pow(adc,3) +
	  (-2) * float(Constants[3]) / 100000000000.0f * pow(adc,2) +
   	    1 * float(Constants[4]) / 1000000.0f * adc +
      (-1.5) * float(Constants[5]) / 100 ;
*/
	uint16_t C[8];

	C[0] = 0;
	C[1] = 28446;  //0xA2 K4
	C[2] = 24926;  //0XA4 k3
 	C[3] = 36016;  //0XA6 K2
	C[4] = 32791;  //0XA8 K1
	C[5] = 40781;  //0XAA K0
	C[6] = 0;
	C[7] = 0;

TEMP = (-2) * float(C[1]) / 1000000000000000000000.0f * pow(adc,4) + 
        4 * float(C[2]) / 10000000000000000.0f * pow(adc,3) +
	  (-2) * float(C[3]) / 100000000000.0f * pow(adc,2) +
   	    1 * float(C[4]) / 1000000.0f * adc +
      (-1.5) * float(C[5]) / 100 ;


}

// Returns the calculated temperature
float Temp::getTemp(void) {
	return TEMP;
}





