/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
typedef StaticTask_t osStaticThreadDef_t;
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
uint32_t defaultTaskBuffer[ 512 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for usart1Task */
osThreadId_t usart1TaskHandle;
uint32_t usart1TaskBuffer[ 512 ];
osStaticThreadDef_t usart1TaskControlBlock;
const osThreadAttr_t usart1Task_attributes = {
  .name = "usart1Task",
  .stack_mem = &usart1TaskBuffer[0],
  .stack_size = sizeof(usart1TaskBuffer),
  .cb_mem = &usart1TaskControlBlock,
  .cb_size = sizeof(usart1TaskControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for usart3Task */
osThreadId_t usart3TaskHandle;
uint32_t usart3TaskBuffer[ 512 ];
osStaticThreadDef_t usart3TaskControlBlock;
const osThreadAttr_t usart3Task_attributes = {
  .name = "usart3Task",
  .stack_mem = &usart3TaskBuffer[0],
  .stack_size = sizeof(usart3TaskBuffer),
  .cb_mem = &usart3TaskControlBlock,
  .cb_size = sizeof(usart3TaskControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for adcTask */
osThreadId_t adcTaskHandle;
uint32_t adcTaskBuffer[ 512 ];
osStaticThreadDef_t adcTaskControlBlock;
const osThreadAttr_t adcTask_attributes = {
  .name = "adcTask",
  .stack_mem = &adcTaskBuffer[0],
  .stack_size = sizeof(adcTaskBuffer),
  .cb_mem = &adcTaskControlBlock,
  .cb_size = sizeof(adcTaskControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for can1Task */
osThreadId_t can1TaskHandle;
uint32_t can1TaskBuffer[ 512 ];
osStaticThreadDef_t can1TaskControlBlock;
const osThreadAttr_t can1Task_attributes = {
  .name = "can1Task",
  .stack_mem = &can1TaskBuffer[0],
  .stack_size = sizeof(can1TaskBuffer),
  .cb_mem = &can1TaskControlBlock,
  .cb_size = sizeof(can1TaskControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for canMsgQueue */
osMessageQueueId_t canMsgQueueHandle;
const osMessageQueueAttr_t canMsgQueue_attributes = {
  .name = "canMsgQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartUsart1Task(void *argument);
void StartUsart3Task(void *argument);
void StartAdcTask(void *argument);
void StartCan1Task(void *argument);

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
  /* creation of canMsgQueue */
  canMsgQueueHandle = osMessageQueueNew (16, sizeof(CAN_MSG), &canMsgQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of usart1Task */
  usart1TaskHandle = osThreadNew(StartUsart1Task, NULL, &usart1Task_attributes);

  /* creation of usart3Task */
  usart3TaskHandle = osThreadNew(StartUsart3Task, NULL, &usart3Task_attributes);

  /* creation of adcTask */
  adcTaskHandle = osThreadNew(StartAdcTask, NULL, &adcTask_attributes);

  /* creation of can1Task */
  can1TaskHandle = osThreadNew(StartCan1Task, NULL, &can1Task_attributes);

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
__weak void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
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

/* USER CODE BEGIN Header_StartUsart3Task */
/**
* @brief Function implementing the usart3Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUsart3Task */
__weak void StartUsart3Task(void *argument)
{
  /* USER CODE BEGIN StartUsart3Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartUsart3Task */
}

/* USER CODE BEGIN Header_StartAdcTask */
/**
* @brief Function implementing the adcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAdcTask */
__weak void StartAdcTask(void *argument)
{
  /* USER CODE BEGIN StartAdcTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartAdcTask */
}

/* USER CODE BEGIN Header_StartCan1Task */
/**
* @brief Function implementing the can1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCan1Task */
__weak void StartCan1Task(void *argument)
{
  /* USER CODE BEGIN StartCan1Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCan1Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
