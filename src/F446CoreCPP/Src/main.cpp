/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <main.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <vector>

#include "pazzle_project.h"
#include "FLGManager.h"
#include "LEDMAT.h"

//#include "LEDMATDISPTEST.h"
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
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
FLGManager fmng;
LEDMAT led_mat(32);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void test_cp_ledmat(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief This function handles System tick timer.
  */
extern "C" void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */
    static int time_1s = 1;
    static int pwm = 0;
    static int line = 0;
    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */
    if (time_1s == TICKS_1S){
    	fmng.set_flg(EN_1s);
    	time_1s = 1;
    //		if(USART2->SR & USART_SR_TXE_Msk)
    //			USART2->DR = 'U';
    //		piece_select = (piece_select+1) % 16 ;
    } else {
        time_1s++;
    }

//    if(my_usart.is_data_recv()){
//    	fmng->set_flg(RECV_USART);
//    }

    led_mat.send_line(pwm, line, 23);

	if (line>=15){
		line = 0;
		pwm = (pwm+1)%3;
	}else{
		line++;
	}


    /* USER CODE END SysTick_IRQn 1 */
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */
	int cnt = 0;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */
	uint32_t sysClk;
	SystemCoreClockUpdate();
	sysClk = SystemCoreClock;

	SysTick_Config(sysClk / TICKS_1S);

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */
	GPIOA->ODR = 0x0;
	GPIOB->ODR = 0x0;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */
		test_cp_ledmat();
		cnt=cnt+1;
		/* USER CODE BEGIN 3 */
	}
/* USER CODE END 3 */
}

void test_cp_ledmat(){
	static int font1_data[4][8]={
		{0x20,0x7c,0x20,0x3c,0x6a,0xb2,0x64,0x0},
		{0x0,0x88,0x84,0x82,0x82,0x50,0x20,0x0},
		{0x78,0x0,0x78,0x84,0x4,0x8,0x70,0x0},
		{0x78,0x0,0xf8,0x10,0x30,0x50,0x8c,0x0}
	};
	static int font2_data[4][8]={
		{0xfc,0x84,0x84,0xfc,0x84,0x84,0xfc,0x0},
		{0x3e,0x22,0x3e,0x22,0x3e,0x42,0x86,0x0},
		{0x10,0x10,0x54,0x58,0x10,0x28,0xc6,0x0},
		{0x10,0x12,0xf4,0x38,0x54,0x92,0x30,0x0}
	};
	static int font3_data[4][8]={
		{0x10,0xfe,0x10,0x38,0x54,0x92,0x10,0x0},
		{0x10,0x28,0xfe,0x10,0xfe,0x54,0xfe,0x0},
		{0x10,0x10,0x7c,0x10,0x10,0x10,0xfe,0x0},
		{0x28,0x7e,0xc8,0x5c,0x5c,0x6a,0x48,0x0}
	};
	static int font4_data[4][8]={
		{0x8,0x7e,0x52,0x5e,0x72,0x5a,0x96,0x0},
		{0x10,0xfe,0x92,0x92,0xfe,0x10,0x10,0x0},
		{0x5c,0x94,0x5c,0xb4,0x5c,0xd4,0x7e,0x0},
		{0x98,0x48,0x8,0xd4,0x62,0x40,0xbe,0x0}
	};
	static int data[4][8];
	static int color[4] = {
		0x300, 0x030, 0x003, 0x333
	};
	static char mes[32][36];
	for (int i = 0; i < 8; ++i) {
		data[0][i]  = 0x00000000;
		data[0][i] |=(font1_data[0][i])<<24;
		data[0][i] |=(font1_data[1][i])<<16;
		data[0][i] |=(font1_data[2][i])<<8;
		data[0][i] |=(font1_data[3][i]);

		data[1][i]  = 0x00000000;
		data[1][i] |=(font2_data[0][i])<<24;
		data[1][i] |=(font2_data[1][i])<<16;
		data[1][i] |=(font2_data[2][i])<<8;
		data[1][i] |=(font2_data[3][i]);

		data[2][i]  = 0x00000000;
		data[2][i] |=(font3_data[0][i])<<24;
		data[2][i] |=(font3_data[1][i])<<16;
		data[2][i] |=(font3_data[2][i])<<8;
		data[2][i] |=(font3_data[3][i]);

		data[3][i]  = 0x00000000;
		data[3][i] |=(font4_data[0][i])<<24;
		data[3][i] |=(font4_data[1][i])<<16;
		data[3][i] |=(font4_data[2][i])<<8;
		data[3][i] |=(font4_data[3][i]);
	}
	for (int i = 0; i < 4; ++i) {
		led_mat.cpMat(8*i, 0, 32, 8, color[i], data[i]);
	}
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			switch(led_mat[i][j]()){
			case 0x300:
				mes[i][j] = 'R';
				break;
			case 0x030:
				mes[i][j] = 'G';
				break;
			case 0x003:
				mes[i][j] = 'B';
				break;
			case 0x333:
				mes[i][j] = 'W';
				break;
			default:
				mes[i][j] = 'x';
				break;
			}
		}
		mes[i][32] = 0;
	}
	mes[31][33] = mes[31][33] + 0;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 320;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LEDMAT_A0_PIN0_Pin|LEDMAT_A1_PIN1_Pin|LEDMAT_A2_PIN2_Pin|LEDMAT_D13_PIN3_Pin
                          |LEDMAT_D12_R0_Pin|LEDMAT_D11_R1_Pin|LEDMAT_D7_G0_Pin|LEDMAT_D8_G1_Pin
                          |LEDMAT_D2_B0_Pin|LEDMAT_PA11_B1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LEDMAT_D4_CLK_Pin|LEDMAT_PB6_STB_Pin|LEDMAT_PB7_OE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3
                           PC4 PC5 PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LEDMAT_A0_PIN0_Pin LEDMAT_A1_PIN1_Pin LEDMAT_A2_PIN2_Pin LEDMAT_D13_PIN3_Pin
                           LEDMAT_D12_R0_Pin LEDMAT_D11_R1_Pin LEDMAT_D7_G0_Pin LEDMAT_D8_G1_Pin
                           LEDMAT_D2_B0_Pin LEDMAT_PA11_B1_Pin */
  GPIO_InitStruct.Pin = LEDMAT_A0_PIN0_Pin|LEDMAT_A1_PIN1_Pin|LEDMAT_A2_PIN2_Pin|LEDMAT_D13_PIN3_Pin
                          |LEDMAT_D12_R0_Pin|LEDMAT_D11_R1_Pin|LEDMAT_D7_G0_Pin|LEDMAT_D8_G1_Pin
                          |LEDMAT_D2_B0_Pin|LEDMAT_PA11_B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LEDMAT_D4_CLK_Pin LEDMAT_PB6_STB_Pin LEDMAT_PB7_OE_Pin */
  GPIO_InitStruct.Pin = LEDMAT_D4_CLK_Pin|LEDMAT_PB6_STB_Pin|LEDMAT_PB7_OE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
