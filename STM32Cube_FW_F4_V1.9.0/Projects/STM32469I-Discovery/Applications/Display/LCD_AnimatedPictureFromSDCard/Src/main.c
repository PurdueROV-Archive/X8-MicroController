/**
  ******************************************************************************
  * @file    Display/LCD_AnimatedPictureFromSDCard/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   This file provides main program functions
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Applications
  * @{
  */

/** @addtogroup LCD_AnimatedPictureFromSDCard
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */
DIR directory;
char* pDirectoryFiles[MAX_BMP_FILES];
uint8_t ubNumberOfFiles = 0;
uint32_t uwBmplen = 0;
extern LTDC_HandleTypeDef  hltdc_eval;
extern DSI_HandleTypeDef hdsi_eval;
DSI_VidCfgTypeDef hdsivideo_handle;

/* Internal Buffer defined in SDRAM memory */
uint8_t *uwInternalBuffer;

/* Private function prototypes -----------------------------------------------*/
static void LCD_Config(void);
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint8_t LCD_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{  
  uint32_t counter = 0;
  uint8_t str[30];
  uwInternalBuffer = (uint8_t *)INTERNAL_BUFFER_START_ADDRESS;

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure the system clock to 200 MHz */
  SystemClock_Config();
  
  /* Configure LED3 */
  BSP_LED_Init(LED3);
  
  /*##-1- Configure LCD ######################################################*/
  LCD_Config();  
  
  /*##-2- Link the SD Card disk I/O driver ###################################*/
  if(FATFS_LinkDriver(&SD_Driver, SD_Path) != 0)
  {
    Error_Handler();
  }
  else
  {
    /*##-3- Initialize the Directory Files pointers (heap) ###################*/
    for (counter = 0; counter < MAX_BMP_FILES; counter++)
    {
      pDirectoryFiles[counter] = malloc(MAX_BMP_FILE_NAME);
      if(pDirectoryFiles[counter] == NULL)
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  Cannot allocate memory ");
        while(1)
        {
        }       
      }
    }
    
    /*##-4- Display Background picture #######################################*/
    /* Select Background Layer  */
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);
    
    /* Register the file system object to the FatFs module */
    if(f_mount(&SD_FatFs, (TCHAR const*)SD_Path, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      /* Set the Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      
      BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"  FatFs Initialization Error ");
    }
    else
    {    
      /* Open directory */
      if (f_opendir(&directory, (TCHAR const*)"/BACK") != FR_OK)
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    Open directory.. fails");
        while(1) { ; }
      }
    }
    
    if (Storage_CheckBitmapFile("BACK/image.bmp", &uwBmplen) == 0)
    {
      /* Format the string */
      Storage_OpenReadFile(uwInternalBuffer, "BACK/image.bmp");
      /* Write bmp file on LCD frame buffer */
      BSP_LCD_DrawBitmap(0, 0, uwInternalBuffer);
    }
    else
    {
      /* Set the Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      
      BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. "); 
      while(1) { ; }
    }        
    
    /*##-5- Display Foreground picture #######################################*/
    /* Select Foreground Layer  */
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_FOREGROUND);
    
    /* Decrease the foreground transparency */
    BSP_LCD_SetTransparency(LTDC_ACTIVE_LAYER_FOREGROUND, 200); 
    
    /* Get the BMP file names on root directory */
    ubNumberOfFiles = Storage_GetDirectoryBitmapFiles("/TOP", pDirectoryFiles);
    
    if (ubNumberOfFiles == 0)
    {
      for (counter = 0; counter < MAX_BMP_FILES; counter++)
      {
        free(pDirectoryFiles[counter]);
      }
      /* Set the Text Color */
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      
      BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    No Bitmap files...      ");
      while(1) { ; }
    } 
  }
  
  /* Infinite loop */
  while(1)
  { 
    counter = 0;
    
    while (counter < ubNumberOfFiles)
    {
      /* Format the string */
      sprintf ((char*)str, "TOP/%-11.11s", pDirectoryFiles[counter]);
      
      if (Storage_CheckBitmapFile((const char*)str, &uwBmplen) == 0)
      {
        /* Format the string */
        sprintf ((char*)str, "TOP/%-11.11s", pDirectoryFiles[counter]);
        
        /* Open a file and copy its content to a buffer */
        Storage_OpenReadFile(uwInternalBuffer, (const char*)str);
        
        HAL_Delay(100);
        
        /* Write bmp file on LCD frame buffer */
        BSP_LCD_DrawBitmap(0, 0, uwInternalBuffer);
        
        /* Jump to next image */
        counter++;   
      }
      else
      {
        /* Set the Text Color */
        BSP_LCD_SetTextColor(LCD_COLOR_RED); 
        
        BSP_LCD_DisplayStringAtLine(7, (uint8_t *) str);        
        BSP_LCD_DisplayStringAtLine(8, (uint8_t*)"    File type not supported. "); 
        while(1) { ; }
      }
    }
  }
}

/**
  * @brief  LCD configuration.
  * @param  None
  * @retval None
  */
static void LCD_Config(void)
{
  uint8_t lcd_status = LCD_OK;


  /* LCD DSI initialization in mode Video Burst  */
  lcd_status = LCD_Init();

  if(lcd_status != LCD_OK)
  {
    Error_Handler();
  }

  /* Activate foreground and background layers */
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_BG_LAYER_ADDRESS);
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_FOREGROUND, LCD_FG_LAYER_ADDRESS);
        
  /* Set LCD Background Layer  */
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);
  /* Clear the Background Layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set LCD Foreground Layer  */
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_FOREGROUND);
  /* Clear the Foreground Layer */ 
  BSP_LCD_Clear(LCD_COLOR_BLACK); 

  /* Configure and enable the Color Keying feature */
  BSP_LCD_SetColorKeying(1, 0); 

  /* Configure the transparency for foreground: Increase the transparency */
  BSP_LCD_SetTransparency(LTDC_ACTIVE_LAYER_FOREGROUND, 100);
  
}

static uint8_t LCD_Init(void)
{
  DSI_PLLInitTypeDef dsiPllInit;
  static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  uint32_t LcdClock  = 19200; 
  uint32_t Clockratio  = 0;
  uint32_t laneByteClk_kHz = 0;
  uint32_t                   VSA; /*!< Vertical start active time in units of lines */
  uint32_t                   VBP; /*!< Vertical Back Porch time in units of lines */
  uint32_t                   VFP; /*!< Vertical Front Porch time in units of lines */
  uint32_t                   VACT; /*!< Vertical Active time in units of lines = imageSize Y in pixels to display */
  uint32_t                   HSA; /*!< Horizontal start active time in units of lcdClk */
  uint32_t                   HBP; /*!< Horizontal Back Porch time in units of lcdClk */
  uint32_t                   HFP; /*!< Horizontal Front Porch time in units of lcdClk */
  uint32_t                   HACT; /*!< Horizontal Active time in units of lcdClk = imageSize X in pixels to display */
  
  
  /* Toggle Hardware Reset of the DSI LCD using
  * its XRES signal (active low) */
  BSP_LCD_Reset();
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();
  
/*************************DSI Initialization***********************************/  
  
  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_eval.Instance = DSI;
  
  HAL_DSI_DeInit(&(hdsi_eval));
  
#if !defined(USE_STM32469I_DISCO_REVA)
  dsiPllInit.PLLNDIV  = 125;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV2;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
#else  
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
#endif
  laneByteClk_kHz = 62500; /* 500 MHz / 8 = 62.5 MHz = 62500 kHz */
  
  /* Set number of Lanes */
  hdsi_eval.Init.NumberOfLanes = DSI_TWO_DATA_LANES;

  /* TXEscapeCkdiv = f(LaneByteClk)/15.62 = 4 */
  hdsi_eval.Init.TXEscapeCkdiv = laneByteClk_kHz/15620; 
  
  HAL_DSI_Init(&(hdsi_eval), &(dsiPllInit));
  Clockratio = laneByteClk_kHz/LcdClock;
  /* Timing parameters for all Video modes
  * Set Timing parameters of LTDC */
    /* lcd_orientation == LCD_ORIENTATION_LANDSCAPE */
    VSA  = OTM8009A_800X480_VSYNC;        /* 12  */
    VBP  = OTM8009A_800X480_VBP;          /* 12  */
    VFP  = OTM8009A_800X480_VFP;          /* 12  */
    HSA  = OTM8009A_800X480_HSYNC;        /* 120 */
    HBP  = OTM8009A_800X480_HBP;          /* 120 */
    HFP  = OTM8009A_800X480_HFP;          /* 120 */
    HACT = OTM8009A_800X480_WIDTH;        /* 800 */
    VACT = OTM8009A_800X480_HEIGHT;       /* 480 */ 
  
  hdsivideo_handle.VirtualChannelID = LCD_OTM8009A_ID;
  hdsivideo_handle.ColorCoding = LCD_DSI_PIXEL_DATA_FMT_RBG888;
  hdsivideo_handle.VSPolarity = DSI_VSYNC_ACTIVE_HIGH;
  hdsivideo_handle.HSPolarity = DSI_HSYNC_ACTIVE_HIGH;
  hdsivideo_handle.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;  
  hdsivideo_handle.Mode = DSI_VID_MODE_BURST; /* Mode Video burst ie : one LgP per line */
  hdsivideo_handle.NullPacketSize = 0xFFF;
  hdsivideo_handle.NumberOfChunks = 0;
  hdsivideo_handle.PacketSize                = HACT; /* Value depending on display orientation choice portrait/landscape */ 
  hdsivideo_handle.HorizontalSyncActive      = HSA*Clockratio;
  hdsivideo_handle.HorizontalBackPorch       = HBP*Clockratio;
  hdsivideo_handle.HorizontalLine            = (HACT + HSA + HBP + HFP)*Clockratio; /* Value depending on display orientation choice portrait/landscape */
  hdsivideo_handle.VerticalSyncActive        = VSA;
  hdsivideo_handle.VerticalBackPorch         = VBP;
  hdsivideo_handle.VerticalFrontPorch        = VFP;
  hdsivideo_handle.VerticalActive            = VACT; /* Value depending on display orientation choice portrait/landscape */
  
  /* Enable or disable sending LP command while streaming is active in video mode */
  hdsivideo_handle.LPCommandEnable = DSI_LP_COMMAND_ENABLE; /* Enable sending commands in mode LP (Low Power) */
  
  /* Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions */
  /* Only useful when sending LP packets is allowed while streaming is active in video mode */
  hdsivideo_handle.LPLargestPacketSize = 64;
  
  /* Largest packet size possible to transmit in LP mode in HFP region during VACT period */
  /* Only useful when sending LP packets is allowed while streaming is active in video mode */
  hdsivideo_handle.LPVACTLargestPacketSize = 64;
  
  
  /* Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this region */
  /* while streaming is active in video mode                                                                         */
  hdsivideo_handle.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;   /* Allow sending LP commands during HFP period */
  hdsivideo_handle.LPHorizontalBackPorchEnable  = DSI_LP_HBP_ENABLE;   /* Allow sending LP commands during HBP period */
  hdsivideo_handle.LPVerticalActiveEnable = DSI_LP_VACT_ENABLE;  /* Allow sending LP commands during VACT period */
  hdsivideo_handle.LPVerticalFrontPorchEnable = DSI_LP_VFP_ENABLE;   /* Allow sending LP commands during VFP period */
  hdsivideo_handle.LPVerticalBackPorchEnable = DSI_LP_VBP_ENABLE;   /* Allow sending LP commands during VBP period */
  hdsivideo_handle.LPVerticalSyncActiveEnable = DSI_LP_VSYNC_ENABLE; /* Allow sending LP commands during VSync = VSA period */
  
  /* Configure DSI Video mode timings with settings set above */
  HAL_DSI_ConfigVideoMode(&(hdsi_eval), &(hdsivideo_handle));
  
  /* Enable the DSI host and wrapper : but LTDC is not started yet at this stage */
  HAL_DSI_Start(&(hdsi_eval));
/*************************End DSI Initialization*******************************/ 
  
  
/************************LTDC Initialization***********************************/  
  
  /* Timing Configuration */    
  hltdc_eval.Init.HorizontalSync = (HSA - 1);
  hltdc_eval.Init.AccumulatedHBP = (HSA + HBP - 1);
  hltdc_eval.Init.AccumulatedActiveW = (HACT + HSA + HBP - 1);
  hltdc_eval.Init.TotalWidth = (HACT + HSA + HBP + HFP - 1);
  
  /* Initialize the LCD pixel width and pixel height */
  hltdc_eval.LayerCfg->ImageWidth  = HACT;
  hltdc_eval.LayerCfg->ImageHeight = VACT;   
  
  
  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384 MHz / 7 = 54.857 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 54.857 MHz / 2 = 27.429 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct); 

  /* Background value */
  hltdc_eval.Init.Backcolor.Blue = 0;
  hltdc_eval.Init.Backcolor.Green = 0;
  hltdc_eval.Init.Backcolor.Red = 0;
  hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_eval.Instance = LTDC;
  
  /* Get LTDC Configuration from DSI Configuration */
  HAL_LTDC_StructInitFromVideoConfig(&(hltdc_eval), &(hdsivideo_handle));
  
  /* Initialize the LTDC */  
  HAL_LTDC_Init(&hltdc_eval);
  
#if !defined(DATA_IN_ExtSDRAM)
  /* Initialize the SDRAM */
  BSP_SDRAM_Init();
#endif /* DATA_IN_ExtSDRAM */
  
  /* Initialize the font */
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
  
/************************End LTDC Initialization*******************************/
  
  
/***********************OTM8009A Initialization********************************/  
  
  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  *  depending on configuration set in 'hdsivideo_handle'.
  */
  OTM8009A_Init(hdsivideo_handle.ColorCoding, LCD_ORIENTATION_LANDSCAPE);
  
/***********************End OTM8009A Initialization****************************/ 
  
  return LCD_OK; 
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
  RCC_OscInitStruct.PLL.PLLM = 25;
#else
  RCC_OscInitStruct.PLL.PLLM = 8;
#endif /* USE_STM32469I_DISCO_REVA */
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
