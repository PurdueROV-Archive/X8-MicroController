#ifndef __IMU_H
#define __IMU_H

/* puts all of the #includes and # defines that you need here */
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_i2c.h"
#include "main.h"

#define IMU_PAGE_ID             0x07
#define IMU_EULER_H_LSB         0x1a
#define IMU_LINEAR_ACC_X_LSB    0x28
#define IMU_UNIT_SEL            0x3b

#define IMU_CHIP_ID             0xa0
#define IMU_ACC_ID              0xfb
#define IMU_MAG_ID              0x32
#define IMU_GYR_ID              0x0f

#define CONFIGMODE              0x00
#define MODE_IMU                0x08
#define MODE_COMPASS            0x09
#define MODE_M4G                0x0a
#define MODE_NDOF_FMC_OFF       0x0b
#define MODE_NDOF               0x0c

#define IMU_OPR_MODE            0x3d

class IMU {


public:
	/* Put function declarations that the user will
	   want to use from the class here */


/*constructor */
    IMU(I2C_HandleTypeDef* handler);

    bool retrieve_euler(void);

    void get_linear_accel(void);

    // Rotational
    double rX(void);

    double rY(void);

    double rZ(void);

    // Linear
    double lX(void);

    double lY(void);

    double lZ(void);



private:

    I2C_HandleTypeDef* I2C_handler;
    double xAngle;
    double yAngle;
    double zAngle;
	uint8_t dt[10];
    double la[3];   // Linear Acceleration vector, [x,y,z].
    uint8_t select_page(uint8_t page);
    void change_fusion_mode(uint8_t mode);
    uint8_t check_operating_mode(void);
    uint8_t page_flag;
    void check_id(void);
    uint8_t ready_flag;
    uint8_t  bootldr_rev_id;
    uint16_t sw_rev_id;
};

#endif
