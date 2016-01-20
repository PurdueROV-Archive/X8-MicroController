/* pressure.h                                       */
/*==================================================*/
/* @version 1.01            first-working-version   */
/* @author bmaxfie                                  */


/* (not including I2C and DMA inits and setups:)
 * HOW TO USE THIS LIBRARY:
 *
 *
 * 1. Create pressure object 'sensor'.
 * 2. Call sensor.reset();
 * 3. Call sensor.begin();
 * 4. Save double baseline_pressure = sensor.getPressure(ADC_4096 <--'for most accurate results');
 *      for other calculations if necessary.
 *
 * 5. For the following, make repeated calls as necessary:
 *      6.  float temperature_c = sensor.getTemperature(CELSIUS, ADC_512);
 *      7.  float temperature_f = sensor.getTemperature(FAHRENHEIT, ADC_512);
 *      8.  double pressure_abs = sensor.getPressure(ADC_4096); <-- returns measurements in mbar.
 *      9.  double pressure_relative = sensor.sealevel(pressure_abs, base_altitude <-- 'the ground 
 *              level altitude of your geolocation');
 *      10. double altitude_delta = sensor.altitude(pressure_abs, pressure_baseline);
 */

 /* (INCLUDING I2C and DMA inits and setups:)
 * HOW TO USE THIS LIBRARY:
 *
 *
 * 1.  Define 3 structs: I2C_HandleTypeDef, and two DMA_HandleTypeDefs. one for rx, one for tx.
 *      ex:
 *          I2C_HandleTypeDef hi2c1;
 *          DMA_HandleTypeDef hdma_i2c1_rx;
 *          DMA_HandleTypeDef hdma_i2c1_tx;
 * #note: for the following init functions, if using the pressure class functions, perform #6 here, 
 *      before #2.
 * 2.  Call MX_GPIO_Init();
 * 3.  Call MX_DMA_Init();
 * 4.  Call MX_I2C1_Init();
 * 5.  Call HAL_I2C_MspInit(&hi2c1);
 * 6.  Create pressure object 'sensor'.
 * 7.  Call sensor.reset();
 * 8.  Call sensor.begin();
 * 9.  Save double baseline_pressure = sensor.getPressure(ADC_4096 <--'for most accurate results');
 *      for other calculations if necessary.
 *
 * 10. For the following, make repeated calls as necessary:
 *      6.  float temperature_c = sensor.getTemperature(CELSIUS, ADC_512);
 *      7.  float temperature_f = sensor.getTemperature(FAHRENHEIT, ADC_512);
 *      8.  double pressure_abs = sensor.getPressure(ADC_4096); <-- returns measurements in mbar.
 *      9.  double pressure_relative = sensor.sealevel(pressure_abs, base_altitude <-- 'the ground 
 *              level altitude of your geolocation');
 *      10. double altitude_delta = sensor.altitude(pressure_abs, pressure_baseline);
 */


#ifndef X8_PRESSURE
#define X8_PRESSURE

// INCLUDES:
#include <math.h>
#include "stm32f4xx_hal.h"
#include "stm32_hal_legacy.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_i2c.h"
// #include "print.h" // For uncommentable debug

// ENUMS & STRUCTS:

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



// DEFINES:

//Commands
#define CMD_RESET 0x1E // reset command 
#define CMD_ADC_READ 0x00 // ADC read command 
#define CMD_ADC_CONV 0x40 // ADC conversion command 
#define CMD_PROM 0xA0 // Coefficient location
//I2C Private function return stats
#define I2C_DMA_ERROR   1
#define I2C_DMA_OK      0


// FORWARD DECLARATIONS:
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;
void printString(char* phrase);
void printInt(int num);


// CLASSES:
class pressure 
{
    public: 
        // Constructor:
        pressure(ms5803_addr addr, I2C_HandleTypeDef* handler);
        // Inits:
        int reset(void);    // Reset device
        uint8_t begin(void); // Collect coefficients from sensor
        
        // Use Functions:
        // Return calculated temperature from sensor
        float getTemperature(temperature_units units, precision _precision);
        // Return calculated pressure from sensor
        float getPressure(precision _precision);

        double sealevel(double P, double A); // See instructions above for general use
        double altitude(double P, double A);



    private:
        // Definitions:
        int32_t _temperature_actual;
        int32_t _pressure_actual;
        I2C_HandleTypeDef* I2C_handle;
        int _address;       // Variable used to store I2C device address.
        uint16_t coefficient[8];// Coefficients;
        
        // Functions:
        void getMeasurements(precision _precision);
        uint32_t getADCconversion(measurement _measurement, precision _precision);  // Retrieve ADC result

        int sendCommand(uint8_t command);  // General I2C_DMA send command function
        int I2Cread(int address, char* data, int length);   // General I2C_DMA read function
        /// ^^^^^^^^^^^ above two functions return either I2C_DMA_ERROR or I2C_DMA_OK. Generalizations of 
        ///             HAL_StatusTypeDef returns from _hal_i2c.c function calls.
        void sensorWait(double time); // General delay function

        // private init functions... designed to be made public by future developers if inits are no longer 
        //  located in init.cpp.
        void MX_I2C1_Init(void);
        void MX_DMA_Init(void);
        void HAL_MspInit(void);
        void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
};

#endif

