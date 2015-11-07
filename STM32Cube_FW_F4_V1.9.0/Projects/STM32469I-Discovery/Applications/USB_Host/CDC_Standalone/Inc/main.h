/**
  ******************************************************************************
  * @file    USB_Host/CDC_Standalone/Inc/main.h 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "usbh_core.h"
#include "stm32469i_discovery.h"
#include "stm32469i_discovery_lcd.h"
#include "lcd_log.h"
#include "usbh_cdc.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
  CDC_DEMO_IDLE = 0,
  CDC_DEMO_WAIT,  
  CDC_DEMO_SEND,
  CDC_DEMO_RECEIVE,
}CDC_Demo_State;

typedef enum {
  CDC_SEND_IDLE = 0,
  CDC_SEND_WAIT,  
}CDC_Send_State;

typedef enum {
  CDC_RECEIVE_IDLE = 0,
  CDC_RECEIVE_WAIT,    
  CDC_RECEIVE_RECEIVE,
}CDC_Receive_State;

typedef struct _DemoStateMachine {
  __IO CDC_Demo_State          state;
  __IO CDC_Send_State          Send_state;
  __IO CDC_Receive_State       Receive_state;   
  __IO uint8_t                 select;
  __IO uint8_t                 lock;
}CDC_DEMO_StateMachine;

typedef enum {
  CDC_SELECT_MENU = 0,
  CDC_SELECT_FILE ,  
  CDC_SELECT_CONFIG,    
}CDC_DEMO_SelectMode;

typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_DISCONNECT,  
  APPLICATION_START,
  APPLICATION_READY,    
  APPLICATION_RUNNING,
}CDC_ApplicationTypeDef;

extern USBH_HandleTypeDef hUSBHost;
extern CDC_ApplicationTypeDef Appli_state;
extern CDC_DEMO_StateMachine CdcDemo;

/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Toggle_Leds(void);
void Menu_Init(void);
void CDC_Handle_Send_Menu(void);
void CDC_Handle_Receive_Menu(void);
void CDC_MenuProcess(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
