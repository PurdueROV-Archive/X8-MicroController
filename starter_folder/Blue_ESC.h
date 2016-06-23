#ifndef BLUE_ESC_H
#define BLUE_ESC_H


#define ACK 0
#define ERR_INVALID_RANGE 10
#define ERR_I2C_READ_FAIL 20
#define ERR_I2C_WRITE_FAIL 30

//as found in the Arduino example. Some values may need to be updated depending on our motor
#define MAX_ESCS 16

// THERMISTOR SPECIFICATIONS
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3900
// the value of the 'other' resistor
#define SERIESRESISTOR 3300 

class Blue_ESC {    
public:
    Blue_ESC(uint8_t address, uint8_t poleCount);
    void update();
    float voltage();
    bool isAlive();
    float current();
    float temperature();
    int16_t rpm();
    void set(float percent);
    void setRaw(int rawThrottle);
    
private:
    int16_t convertBytesToInt(uint8_t byte_h, uint8_t byte_l);
    
    uint8_t _address;
    uint16_t _voltage_raw, _current_raw, _temp_raw;
    int16_t _rpm;
    uint16_t _rpmTimer;
    uint8_t _identifier;
    uint8_t _poleCount;
    
    static void readBuffer(uint8_t address, uint8_t buffer[]);
};
    
#endif