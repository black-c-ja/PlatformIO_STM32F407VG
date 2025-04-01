/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "bsp_eeprom.h"
#include "atk_ms6050.h"
#include "inv_mpu.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
float pit, rol, yaw;
volatile float speed_x = 0, speed_y = 0;
volatile float cnt_x = 60.0, cnt_y = 28.0;
volatile uint8_t add_x = 60, add_y = 28;
uint8_t init_flag = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId mpu6050TaskHandle;
osThreadId oledTaskHandle;
osThreadId debugTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void MPU6050_Task(void const * argument);
void OLED_Display_Task(void const * argument);
void Debug_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of mpu6050Task */
  osThreadDef(mpu6050Task, MPU6050_Task, osPriorityBelowNormal, 0, 256);
  mpu6050TaskHandle = osThreadCreate(osThread(mpu6050Task), NULL);

  /* definition and creation of oledTask */
  osThreadDef(oledTask, OLED_Display_Task, osPriorityBelowNormal, 0, 1024);
  oledTaskHandle = osThreadCreate(osThread(oledTask), NULL);

  /* definition and creation of debugTask */
  osThreadDef(debugTask, Debug_Task, osPriorityLow, 0, 256);
  debugTaskHandle = osThreadCreate(osThread(debugTask), NULL);

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
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_MPU6050_Task */
/**
* @brief Function implementing the mpu6050Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MPU6050_Task */
void MPU6050_Task(void const * argument)
{
  /* USER CODE BEGIN MPU6050_Task */
  init_flag = atk_ms6050_init();
  if(init_flag != 0)
    printf("ATK-MS6050 init error!\r\n"); 
  else
    printf("ATK-MS6050 init success!\r\n");
  osDelay(100);
  init_flag = atk_ms6050_dmp_init();
  if(init_flag!= 0)
    printf("ATK-MS6050 DMP init error!\r\n");
  else
    printf("ATK-MS6050 DMP init success!\r\n");
  /* Infinite loop */
  for(;;)
  {
    atk_ms6050_dmp_get_data(&pit, &rol, &yaw);
    speed_x = rol * 0.05; speed_y = -pit * 0.05;
    cnt_x += speed_x; cnt_y += speed_y;
    if(cnt_x < 0) cnt_x = 0;
    if(cnt_y < 0) cnt_y = 0;
    if(cnt_x > 119) cnt_x = 119;
    if(cnt_y > 55) cnt_y = 55;
    add_x = (uint8_t)cnt_x; add_y = (uint8_t)cnt_y;
    osDelay(50);
  }
  /* USER CODE END MPU6050_Task */
}

/* USER CODE BEGIN Header_OLED_Display_Task */
/**
* @brief Function implementing the oledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_OLED_Display_Task */
void OLED_Display_Task(void const * argument)
{
  /* USER CODE BEGIN OLED_Display_Task */
  /* Infinite loop */
  for(;;)
  {
    OLED_DrawBoow(add_x,add_y);
    osDelay(50);
  }
  /* USER CODE END OLED_Display_Task */
}

/* USER CODE BEGIN Header_Debug_Task */
/**
* @brief Function implementing the debugTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Debug_Task */
void Debug_Task(void const * argument)
{
  /* USER CODE BEGIN Debug_Task */
  /* Infinite loop */
  for(;;)
  {
    USART1_Process();
    //printf("pit:%.2f rol:%.2f yaw:%.2f\r\n",pit,rol,yaw);
    osDelay(500);
  }
  /* USER CODE END Debug_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
