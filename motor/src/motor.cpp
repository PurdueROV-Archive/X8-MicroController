/* Put all of your #includes and #defines up here   */

#include "motor.h"


//HAL_I2C_Master_Transmit_DMA(&hi2c1, 2, buffer, 2);

motor::motor(I2C_HandleTypeDef* handler) {

    /*this variable is what you plug into the send function */
    I2C_handler = handler;
}

// Read the incoming data buffer from an ESC
void motor::readBuffer(uint8_t address, uint8_t buffer[]) {

    temp[1] = {0x02};

    HAL_I2C_Master_Transmit_DMA(I2C_handler, address, temp, 1);



    Wire.requestFrom(address,uint8_t(9));
    uint8_t i = 0;
    while(Wire.available()) {
        buffer[i] = Wire.read();
        i++;
    }
}

// Send motor speed command to ESC
void motor::set(int16_t throttle) {
    Wire.beginTransmission(_address);
    Wire.write(0x00);
    Wire.write(throttle>>8);
    Wire.write(throttle);
    Wire.endTransmission();
}

void motor::update() {
    _buffer[8] = 0x00; // Reset last byte so we can check for alive

    readBuffer(_address,_buffer);

    _rpm = (_buffer[0] << 8) | _buffer[1];
    _voltage_raw = (_buffer[2] << 8) | _buffer[3];
    _temp_raw = (_buffer[4] << 8) | _buffer[5];
    _current_raw = (_buffer[6] << 8) | _buffer[7];
    _identifier = _buffer[8];

    _rpm = float(_rpm)/((uint16_t(millis())-_rpmTimer)/1000.0f)*60/float(_poleCount);
    _rpmTimer = millis();
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

