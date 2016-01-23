/* Put all of your #includes and #defines up here   */
#include "imu.h"


imu::imu(I2C_HandleTypeDef* handler) {

    /*this variable is what you plug into the send function */
    I2C_handler = handler;

	page_flag = 0xff;
	select_page(0);

	check_id();

	select_page(0);
	dt[0] = IMU_UNIT_SEL;
	dt[1] = 0x00;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	uint8_t chip_mode = 0x08;
	change_fusion_mode(chip_mode);
}

// retrieves the data from the sensor and stores it into variables
bool imu::retrieve(void) {

    /* In this function, I want you to retreive the data from the sensor and then store it into
    * xAngle, yAngle, and zAngle so that you can call getX, getY, and getZ later whenever you need
    * the angle data. This means that you must call retreive before you call the other functions
    * if you want to get new angle data*/

	uint8_t deg_or_rad;
	int16_t x, y, z;

	select_page(0);
	dt[0] = IMU_UNIT_SEL;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_Delay(50);
	deg_or_rad = (char)dt[0] & 0x04;

	dt[0] = IMU_EULER_H_LSB;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 6);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_Delay(50);

	x = (char)dt[1] << 8 | (char)dt[0];
	y = (char)dt[3] << 8 | (char)dt[2];
	z = (char)dt[5] << 8 | (char)dt[4];

	if (deg_or_rad) {
		xAngle = (double)x / 900;
		yAngle = (double)y / 900;
		zAngle = (double)z / 900;
	} else {
		xAngle = (double)x / 16;
		yAngle = (double)y / 16;
		zAngle = (double)z / 16;
	}
	return true;
}

//returns the angle with respect to the X axis
double imu::getX(void){
    return xAngle;
}

//returns the angle with respect to the Y axis
double imu::getY(void){
    return yAngle;
}

//returns the angle with respect to the Z axis
double imu::getZ(void){
    return zAngle;
}

uint8_t imu::select_page(uint8_t page) {
	if (page != page_flag){
		dt[0] = IMU_PAGE_ID;
		if (page == 1) {
			dt[1] = 1;  // select page 1
		} else {
			dt[1] = 0;  // select page 0
		}
		HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
		while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

		dt[0] = IMU_PAGE_ID;

		HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
		while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

		HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
		while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

		page_flag = dt[0];
	}
	return page_flag;
}

void imu::check_id(void) {

	select_page(0);
	dt[0] = IMU_CHIP_ID;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 7);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	if (dt[0] == IMU_CHIP_ID) {
		ready_flag = 1;
	} else {
		ready_flag = 0;
	}
	if (dt[1] == IMU_ACC_ID) {
		ready_flag |= 2;
	}
	if (dt[2] == IMU_MAG_ID) {
		ready_flag |= 4;
	}
	if (dt[2] == IMU_MAG_ID) {
		ready_flag |= 8;
	}
	bootldr_rev_id = dt[5] << 8 | dt[4];
	sw_rev_id = dt[6];
}

void imu::change_fusion_mode(uint8_t mode) {
	uint8_t current_mode;

	select_page(0);
	current_mode = check_operating_mode();
	switch (mode) {
		case CONFIGMODE:
			dt[0] = IMU_OPR_MODE;
			dt[1] = mode;

			HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
			while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

			HAL_Delay(19);    // wait 19mS
			break;
		case MODE_IMU:
		case MODE_COMPASS:
		case MODE_M4G:
		case MODE_NDOF_FMC_OFF:
		case MODE_NDOF:
			if (current_mode != CONFIGMODE) {   // Can we change the mode directry?
				dt[0] = IMU_OPR_MODE;
				dt[1] = CONFIGMODE;
				HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
				while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

				HAL_Delay(19);    // wait 19mS
			}
			dt[0] = IMU_OPR_MODE;
			dt[1] = mode;
			HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
			while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

			HAL_Delay(7);
			break;
		default:
			break;
	}
}

uint8_t imu::check_operating_mode(void) {
	select_page(0);
	dt[0] = IMU_OPR_MODE;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	return dt[0];
}


