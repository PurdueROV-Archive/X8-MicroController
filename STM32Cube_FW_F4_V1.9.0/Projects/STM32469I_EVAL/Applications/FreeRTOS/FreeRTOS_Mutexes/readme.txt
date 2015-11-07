/**
  @page FreeRTOS_Mutexes FreeRTOS Mutexes application
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_Mutexes/readme.txt
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   Description of the FreeRTOS Mutexes application.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  @endverbatim

@par Description

This directory contains a set of source files that implement an application
that uses mutexes with CMSIS RTOS API 

This application creates three threads with different priorities that access the same mutex.

MutexHighPriorityThread() has the highest priority, thus executes 
first and grabs the mutex and sleeps for a short period to let the lower 
priority threads execute.  When it has completed its demo functionality
it gives the mutex back before suspending itself. 
 
MutexMeduimPriorityThread() attempts to access the mutex by performing
a blocking 'wait'. This thread blocks when the mutex is already taken 
by the high priority one. It does not unblock until the highest 
priority thread  has released the mutex, and it does not actually run until 
the highest priority thread has suspended itself.
When it eventually does obtain the mutex all it does is give the mutex back
prior to also suspending itself. At this point both the high and medium
priority threads are suspended.

MutexLowPriorityThread() runs at the idle priority. It spins round
a tight loop attempting to obtain the mutex with a non-blocking call. As
the lowest priority thread it will not successfully obtain the mutex until
both high and medium priority threads are suspended. Once it eventually 
does obtain the mutex it first resumes both suspended threads prior to giving
the mutex back - resulting in the low priority thread temporarily inheriting
the highest thread priority.       

Add the following variables to LiveWatch, these variables must remain equal all the time:
 - HighPriorityThreadCycles
 - MediumPriorityThreadCycles
 - LowPriorityThreadCycles

STM32 Eval board's LEDs can be used to monitor the application status:
  - LED1, LED2 and LED4 should toggle when the application runs successfully.
  - LED3 is ON when any error occurs.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to the OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents

    - FreeRTOS/FreeRTOS_Mutexes/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Mutexes/Inc/stm32f4xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Mutexes/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Mutexes/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Mutexes/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Mutexes/Src/stm32f4xx_it.c        Interrupt handlers
    - FreeRTOS/FreeRTOS_Mutexes/Src/system_stm32f4xx.c    STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This example runs on STM32F469xx and STM32F479xx devices.
  
  - This example has been tested with STM32469I-EVAL revB board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
