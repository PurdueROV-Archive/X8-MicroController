/* Put all of your #includes and #defines up here   */
#include "imu.h"

/*
 * This library uses I2C to communicate with the BNO055 Inertial Measurement Unit.
 * Upon request, the IMU will send Euler angles x y and z in degrees.
 * To set up communication with the IMU, use the constructor IMU( I2C handler pointer ).
 * 		where 'I2C handler pointer' is a pointer of type I2C_HandleTypeDef*
 *
 *
 * To read angles from the IMU, run the retrieve() function
 * which will receive and parse the angles and store them in the private variables
 * xAngle, yAngle, and zAngle
 * These variables are available to the user using the functions getX(), getY(), or getZ().
 *
 */

IMU::IMU(I2C_HandleTypeDef* handler) {

	/*this variable is what you plug into the send function */
	I2C_handler = handler;

	page_flag = 0xff;
	select_page(0);

	check_id();

	select_page(0);
	dt[0] = IMU_UNIT_SEL;
	dt[1] = 0x00;
	la[0] = 0.0;
	la[1] = 0.0;
	la[2] = 0.0;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 2);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	uint8_t chip_mode = 0x08;
	change_fusion_mode(chip_mode);
}

// retrieves the data from the sensor and stores it into variables
bool IMU::retrieve_euler(void) {

	/* this function retrieves the data from the sensor and then stores it into
     * xAngle, yAngle, and zAngle so that you can call getX, getY, and getZ later whenever you need
     * the angle data. This means that you must call retrieve before you call the other functions
     * if you want to get new angle data
     */

	uint8_t deg_or_rad;
	int16_t x, y, z;

	select_page(0);
	dt[0] = IMU_UNIT_SEL;

	// Request data? Unit
	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	// Received or ready to recive? Better to make sure we call it at a suitable rate than adding a delay here.
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	//HAL_Delay(50);
	deg_or_rad = (char)dt[0] & 0x04;

	dt[0] = IMU_EULER_H_LSB;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 6);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	//HAL_Delay(50);

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


void IMU::get_linear_accel(void)
{
	uint8_t ms2_or_mg;
	int16_t x,y,z;

	select_page(0);
	dt[0] = IMU_UNIT_SEL;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);
	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);
	if (dt[0] & 0x01) {
		ms2_or_mg = 1; // mg
	} else {
		ms2_or_mg = 0; // m/s*s
	}
	dt[0] = IMU_LINEAR_ACC_X_LSB;
	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);
	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 6);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);
	x = dt[1] << 8 | dt[0];
	y = dt[3] << 8 | dt[2];
	z = dt[5] << 8 | dt[4];
	if (ms2_or_mg) {
		la[0] = (double)x;
		la[1] = (double)y;
		la[2] = (double)z;
	} else {
		la[0] = (double)x / 100;
		la[1] = (double)y / 100;
		la[2] = (double)z / 100;
	}
}



//returns the angle with respect to the X axis
double IMU::rX(void){
	return xAngle;
}

//returns the angle with respect to the Y axis
double IMU::rY(void){
	return yAngle;
}

//returns the angle with respect to the Z axis
double IMU::rZ(void){
	return zAngle;
}

//returns the linear acceleration with respect to the X axis.
double IMU::lX(void){
	return la[0];
}

//returns the linear acceleration with respect to the Y axis.
double IMU::lY(void){
	return la[1];
}

//returns the linear acceleration with respect to the Z axis.
double IMU::lZ(void){
	return la[2];
}

uint8_t IMU::select_page(uint8_t page) {
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

void IMU::check_id(void) {

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

void IMU::change_fusion_mode(uint8_t mode) {
	/* Changes fusion mode to configure or send angles via I2C */
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

uint8_t IMU::check_operating_mode(void) {
	/* initialize operating mode, */
	select_page(0);
	dt[0] = IMU_OPR_MODE;

	HAL_I2C_Master_Transmit_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	HAL_I2C_Master_Receive_DMA(I2C_handler, (0x28 << 1), dt, 1);
	while (HAL_I2C_GetState(I2C_handler) != HAL_I2C_STATE_READY) HAL_Delay(1);

	return dt[0];
}


