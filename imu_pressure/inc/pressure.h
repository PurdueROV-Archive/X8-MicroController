#ifndef X8_PRESSURE
#define X8_PRESSURE

/* put all of the #includes and # defines that you need here */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_i2c.h"
#include "math.h"

// Define units for conversions. 
enum temperature_units
{
    CELSIUS,
    FAHRENHEIT,
};

// Define measurement type.
enum measurement
{   
    PRESSURE = 0x00,
    TEMPERATURE = 0x10
};

// Define constants for Conversion precision
enum precision
{
    ADC_256  = 0x00,
    ADC_512  = 0x02,
    ADC_1024 = 0x04,
    ADC_2048 = 0x06,
    ADC_4096 = 0x08
};

// Define address choices for the device (I2C mode)
enum ms5803_addr
{
    ADDRESS_HIGH = (0x76 << 1),
    ADDRESS_LOW  = (0x77 << 1)
};

//Commands
#define CMD_RESET 0x1E // reset command 
#define CMD_ADC_READ 0x00 // ADC read command 
#define CMD_ADC_CONV 0x40 // ADC conversion command 

#define CMD_PROM 0xA0 // Coefficient location

#define I2C_DMA_ERROR   1
#define I2C_DMA_OK      0


class Pressure
{
    public: 
        Pressure(ms5803_addr addr);//PinName p_sda, PinName p_scl); 
        int reset(void);    //Reset device
        uint8_t begin(void); // Collect coefficients from sensor

        I2C_HandleTypeDef hi2c1;
        DMA_HandleTypeDef hdma_i2c1_rx;
        DMA_HandleTypeDef hdma_i2c1_tx;
        
        // Return calculated temperature from sensor
        float getTemperature(temperature_units units, precision _precision);
        // Return calculated pressure from sensor
        float getPressure(precision _precision);

        double sealevel(double P, double A);
        double altitude(double P, double A);

    private:
        
        int32_t _temperature_actual;
        int32_t _pressure_actual;

        //I2C *i2c;
        I2C_HandleTypeDef hi2c;

    
        int _address;       // Variable used to store I2C device address.
        uint16_t coefficient[8];// Coefficients;
        
        void getMeasurements(precision _precision);

        int sendCommand(uint8_t command);  // General I2C send command function
        uint32_t getADCconversion(measurement _measurement, precision _precision);  // Retrieve ADC result

        void sensorWait(double time); // General delay function see: delay()
        int I2Cread(int address, char* data, int length);

        void MX_I2C1_Init(void);
        void MX_DMA_Init(void);
        void HAL_MspInit(void);
        void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
};

#endif

