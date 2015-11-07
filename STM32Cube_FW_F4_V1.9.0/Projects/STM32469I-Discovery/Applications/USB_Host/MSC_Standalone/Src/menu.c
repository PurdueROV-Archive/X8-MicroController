/**
  ******************************************************************************
  * @file    USB_Host/MSC_Standalone/Src/menu.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   This file implements Menu Functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
MSC_DEMO_StateMachine msc_demo;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Demo state machine.
  * @param  None
  * @retval None
  */
void Menu_Init(void)
{
  USBH_UsrLog("Starting MSC Demo");
  msc_demo.state = MSC_DEMO_START;
}

/**
  * @brief  Manages MSC Menu Process.
  * @param  None
  * @retval None
  */
void MSC_MenuProcess(void)
{
  switch(msc_demo.state)
  {
  case MSC_DEMO_START:
    if(Appli_state == APPLICATION_READY)
    {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(19, (uint8_t *)"Press User button to start read and write operations");
      
      /* Wait for User Input */
      while((BSP_PB_GetState(BUTTON_WAKEUP) != SET) && (Appli_state != APPLICATION_DISCONNECT))
      {
      }
      msc_demo.state = MSC_DEMO_FILE_OPERATIONS;
      
      /* Prevent debounce effect for user key */
      HAL_Delay(400);
      
      BSP_LCD_ClearStringLine(19);
    }
    break;
    
  case MSC_DEMO_FILE_OPERATIONS:
    /* Read and Write File Here */
    if(Appli_state == APPLICATION_READY)
    {
      MSC_File_Operations();
      
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_DisplayStringAtLine(19, (uint8_t *)"Press User button to display disk content");
      
      /* Wait for User Input */
      while((BSP_PB_GetState(BUTTON_WAKEUP) != SET) && (Appli_state != APPLICATION_DISCONNECT))
      {
      }
      msc_demo.state = MSC_DEMO_EXPLORER;
      
      /* Prevent debounce effect for user key */
      HAL_Delay(400);
      
      BSP_LCD_ClearStringLine(19);
    }
    break; 
    
  case MSC_DEMO_EXPLORER:
    /* Display disk content */
    if(Appli_state == APPLICATION_READY)
    {
      Explore_Disk("0:/", 1);
      msc_demo.state = MSC_DEMO_START;
      
      /* Prevent debounce effect for user key */
      HAL_Delay(400);
    }
    break;
    
  default:
    break;
  }
  
  if(Appli_state == APPLICATION_DISCONNECT)
  {
    Appli_state = APPLICATION_IDLE;
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("MSC device disconnected!\n");
    msc_demo.state = MSC_DEMO_START;
    msc_demo.select = 0;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
