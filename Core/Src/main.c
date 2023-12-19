/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include "time.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BASE_FREQ 130
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
  for (uint8_t i = 0; i <= 0x0C; i += 4)
  {
    __HAL_TIM_SET_COMPARE(&htim1, i, BASE_FREQ);
    __HAL_TIM_SET_COMPARE(&htim2, i, BASE_FREQ);
    __HAL_TIM_SET_COMPARE(&htim3, i, BASE_FREQ);
    __HAL_TIM_SET_COMPARE(&htim4, i, BASE_FREQ);
    __HAL_TIM_SET_COMPARE(&htim5, i, BASE_FREQ);
    __HAL_TIM_SET_COMPARE(&htim12, i, BASE_FREQ);
  }

  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_Base_Start(&htim4);
  HAL_TIM_Base_Start(&htim5);
  HAL_TIM_Base_Start(&htim12);

  for (uint8_t i = 0; i <= 0x0C; i += 4)
  {
    HAL_TIM_OC_Start_IT(&htim1, i);
    HAL_TIM_OC_Start_IT(&htim2, i);
    HAL_TIM_OC_Start_IT(&htim3, i);
    HAL_TIM_OC_Start_IT(&htim4, i);
    HAL_TIM_OC_Start_IT(&htim5, i);
    HAL_TIM_OC_Start_IT(&htim12, i);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t channel_duty1 = BASE_FREQ, channel_duty2 = BASE_FREQ, channel_duty3 = BASE_FREQ, channel_duty4 = BASE_FREQ;
  static uint8_t channel_toggle1 = 0, channel_toggle2 = 0, channel_toggle3 = 0, channel_toggle4 = 0;

  /*获取当前计数*/
  uint32_t g_count_val = __HAL_TIM_GET_COUNTER(htim);
  __NOP();
  __NOP();
  __NOP();
  __NOP();
  __NOP();

  /*设置比较数值*/
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    if (channel_toggle1 == 0)
    {
      channel_toggle1++;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (g_count_val + channel_duty1));
    }
    else
    {
      channel_duty1 = Random_ccr();
      channel_toggle1 = 0;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (g_count_val + channel_duty1));
    }
  }

  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    if (channel_toggle2 == 0)
    {
      channel_toggle2++;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (g_count_val + channel_duty2));
    }
    else
    {
      channel_duty2 = Random_ccr();
      channel_toggle2 = 0;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (g_count_val + channel_duty2));
    }
  }

  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
    if (channel_toggle3 == 0)
    {
      channel_toggle3++;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (g_count_val + channel_duty3));
    }
    else
    {
      channel_duty3 = Random_ccr();
      channel_toggle3 = 0;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (g_count_val + channel_duty3));
    }
  }

  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
    if (channel_toggle4 == 0)
    {
      channel_toggle4++;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, (g_count_val + channel_duty4));
    }
    else
    {
      channel_duty4 = Random_ccr();
      channel_toggle4 = 0;
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, (g_count_val + channel_duty4));
    }
  }
}

uint8_t Random_ccr(void)
{
  srand(HAL_GetTick());
  uint8_t rc = rand() % 30;
  return (rc + BASE_FREQ);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
