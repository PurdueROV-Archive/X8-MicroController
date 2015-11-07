/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Src/cdc_receive.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   CDC Receive state machine
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

/* Private define ------------------------------------------------------------*/
#define RX_BUFF_SIZE   0x400  /* Max Received data 1KB */

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t xPos, yLinePos;
uint8_t CDC_RX_Buffer[RX_BUFF_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void DumpReceivedData(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handles CDC Receive Menu. 
  * @param  None
  * @retval None
  */
void CDC_Handle_Receive_Menu(void)
{
  switch(CdcDemo.Receive_state)
  {
  case CDC_RECEIVE_IDLE:
    /* Start Reception */
    LCD_LOG_ClearTextZone();
    CdcDemo.Receive_state = CDC_RECEIVE_WAIT;
    USBH_CDC_Stop(&hUSBHost);
    LCD_DbgLog("Receiving data ...\n");
    
    xPos = 0;
    yLinePos = 5;
    
    memset(CDC_RX_Buffer, 0, RX_BUFF_SIZE); 
    USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
    
    BSP_LCD_ClearStringLine(19);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAtLine(19, (uint8_t *)"Press User button to start sending data ");
    break;
    
  case CDC_RECEIVE_WAIT:
    if((BSP_PB_GetState(BUTTON_WAKEUP) == SET) && (Appli_state != APPLICATION_DISCONNECT))
    {
      CdcDemo.state =  CDC_DEMO_WAIT;
      CdcDemo.Receive_state = CDC_RECEIVE_IDLE;
    }
    break;
    
  default:
    break;
  }
}

/**
  * @brief  CDC data receive callback.
  * @param  phost: Host handle
  * @retval None
  */
void USBH_CDC_ReceiveCallback(USBH_HandleTypeDef *phost)
{
  DumpReceivedData();
  USBH_CDC_Receive(&hUSBHost, CDC_RX_Buffer, RX_BUFF_SIZE);
}

/**
  * @brief  Displays received data
  * @param  data: Keyboard data to be displayed
  * @retval None
  */
static void DumpReceivedData(void)
{
  uint16_t size;
  uint8_t *ptr = CDC_RX_Buffer;
  
  size = USBH_CDC_GetLastReceivedDataSize(&hUSBHost);
  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
  
  while(size--)
  {
    if((*ptr != '\n') && (*ptr != '\r'))
    { 
      if(*ptr == '\t')
      {
         BSP_LCD_DisplayChar(xPos, LINE(yLinePos), ' ');
      }
      else
      {
        BSP_LCD_DisplayChar(xPos, LINE(yLinePos), *ptr);
      }
      xPos += 10;
    }
    else if(*ptr == '\n')
    {
      yLinePos++;
      xPos = 0;
    }
    
    ptr++;
    
    if(xPos > (BSP_LCD_GetXSize() - 7))
    {
      xPos = 0;
      yLinePos++;
    }
    
    if(yLinePos > 13)
    {
      xPos = 0;
      yLinePos = 5;
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
