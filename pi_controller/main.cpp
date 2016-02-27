//put all of your #includes into main.h file
#include "main.h"

#include "pi_controller.h"
#include "matrices.h"
#include "imu.h"
#include "print.h"

/* structure used to initialize the gpio pin */
GPIO_InitTypeDef  GPIO_InitStruct;
//I2C_HandleTypeDef hi2c1;


int main(void)  {

	//must be included to initially configure the library
	HAL_Init();
	SystemClock_Config();
	  
	//enable the led clock
	__HAL_RCC_GPIOA_CLK_ENABLE();

  
	//configures the led pin
	GPIO_InitStruct.Pin = GPIO_PIN_5;  //pin 5
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  //initializes the pin A5
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	HAL_I2C_MspInit(&hi2c1);

	initPrint();

	//uint8_t buffer[2] = {2, 3};
	vect6 force_output = vect6Make(0,0,0,0,0,0);
	PIController piController = PIController();
	piController.start();
	piController.setNewRotation(vect3Make(0,0,0));
	piController.setNewP(0.001);
	piController.setNewI(0.001);
	IMU imu1(&hi2c1);

	while (1) {
		imu1.get_linear_accel();
		//HAL_I2C_Master_Transmit_DMA(&hi2c1, (0x28 << 1), buffer, 2);
		if (imu1.retrieve_euler()) {
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			/*printString("\trX: ");
			printDouble(imu1.rX());
			printString("\trY: ");
			printDouble(imu1.rY());
			printString("\trZ: ");
			printDouble(imu1.rZ());
			//printString("\n");

			printString("\taX: ");
			printDouble(imu1.aX());
			printString("\taY: ");
			printDouble(imu1.aY());
			printString("\taZ: ");
			printDouble(imu1.aZ());
			printString("\n");*/

			piController.sensorInput(vect3Make((int16_t) (imu1.rX() * 1000), (int16_t) (imu1.rY() * 1000), (int16_t) (imu1.rZ() * 1000)),
				vect3Make((int16_t) (imu1.aX() * 1000), (int16_t) (imu1.aY() * 1000), (int16_t) (imu1.aZ() * 1000)), HAL_GetTick());
			force_output.R = piController.getOutput();

			printString("x: ");
			printDouble(force_output.R.x / 1000);
			printString("\ty: ");
			printDouble(force_output.R.y / 1000);
			printString("\tz: ");
			printDouble(force_output.R.z / 1000);
			printString("\n");

		}


		// 50ms delay
		//HAL_Delay(50);
	}
}



