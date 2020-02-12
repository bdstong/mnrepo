/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "TaskApp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512
};
/* Definitions for ethTask */
osThreadId_t ethTaskHandle;
const osThreadAttr_t ethTask_attributes = {
  .name = "ethTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 512
};
/* Definitions for usart1Task */
osThreadId_t usart1TaskHandle;
const osThreadAttr_t usart1Task_attributes = {
  .name = "usart1Task",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 512
};
/* Definitions for serverTask */
osThreadId_t serverTaskHandle;
const osThreadAttr_t serverTask_attributes = {
  .name = "serverTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 1024
};
/* Definitions for ethMsgQueue */
osMessageQueueId_t ethMsgQueueHandle;
const osMessageQueueAttr_t ethMsgQueue_attributes = {
  .name = "ethMsgQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern uint32_t RunTime;

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartEthTask(void *argument);
void StartUsart1Task(void *argument);
void StartServerTask(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ethMsgQueue */
  ethMsgQueueHandle = osMessageQueueNew (16, sizeof(ETH_MSG), &ethMsgQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ethTask */
  ethTaskHandle = osThreadNew(StartEthTask, NULL, &ethTask_attributes);

  /* creation of usart1Task */
  usart1TaskHandle = osThreadNew(StartUsart1Task, NULL, &usart1Task_attributes);

  /* creation of serverTask */
  serverTaskHandle = osThreadNew(StartServerTask, NULL, &serverTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  //tcp_socket_svr_init();
  /* Infinite loop */
  for(;;)
  {
	LED0_T;
    osDelay(100);
	RunTime++;
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartEthTask */
/**
* @brief Function implementing the ethTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartEthTask */
__weak void StartEthTask(void *argument)
{
  /* USER CODE BEGIN StartEthTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
  /* USER CODE END StartEthTask */
}

/* USER CODE BEGIN Header_StartUsart1Task */
/**
* @brief Function implementing the usart1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsart1Task */
__weak void StartUsart1Task(void *argument)
{
  /* USER CODE BEGIN StartUsart1Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUsart1Task */
}

/* USER CODE BEGIN Header_StartServerTask */
/**
* @brief Function implementing the serverTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartServerTask */
__weak void StartServerTask(void *argument)
{
  /* USER CODE BEGIN StartServerTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartServerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
