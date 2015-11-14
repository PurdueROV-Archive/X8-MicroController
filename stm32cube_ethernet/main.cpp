#include "main.h"
#include "lwip/init.h"
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "lwip/lwip_timers.h"

static GPIO_InitTypeDef  GPIO_InitStruct;

static void SystemClock_Config(void);
static void Error_Handler(void);

int main(void)  
{
	  //must be included to initially configure the library
	  HAL_Init();
	  
	  /* Configure the system clock to 100 MHz */
	  SystemClock_Config();
	  
	//enable the led clock
	 __HAL_RCC_GPIOA_CLK_ENABLE();
		
  
	  //configures the led pin  
	  GPIO_InitStruct.Pin = GPIO_PIN_5;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 



    ETH_BSP_Config();
    Lwip_Init();
    tcp_echoserver_init();

  



	while (1)
	{
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		
	
		/* Insert a 500ms delay */
		HAL_Delay(500);
	}
}

//configures the system clcok
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

static void Error_Handler(void)
{
  while(1)
  {
  }
}


void tcp_echoserver_init(void)
{
 /* create new tcp pcb */
 tcp_echoserver_pcb = tcp_new();
 if (tcp_echoserver_pcb != NULL)
 {
 err_t err;

 /* bind echo_pcb to port 7 (ECHO protocol) */
 err = tcp_bind(tcp_echoserver_pcb, IP_ADDR_ANY, 7);

 if (err == ERR_OK)
 {
 /* start tcp listening for echo_pcb */
 tcp_echoserver_pcb = tcp_listen(tcp_echoserver_pcb);

 /* initialize LwIP tcp_accept callback function */
 tcp_accept(tcp_echoserver_pcb, tcp_echoserver_accept);
 }
 else
 {
 printf("Can not bind pcb\n");
 }
 }
 else
 {
 printf("Can not create new pcb\n");
 }
}


