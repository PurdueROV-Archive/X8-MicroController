#ifndef X8_MOTOR
#define X8_MOTOR

#define NUM_MOTORS 8

// THERMISTOR SPECIFICATIONS
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3900
// the value of the 'other' resistor
#define SERIESRESISTOR 3300

/* puts all of the #includes and # defines that you need here */
#include "main.h"

class motor {


public:
    void set(int16_t throttle);

    /* The update function reads new data from the ESC. If used, this function
     * must be called at least every 65 seconds to prevent 16-bit overflow of
     * the timer keeping track of RPM. */
    void update();

    bool isAlive();

    float voltage();

    float current();

    float temperature();

    int16_t rpm();


/*constructor */
    motor(I2C_HandleTypeDef* handler, uint8_t numberMotors);

    



private:

    void readBuffer(uint8_t address, uint8_t buffer[]);

    void readSensors(uint8_t address, uint16_t *rpm, uint16_t *vbat, uint16_t *temp, uint16_t *curr);

    uint8_t motorAddress;

    uint16_t _voltage_raw, _current_raw, _temp_raw;
    int16_t _rpm;
    uint16_t _rpmTimer;
    uint8_t _identifier;
    uint8_t _poleCount;
    I2C_HandleTypeDef* I2C_handler;
    uint8_t numMotors;

    uint8_t _buffer[9];


};

#endif
