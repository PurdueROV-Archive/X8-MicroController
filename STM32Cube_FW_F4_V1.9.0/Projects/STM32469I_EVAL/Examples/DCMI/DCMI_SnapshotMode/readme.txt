/**
  @page DCMI_SnapshotMode DCMI Capture Mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    DCMI/DCMI_SnapshotMode/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   Description of the STM32F4xx DCMI_SnapshotMode example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example provides a short description of how to use the DCMI to interface with
a camera module, capture a single image in Camera Frame Buffer (320x240 in RGB565) 
and once full frame camera is captured display it on the LCD in ARGB8888 format.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The Digital camera interface (DCMI) is configured to receive the capture from
the camera module mounted on STM32469I-EVAL evaluation board.
A DMA Peripheral to Memory is programmed between DCMI and Camera Frame buffer to receive a 320x240 RGB565
in camera frame buffer in SDRAM or SRAM on a line by line basis.
On each DCMI Line event callback, the DMA2D is used to copy and convert 
a RGB565 line from Camera frame buffer in a ARGB8888 line in LCD Frame buffer also in SDRAM or SRAM. 
When the IT DCMI End of frame occurs, the obtained LCD Frame buffer is displayed on LCD.

The camera module is configured to generate QVGA (320x240) image resolution
and the LCD is configured to display QVGA image resolution

LED1 is ON to indicate the test is OK. LED3 is toggling to indicate the CPU is still running.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note Make sure jumper JP3 of STM32469I-EVAL board is on position PC0, because PA4 is shared with the DCMI_HSYNC signal.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents 

  - DCMI/DCMI_SnapshotMode/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - DCMI/DCMI_SnapshotMode/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - DCMI/DCMI_SnapshotMode/Inc/main.h                  Header for main.c module  
  - DCMI/DCMI_SnapshotMode/Src/stm32f4xx_it.c          Interrupt handlers
  - DCMI/DCMI_SnapshotMode/Src/main.c                  Main program
  - DCMI/DCMI_SnapshotMode/Src/system_stm32f4xx.c      STM32F4xx system source file


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.
    
  - This example has been tested and validated with STM32469I-EVAL revB board and can be
    easily tailored to any other supported device and development board.      

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
