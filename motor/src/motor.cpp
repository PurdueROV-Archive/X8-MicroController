/* Put all of your #includes and #defines up here   */

#include "motor.h"
#include "math.h"


motor::motor(I2C_HandleTypeDef* handler, uint8_t numberMotors) {

    /*this variable is what you plug into the send function */
    I2C_handler = handler;
    motorAddress = 0x30;
    numMotors = numberMotors;
}
// Read the incoming data buffer from an ESC
void motor::readBuffer(uint8_t address, uint8_t buffer[]) {

    uint8_t temp[1] = {0x02};

    HAL_I2C_Master_Transmit_DMA(I2C_handler, address, temp, 1);
    while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY)
    {
        HAL_Delay(1);
    }
    HAL_I2C_Master_Receive_DMA(I2C_handler, address, buffer, 9);
    while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY)
    {
        HAL_Delay(1);
    }
}

// Send motor speed command to ESC
void motor::set(int16_t throttle) {

    uint8_t temp[3] = {0x00, (throttle>>8), throttle};
    HAL_I2C_Master_Transmit(I2C_handler, motorAddress << 1, temp, 3, 100);
    while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY)
    {
        HAL_Delay(1);
    }

}

void motor::update() {
    _buffer[8] = 0x00; // Reset last byte so we can check for alive

    readBuffer(motorAddress << 1,_buffer);

    _rpm = (_buffer[0] << 8) | _buffer[1];
    _voltage_raw = (_buffer[2] << 8) | _buffer[3];
    _temp_raw = (_buffer[4] << 8) | _buffer[5];
    _current_raw = (_buffer[6] << 8) | _buffer[7];
    _identifier = _buffer[8];

   //_rpm = float(_rpm)/((uint16_t(millis())-_rpmTimer)/1000.0f)*60/float(_poleCount);
    //_rpmTimer = millis();
}

bool motor::isAlive() {
    return (_identifier == 0xab);
}

float motor::voltage() {
    return float(_voltage_raw)/65536.0f*5.0f*6.45f;
}

float motor::current() {
    return (float(_current_raw)-32767)/65535.0f*5.0f*14.706f;
}

float motor::temperature() {
    // This code was taken from an Adafruit
    float resistance = SERIESRESISTOR/(65535/float(_temp_raw)-1);

    float steinhart;
    steinhart = resistance / THERMISTORNOMINAL;  // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C

    return steinhart;
}

int16_t motor::rpm() {
    return _rpm;
}

