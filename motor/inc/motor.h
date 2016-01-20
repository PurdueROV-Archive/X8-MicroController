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
    motor(I2C_HandleTypeDef* handler);

    



private:

    I2C_HandleTypeDef* I2C_handler;
    uint8_t motorAddress = 0x29;

    uint16_t _voltage_raw[8], _current_raw[8], _temp_raw[8];
    int16_t _rpm[NUM_MOTORS];
    uint16_t _rpmTimer[NUM_MOTORS];
    uint8_t _identifier[NUM_MOTORS];
    uint8_t _poleCount[NUM_MOTORS];

    static void readBuffer(uint8_t address, uint8_t buffer[]);

    void readSensors(uint8_t address, uint16_t *rpm, uint16_t *vbat, uint16_t *temp, uint16_t *curr);

};

#endif
