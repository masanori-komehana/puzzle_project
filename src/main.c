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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx.h"

#include <stdlib.h>
#include <time.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef uint32_t * uintp;

typedef GPIO_TypeDef * GPIOx;

typedef struct TAG_POS{
	int row;
	int col;
} POS;

// 時間管理用
typedef struct TAG_PAZZLE_TIME{
	int min;
	int sec;
	int ms10;
}Pazzle_time;

//
typedef struct TAG_SW_IF{
	GPIOx reg;
	int32_t port_num;
	uint32_t swc;
	uint32_t swp1;
	uint32_t swp2;
	uint32_t now;
	uint32_t last;
	uint32_t up;
} SW_IF;

// ゲーム管理データ
typedef struct TAG_GAME_DATA{
	int flg;
	int state;
	int posedge_buttons;
	Pazzle_time * pzt;
} GAME_DATA;

// 状態
enum TState{
	// 初期状態
	STATE_INIT,
	// パズル進行中
	STATE_PAZZLE,
	//　クリア画面表示
	STATE_CLEAR,
};

// 押しボタンスイッチ
enum TButtons{
	BTN_LEFT,
	BTN_DOWN,
	BTN_UP,
	BTN_RIGHT,
	BTN_ESC,
};

enum TGeneralFlgs {
	// 画面データ要求
	CLS,
	// 区間ごとに立てるフラグ
	EN_1s,
	EN_10ms,
	// 何かのスイッチ押された
	SW_POSEDGE,
	// USBになんか来た
	RECV_USART,
};



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define waite_time 1

#define A_HI 0x1
#define B_HI 0x2
#define C_HI 0x10
#define D_HI 0x20
#define LINE_MASK 0x33
#define R0_HI 0x40
#define R1_HI 0x80
#define G0_HI 0x100
#define G1_HI 0x200
#define B0_HI 0x400
#define B1_HI 0x800
#define RGB_MASK 0xFC0
#define CLK_HI 0x20
#define STB_HI 0x40
#define OE_HI 0x80

#define TICKS_1S 3200

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define FLG(flg_val) (((uint32_t)0x00000001)<<(flg_val))

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */


volatile int clkCnt = 0;

int piece_select = 0;

//グローバル変数
int delay_time;

// LEDマトリクス--------------------------------------------------------------------
int clk=0;
int clk_count=0;
int line = 0;
int line_bits=0;
int line_data_r0;
int line_data_r1;
int line_data_g0;
int line_data_g1;
int line_data_b0;
int line_data_b1;

// 明るい方
int data_r0a[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_r1a[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_g0a[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_g1a[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_b0a[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_b1a[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

int *data_r0 = data_r0a;
int *data_r1 = data_r1a;
int *data_g0 = data_g0a;
int *data_g1 = data_g1a;
int *data_b0 = data_b0a;
int *data_b1 = data_b1a;

// 暗い方
int data_r0b[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_r1b[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_g0b[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_g1b[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_b0b[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
int data_b1b[16] ={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

// パズルの駒
int board[4][4] = {
		{ 1, 2, 3, 4},
		{ 5, 6, 7, 8},
		{ 9,10,11,12},
		{13,14,15,16},
};


#define BOARD_SIZE 4
#define BLANK_NUMBER 16


int rgb_data;

// タイマー動かすフラグ
int is_timcnt = 0;
// 時間計測タイマー
Pazzle_time pazzle_time = {
		0, 0, 0,
};

// パズル進行中フラグ
int is_play = 0;

// ゲームデータ
GAME_DATA game_dat = {
	//int flg;
	0,
	//int state;
	-1,
	//int posedge_buttons;
	0,
	//Pazzle_time * pzt;
	&pazzle_time,
};

//
int cnt_10ms = 0;

uint8_t usart_dat;

// 押しボタンスイッチ-----------------------------------------------------------------
SW_IF swc0 = {GPIOC, 0, 0, 0, 0, 0, 0, 0};
SW_IF swc1 = {GPIOC, 1, 0, 0, 0, 0, 0, 0};
SW_IF swc2 = {GPIOC, 2, 0, 0, 0, 0, 0, 0};
SW_IF swc3 = {GPIOC, 3, 0, 0, 0, 0, 0, 0};
SW_IF swc4 = {GPIOC, 4, 0, 0, 0, 0, 0, 0};

SW_IF * sw_all[5] = {
	&swc0,
	&swc1,
	&swc2,
	&swc3,
	&swc4,
};

// 駒の色データ--------------------------------------------------------------------
int rgb_data1[16][3] = {
	/**/{1, 0, 0}, {1, 0, 0}, {1, 1, 0}, {1, 1, 0},
	/**/{0, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1},
	/**/{0, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1},
	/**/{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0},
};
int rgb_data2[16][3] = {
	/**/{1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {1, 1, 0},
	/**/{0, 1, 0}, {0, 1, 1}, {0, 1, 1}, {0, 1, 1},
	/**/{0, 0, 1}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
	/**/{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {0, 0, 0},
};

//関数のプロトタイプ
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */


void  init_system(void);
void waite_func(int);


// LED Matrix

void LEDMAT(int);
void LEDMATv2(int, int, int, int, int, int, int, int);
void LEDMATv3(int, int, int, int, int, int, int, int);
void LEDMAT_old(int);

void cp_data(int, int[16][3], int[16], int[16], int[16], int[16], int[16], int[16]);

void cl_disp_data(void);

////// Show ClearTime

void show_clear_disp(Pazzle_time *);
void show_clear_lt1min(Pazzle_time *);
void show_clear_lt10min(Pazzle_time *);
void show_clear_gt10min(Pazzle_time *);
void add_10ms(Pazzle_time *);
void clr_pzltim(Pazzle_time *);
int get_7seg_dot (int, int);

////// GPIO Read

int is_push(SW_IF *);
int is_posedge(SW_IF *);

////// pazzle15 logic

void state_change(int);

void pazzle_move(int);

POS get_blank_pos(void);
void move_blank(POS);

void board_init(void);
void board_shuffle(int, int);
int is_game_clear(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void SysTick_Handler(void){
	static int time_1s = 1;
	static int data_select = 0;

	if (time_1s == TICKS_1S){
		game_dat.flg |= FLG(EN_1s);
		time_1s = 1;
//		if(USART2->SR & USART_SR_TXE_Msk)
//			USART2->DR = 'U';
//		piece_select = (piece_select+1) % 16 ;
	} else {
		time_1s++;
	}

	if(++cnt_10ms == (TICKS_1S/100)){
		cnt_10ms = 0;
		if(is_timcnt){
			add_10ms(game_dat.pzt);
		}
		for (int i = 0; i < 5; ++i) {
			if(is_posedge(sw_all[i])){
				game_dat.flg |= FLG(SW_POSEDGE);
			}
		}
	}

	;

	if(delay_time==0)
	  delay_time=0;
	else
	  delay_time--;

	if (data_select){
		LEDMATv3(23, line,
				data_r0a[line], data_r1a[line],
				data_g0a[line], data_g1a[line],
				data_b0a[line], data_b1a[line]);
	}else{
		LEDMATv3(23, line,
				data_r0b[line], data_r1b[line],
				data_g0b[line], data_g1b[line],
				data_b0b[line], data_b1b[line]);
	}
	if (line>=15){
		line = 0;
		data_select = (data_select+1)%3;
		if (game_dat.state == STATE_CLEAR){
			game_dat.flg |= FLG(CLS);
		}
	}else{
		line++;
	}

	if(USART2->SR & USART_SR_RXNE_Msk){
		game_dat.flg |= FLG(RECV_USART);
	}

}
/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */


//// カラーデータ
//
////	int rgb_data_old[16][3] = {
////		{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 1, 1},
////		{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 0},
////		{1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1},
////		{1, 0, 1}, {1, 1, 1}, {1, 1, 1}, {1, 0, 1},
////	};
////	int rgb_data[16][3] = {
////		{1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {0, 1, 1},
////		{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 0},
////		{1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1},
////		{1, 0, 1}, {1, 0, 1}, {0, 0, 1}, {0, 0, 1},
////	};
////	int rgb_data2[16][3] = {
////		{1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 1, 1},
////		{0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 0},
////		{1, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1},
////		{1, 0, 1}, {0, 0, 1}, {1, 0, 1}, {0, 0, 1},
////	};
//
	int rgb_data1[16][3] = {
		/**/{1, 0, 0}, {1, 0, 0}, {1, 1, 0}, {1, 1, 0},
		/**/{0, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1},
		/**/{0, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1},
		/**/{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0},
	};
	int rgb_data2[16][3] = {
		/**/{1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {1, 1, 0},
		/**/{0, 1, 0}, {0, 1, 1}, {0, 1, 1}, {0, 1, 1},
		/**/{0, 0, 1}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1},
		/**/{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {0, 0, 0},
	};

//	uint32_t rng_seed;
//	();



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
	/* USER CODE BEGIN 2 */
	GPIOA->ODR = 0x0;
	GPIOB->ODR = 0x0;
	//データのコピー

//	state_change(STATE_INIT);
	cl_disp_data();
	cp_data(0, rgb_data1,
			data_r0a, data_r1a,
			data_g0a, data_g1a,
			data_b0a, data_b1a);
	cp_data(1, rgb_data2,
			data_r0b, data_r1b,
			data_g0b, data_g1b,
			data_b0b, data_b1b);
	while (1)
	{
		if (game_dat.flg & FLG(CLS)){
			game_dat.flg &= ~FLG(CLS);

			switch (game_dat.state){
			case STATE_INIT:
			case STATE_PAZZLE:
				cp_data(0, rgb_data1,
						data_r0a, data_r1a,
						data_g0a, data_g1a,
						data_b0a, data_b1a);
				cp_data(1, rgb_data2,
						data_r0b, data_r1b,
						data_g0b, data_g1b,
						data_b0b, data_b1b);
				break;
			case STATE_CLEAR:
				cl_disp_data();
				show_clear_disp(&pazzle_time);
				break;
			default:
				break;
			}
		}
		if (game_dat.flg & FLG(SW_POSEDGE)){
			game_dat.flg &= ~FLG(SW_POSEDGE);
			if(game_dat.state!=-1){
				game_dat.flg |= FLG(CLS);
				cl_disp_data();
			}
			for (int i = 0; i < 5; ++i) {
				if(sw_all[i]->up){
					sw_all[i]->up = 0;
					pazzle_move(i);
				}
			}
		}
		if (game_dat.flg & FLG(EN_1s)){
			game_dat.flg &= ~FLG(EN_1s);
		}
		if (game_dat.flg & FLG(RECV_USART)){
			game_dat.flg &= ~FLG(RECV_USART);
			usart_dat = USART2->DR;
			//USART2->DR = usart_dat;
			if(game_dat.state!=-1){
				game_dat.flg |= FLG(CLS);
				cl_disp_data();
			}
			switch (usart_dat) {
			case '\e':
				pazzle_move(BTN_ESC);
				break;
			case 'w':
				pazzle_move(BTN_UP);
				break;
			case 'a':
				pazzle_move(BTN_LEFT);
				break;
			case 's':
				pazzle_move(BTN_DOWN);
				break;
			case 'd':
				pazzle_move(BTN_RIGHT);
				break;
			case 'z':
				board_init();
				break;
			case 'Z':
				state_change(STATE_CLEAR);
				break;
			case 'x':
				is_timcnt = 0;
				break;
			case 'X':
				is_timcnt = 1;
				break;
			case 'c':
				if(!is_timcnt){
					clr_pzltim(game_dat.pzt);
				}
				break;
			case 'C':
				game_dat.state = STATE_INIT;
				clr_pzltim(game_dat.pzt);
				board_init();
				board_shuffle(100, 96);
				break;
			default:
				break;
			}
		}


		while(!game_dat.flg) clk_count++;
	}
	/* USER CODE END 2 */
}


void waite_func(int time){
  delay_time = time;// * TICKS_1S / 1000;
  while(delay_time>0);
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
//	RCC_OscInitStruct.PLL.PLLM = 32;
//	RCC_OscInitStruct.PLL.PLLN = 320;
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
						   PC4 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
						  |GPIO_PIN_4;
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
void LEDMAT(int oetime){
	for(int fclk_cnt=0;fclk_cnt<32;fclk_cnt++){
		//データ取得 データシフト
		if(fclk_cnt==0){
			//元データをここに入れる
			line_data_r0 = data_r0[line];
			line_data_r1 = data_r1[line];
			line_data_g0 = data_g0[line];
			line_data_g1 = data_g1[line];
			line_data_b0 = data_b0[line];
			line_data_b1 = data_b1[line];
		} else {
			line_data_r0 = line_data_r0<<1;
			line_data_r1 = line_data_r1<<1;
			line_data_g0 = line_data_g0<<1;
			line_data_g1 = line_data_g1<<1;
			line_data_b0 = line_data_b0<<1;
			line_data_b1 = line_data_b1<<1;
		}
		//データ統合
		rgb_data = 0;
		rgb_data |= (line_data_r0 &0x80000000) >>25;
		rgb_data |= (line_data_r1 &0x80000000) >>24;
		rgb_data |= (line_data_g0 &0x80000000) >>23;
		rgb_data |= (line_data_g1 &0x80000000) >>22;
		rgb_data |= (line_data_b0 &0x80000000) >>21;
		rgb_data |= (line_data_b1 &0x80000000) >>20;
		GPIOA->ODR &= ~RGB_MASK | rgb_data ;
		GPIOA->ODR |= RGB_MASK & rgb_data ;
		//ラッチ
		if(fclk_cnt==31){
			GPIOB->ODR |=  STB_HI;      //SLATB =1
			line_bits = ((line & 0xc)<<2) |(line & 0x3);
			GPIOA->ODR &= ~LINE_MASK | line_bits ;
			GPIOA->ODR |= LINE_MASK & line_bits ;
		}
		if(fclk_cnt==0){
			GPIOB->ODR &= ~ STB_HI;     //SLATB =0
		}
		//出力イネーブル
		if(fclk_cnt==oetime){// 明るさ
			GPIOB->ODR |= OE_HI;      //OEB=1
		}
		if(fclk_cnt==0){
			GPIOB->ODR &= ~OE_HI;     //OEB=0
		}
		//クロック出力 Hi
		GPIOB->ODR |= CLK_HI;        //SCK=1
		//クロック出力 LoW
		GPIOB->ODR &= ~CLK_HI;       //SCK=0
	}
    GPIOB->ODR &= ~CLK_HI;       //SCK=0
}

void LEDMATv2(int oetime, int line_num,
		int r0_line, int r1_line,
		int g0_line, int g1_line,
		int b0_line, int b1_line){
	for(int fclk_cnt=0;fclk_cnt<32;fclk_cnt++){
		//データ取得 データシフト
		if(fclk_cnt==0){
			//元データをここに入れる
			line_data_r0 = r0_line;
			line_data_r1 = r1_line;
			line_data_g0 = g0_line;
			line_data_g1 = g1_line;
			line_data_b0 = b0_line;
			line_data_b1 = b1_line;
		}
		else{
			line_data_r0 = line_data_r0<<1;
			line_data_r1 = line_data_r1<<1;
			line_data_g0 = line_data_g0<<1;
			line_data_g1 = line_data_g1<<1;
			line_data_b0 = line_data_b0<<1;
			line_data_b1 = line_data_b1<<1;
		}
		//データ統合
		rgb_data = 0;
		rgb_data |= (line_data_r0 &0x80000000) >>25;
		rgb_data |= (line_data_r1 &0x80000000) >>24;
		rgb_data |= (line_data_g0 &0x80000000) >>23;
		rgb_data |= (line_data_g1 &0x80000000) >>22;
		rgb_data |= (line_data_b0 &0x80000000) >>21;
		rgb_data |= (line_data_b1 &0x80000000) >>20;
		GPIOA->ODR &= ~RGB_MASK | rgb_data ;
		GPIOA->ODR |= RGB_MASK & rgb_data ;
		//ラッチ
		if(fclk_cnt==31){
			GPIOB->ODR |=  STB_HI;      //SLATB =1
			line_bits = ((line & 0xc)<<2) |(line & 0x3);
			GPIOA->ODR &= ~LINE_MASK | line_bits ;
			GPIOA->ODR |= LINE_MASK & line_bits ;
		}
		if(fclk_cnt==0){
			GPIOB->ODR &= ~ STB_HI;     //SLATB =0
		}
		//出力イネーブル
		if(fclk_cnt==oetime){// 明るさ
			GPIOB->ODR |= OE_HI;      //OEB=1
		}
		if(fclk_cnt==0){
			GPIOB->ODR &= ~OE_HI;     //OEB=0
		}
		//クロック出力 Hi
		GPIOB->ODR |= CLK_HI;        //SCK=1
		//クロック出力 LoW
		GPIOB->ODR &= ~CLK_HI;       //SCK=0
	}
    GPIOB->ODR &= ~CLK_HI;       //SCK=0
}

void LEDMATv3(int oetime, int line_num,
		int r0_line, int r1_line,
		int g0_line, int g1_line,
		int b0_line, int b1_line){
	static int clk_cnt=31;
	for(int fclk_cnt=0;fclk_cnt<64;fclk_cnt++){
		// クロック立下り時の動作
		if ((clk==0)){
			// クロックフラグ反転
			clk=1;
			// クロック数カウント
			if(clk_cnt>=31){
				clk_cnt=0;
			}
			else
				clk_cnt++;
			//クロック出力 LoW
			GPIOB->ODR &= ~CLK_HI;       //SCK=0
			//データ取得 データシフト
			if(clk_cnt==0){
				line_data_r0 = r0_line;
				line_data_r1 = r1_line;
				line_data_g0 = g0_line;
				line_data_g1 = g1_line;
				line_data_b0 = b0_line;
				line_data_b1 = b1_line;
			}else{
				line_data_r0 = line_data_r0<<1;
				line_data_r1 = line_data_r1<<1;
				line_data_g0 = line_data_g0<<1;
				line_data_g1 = line_data_g1<<1;
				line_data_b0 = line_data_b0<<1;
				line_data_b1 = line_data_b1<<1;
			}
			//データ統合
			rgb_data = 0;
			rgb_data |= (line_data_r0 &0x80000000) >>25;
			rgb_data |= (line_data_r1 &0x80000000) >>24;
			rgb_data |= (line_data_g0 &0x80000000) >>23;
			rgb_data |= (line_data_g1 &0x80000000) >>22;
			rgb_data |= (line_data_b0 &0x80000000) >>21;
			rgb_data |= (line_data_b1 &0x80000000) >>20;
			//データ出力
			GPIOA->ODR &= ~RGB_MASK | rgb_data ;
			GPIOA->ODR |= RGB_MASK & rgb_data ;
			//ラッチ
			if(clk_cnt==31){
				GPIOB->ODR |=  STB_HI;      //SLATB =1
				line_bits = ((line_num & 0xc)<<2) |(line_num & 0x3);
				GPIOA->ODR &= ~LINE_MASK | line_bits ;
				GPIOA->ODR |= LINE_MASK & line_bits ;
			}
			if(clk_cnt==0){
				GPIOB->ODR &= ~ STB_HI;     //SLATB =0
			}
			//出力イネーブル
			if(clk_cnt==oetime){
				GPIOB->ODR |= OE_HI;      //OEB=1
			}
			if(clk_cnt==0){
				GPIOB->ODR &= ~OE_HI;     //OEB=0
			}
		}
		// クロック立ち上がり時の動作
		else{
			// クロックフラグ反転
			clk=0;
			//クロック出力 Hi
			GPIOB->ODR |= CLK_HI;        //SCK=1
		}
	}
    GPIOB->ODR &= ~CLK_HI;       //SCK=0
}

void LEDMAT_old(int oetime){
	for(int fclk_cnt=0;fclk_cnt<32;fclk_cnt++){
		//データ取得 データシフト
		if(fclk_cnt==0){
			//元データをここに入れる
			line_data_r0 = data_r0[line];
			line_data_r1 = data_r1[line];
			line_data_g0 = data_g0[line];
			line_data_g1 = data_g1[line];
			line_data_b0 = data_b0[line];
			line_data_b1 = data_b1[line];
		}else{
			line_data_r0 = line_data_r0<<1;
			line_data_r1 = line_data_r1<<1;
			line_data_g0 = line_data_g0<<1;
			line_data_g1 = line_data_g1<<1;
			line_data_b0 = line_data_b0<<1;
			line_data_b1 = line_data_b1<<1;
		}
		//データ統合
		rgb_data = 0;
		rgb_data |= (line_data_r0 &0x80000000) >>25;
		rgb_data |= (line_data_r1 &0x80000000) >>24;
		rgb_data |= (line_data_g0 &0x80000000) >>23;
		rgb_data |= (line_data_g1 &0x80000000) >>22;
		rgb_data |= (line_data_b0 &0x80000000) >>21;
		rgb_data |= (line_data_b1 &0x80000000) >>20;
		GPIOA->ODR &= ~RGB_MASK | rgb_data ;
		GPIOA->ODR |= RGB_MASK & rgb_data ;
		//ラッチ
		if(fclk_cnt==31){
			GPIOB->ODR |=  STB_HI;      //SLATB =1
			line_bits = ((line & 0xc)<<2) |(line & 0x3);
			GPIOA->ODR &= ~LINE_MASK | line_bits ;
			GPIOA->ODR |= LINE_MASK & line_bits ;
		}
		if(fclk_cnt==0){
			GPIOB->ODR &= ~ STB_HI;     //SLATB =0
		}
		//出力イネーブル
		if(fclk_cnt==oetime){// 明るさ
			GPIOB->ODR |= OE_HI;      //OEB=1
		}
		if(fclk_cnt==0){
			GPIOB->ODR &= ~OE_HI;     //OEB=0
		}
		//クロック出力 Hi
		GPIOB->ODR |= CLK_HI;        //SCK=1
		//クロック出力 LoW
		GPIOB->ODR &= ~CLK_HI;       //SCK=0

	}
    GPIOB->ODR &= ~CLK_HI;       //SCK=0
	if (line>=15){
		line = 0;
	}else{
		line++;
	}
}

void LEDMAT_older(void){
	int fclk_cnt;
	for(fclk_cnt=0;fclk_cnt<64;fclk_cnt++){
	  // クロック立下り時の動作
	  if ((clk==0)){
		// クロックフラグ反転
	    clk=1;
	    // クロック数カウント
        if(clk_count>=31){
    	  clk_count=0;
    	  if (line>=15)
    	    line = 0;
    	  else
    	    line++;
        }
        else
  	      clk_count++;
        //クロック出力 LoW
        GPIOB->ODR &= ~CLK_HI;       //SCK=0
	    //データ取得 データシフト
	    if(clk_count==0){
          line_data_r0 = data_r0[line];
          line_data_r1 = data_r1[line];
          line_data_g0 = data_g0[line];
          line_data_g1 = data_g1[line];
          line_data_b0 = data_b0[line];
          line_data_b1 = data_b1[line];
	    }else{
	      line_data_r0 = line_data_r0<<1;
	      line_data_r1 = line_data_r1<<1;
	      line_data_g0 = line_data_g0<<1;
	      line_data_g1 = line_data_g1<<1;
	      line_data_b0 = line_data_b0<<1;
	      line_data_b1 = line_data_b1<<1;
	    }
	    //データ統合
	    rgb_data = 0;
	    rgb_data |= (line_data_r0 &0x80000000) >>25;
	    rgb_data |= (line_data_r1 &0x80000000) >>24;
	    rgb_data |= (line_data_g0 &0x80000000) >>23;
	    rgb_data |= (line_data_g1 &0x80000000) >>22;
	    rgb_data |= (line_data_b0 &0x80000000) >>21;
	    rgb_data |= (line_data_b1 &0x80000000) >>20;
	    //データ出力
	    GPIOA->ODR &= ~RGB_MASK | rgb_data ;
	    GPIOA->ODR |= RGB_MASK & rgb_data ;
        //ラッチ
	    if(clk_count==31){
	      GPIOB->ODR |=  STB_HI;      //SLATB =1
          line_bits = ((line & 0xc)<<2) |(line & 0x3);
          GPIOA->ODR &= ~LINE_MASK | line_bits ;
          GPIOA->ODR |= LINE_MASK & line_bits ;
	    }
	    if(clk_count==0){
	      GPIOB->ODR &= ~ STB_HI;     //SLATB =0
  	    }
        //出力イネーブル
	    if(clk_count==23){
	      GPIOB->ODR |= OE_HI;      //OEB=1
	    }
	    if(clk_count==0){
	      GPIOB->ODR &= ~OE_HI;     //OEB=0
	    }
	  }
	  // クロック立ち上がり時の動作
	  else{
	    // クロックフラグ反転
		clk=0;
        //クロック出力 Hi
		GPIOB->ODR |= CLK_HI;        //SCK=1
	  }
	}
    GPIOB->ODR &= ~CLK_HI;       //SCK=0
}


int get_piece_data(int i, int line){

	static int piece_data[16][8]={
		{0xff,  //　■■■■■■■■
		 0xe7,  //　■■■　　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xdf,  //　■■　■■■■■
		 0xdf,  //　■■　■■■■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdf,  //　■■　■■■■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdf,  //　■■　■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■A
		 0xdb,  //　■■　■■　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xaf,  //　■　■　■■■■
		 0xaf,  //　■　■　■■■■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xaf,  //　■　■　■■■■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■

		{0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,},//　■■■■■■■■
	};
	if (i < 0){
		return 0xff;
	}else if(i > 15){
		return 0;
	}
	return ~piece_data[i][line]&0xff;
}

int* get_color_data(int i, int j){
	static int rgb_data[16][3] = {
		/**/{1, 0, 0}, {1, 0, 0}, {1, 1, 0}, {1, 1, 0},
		/**/{0, 1, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1},
		/**/{0, 0, 1}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1},
		/**/{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0},
	};
	static int rgb_data2[16][3] = {
		/**/{1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {1, 1, 0},
		/**/{0, 1, 0}, {0, 1, 1}, {0, 1, 1}, {0, 1, 1},
		/**/{0, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1},
		/**/{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {0, 0, 0},
	};
	if(i == 0){
		return rgb_data[j];
	}
	return rgb_data2[j];
}


int get_font_data(int i,int j, int line){

	static int font1_data[4][8]={
		{0xff,  //　■■■■■■■■
		 0xe7,  //　■■■　　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xf7,  //　■■■■　■■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xdf,  //　■■　■■■■■
		 0xdf,  //　■■　■■■■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xff,},//　■■■■■■■■
	};

	static int font2_data[4][8]={
		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdf,  //　■■　■■■■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdf,  //　■■　■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■A
		 0xdb,  //　■■　■■　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xfb,  //　■■■■■　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

	};
	static int font3_data[4][8]={
		{0xff,  //　■■■■■■■■
		 0xc3,  //　■■　　　　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xc3,  //　■■　　　　■■
		 0xfb,  //　■■■■■　■■
		 0xc3,  //　■■　　　　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xdb,  //　■■　■■　■■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xaf,  //　■　■　■■■■
		 0xaf,  //　■　■　■■■■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■
	};

	static int font4_data[4][8]={
		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xad,  //　■　■　■■　■
		 0xad,  //　■　■　■■　■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xff,},//　■■■■■■■■

		{0xff,  //　■■■■■■■■
		 0xa1,  //　■　■　　　　■
		 0xaf,  //　■　■　■■■■
		 0xa1,  //　■　■　　　　■
		 0xbd,  //　■　■■■■　■
		 0xbd,  //　■　■■■■　■
		 0xa1,  //　■　■　　　　■
		 0xff,},//　■■■■■■■■

		{0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,  //　■■■■■■■■
		 0x00,},//　■■■■■■■■
	};

	switch (i) {
	case -1:
		return 0xff;
	case 0:
		return font1_data[j][line];
	case 1:
		return font2_data[j][line];
	case 2:
		return font3_data[j][line];
	case 3:
		return font4_data[j][line];
	default:
		return -1;
	}
}

void cp_data(int layer, int rgb_data[16][3],
		int data_r0[16], int data_r1[16],
		int data_g0[16], int data_g1[16],
		int data_b0[16], int data_b1[16]){
	// ライン
	int l = 0;
	for (int i = 0; i < 32; ++i) {
		int font_data;
		int frame = (i%8==0||i%8==7)?0xff:0x81;
		if(i < 8){
			l=0;
		}else if(i < 16){
			l=1;
		}else if(i < 24){
			l=2;
		}else {
			l=3;
		}
		for (int j = 0; j < 4; ++j) {
			int piece = board[l][j]-1;
			for (int k = 0; k < 3; ++k) {
//				font_data = get_font_data(-1, j, i%8);
//				font_data = get_piece_data(l*4 + j, i%8);
				font_data = get_piece_data(piece, i%8);
				font_data &= 0xff;
//				if((l*4+j) != piece_select) continue;
				if(rgb_data[piece][k])
				{
					if(i<16){
						switch (k) {
						case 0:
							data_r0[i] |= font_data << (24-(8*j));
							break;
						case 1:
							data_g0[i] |= font_data << (24-(8*j));
							break;
						case 2:
							data_b0[i] |= font_data << (24-(8*j));
							break;
						}
						//if(piece!=15){
						//	data_r0[i] |= (~font_data &0xff) << (24-(8*j));
						//	data_g0[i] |= (~font_data &0xff) << (24-(8*j));
						//	data_b0[i] |= (~font_data &0xff) << (24-(8*j));
						//}
						if(piece!=15){
							data_r0[i] |= (frame &0xff) << (24-(8*j));
							data_g0[i] |= (frame &0xff) << (24-(8*j));
							data_b0[i] |= (frame &0xff) << (24-(8*j));
						}
					}else{
						switch (k) {
						case 0:
							data_r1[i-16] |= font_data << (24-(8*j));
							break;
						case 1:
							data_g1[i-16] |= font_data << (24-(8*j));
							break;
						case 2:
							data_b1[i-16] |= font_data << (24-(8*j));
							break;
						}
						//if(piece!=15){
						//	data_r1[i-16] |= (~font_data &0xff) << (24-(8*j));
						//	data_g1[i-16] |= (~font_data &0xff) << (24-(8*j));
						//	data_b1[i-16] |= (~font_data &0xff) << (24-(8*j));
						//}
						if(piece!=15){
							data_r1[i-16] |= (frame &0xff) << (24-(8*j));
							data_g1[i-16] |= (frame &0xff) << (24-(8*j));
							data_b1[i-16] |= (frame &0xff) << (24-(8*j));
						}
					}
				}
			}
		}
	}
}


void cl_disp_data(void){
	for(int i=0;i<16;i++){
		data_r0a[i] = 0x0;
		data_r1a[i] = 0x0;
		data_g0a[i] = 0x0;
		data_g1a[i] = 0x0;
		data_b0a[i] = 0x0;
		data_b1a[i] = 0x0;
		data_r0b[i] = 0x0;
		data_r1b[i] = 0x0;
		data_g0b[i] = 0x0;
		data_g1b[i] = 0x0;
		data_b0b[i] = 0x0;
		data_b1b[i] = 0x0;
	}
}

void cp_disp_data(int disp_data[], int time_data[], int shift_data[]){
	for (int i = 0; i < 16; ++i) {
		int line = 0;
		int l = i % 8;
		if(i < 8){
			int frame = (l==0||l==7) ? 0xffffffff : 0x80000001;
			line = i+8;
			data_r0a[line] = disp_data[i];
			data_r0b[line] = disp_data[i];
			data_g0a[line] = disp_data[i];
			data_g0b[line] = disp_data[i];
			data_b0a[line] = disp_data[i] | frame;
			data_b0b[line] = disp_data[i] | frame;
		}else{
			int frame = (l==7) ? 0xffffffff : 0x80000001;
			line = i-8;
			data_r1a[line] = disp_data[i];
			data_r1b[line] = disp_data[i];
			data_g1a[line] = disp_data[i];
			data_g1b[line] = disp_data[i];
			data_b1a[line] = disp_data[i];
			data_b1b[line] = disp_data[i];
			for (int j = 0; j < 4; ++j) {
				int dots = get_7seg_dot(time_data[j],l);
				data_r1a[line] |= dots << shift_data[j];
				data_r1b[line] |= dots << shift_data[j];
				data_g1a[line] |= dots << shift_data[j];
				data_g1b[line] |= dots << shift_data[j];
				data_b1a[line] |= dots << shift_data[j];
				data_b1b[line] |= dots << shift_data[j];
			}
			data_b1a[line] |= frame;
			data_b1b[line] |= frame;
		}
	}
}

void show_clear_disp(Pazzle_time *pz){

	if (pz->min < 1){
		show_clear_lt1min(pz);
	}else if (pz->min < 10){
		show_clear_lt10min(pz);
	}else{
		show_clear_gt10min(pz);
	}
}

void show_clear_lt1min(Pazzle_time *pz){
	static int disp_data[16] = {
			 0x00000000,
			 0x3A0E6714,
			 0x42109494,
			 0x421EF714,
			 0x42109480,
			 0x3BCE9494,
			 0x00000000,
			 0x00000000,
			 0x1C000000,
			 0x2A000000,
			 0x49000000,
			 0x4D000000,
			 0x41000000,
			 0x22000000,
			 0x1C001000,
			 0x00000000,
	};
	static int shift_data[4] = {19, 14, 7, 2};
	int time_data[4] = {
			pz->sec/10,
			pz->sec%10,
			pz->ms10/10,
			pz->ms10%10,
	};

	cp_disp_data(disp_data, time_data, shift_data);

}

void show_clear_lt10min(Pazzle_time *pz){
	static int disp_data[16] = {
			 0x00000000,
			 0x3A0E6714,
			 0x42109494,
			 0x421EF714,
			 0x42109480,
			 0x3BCE9494,
			 0x00000000,
			 0x00000000,
			 0x1C000000,
			 0x2A020000,
			 0x49000000,
			 0x4D000000,
			 0x41000000,
			 0x22020000,
			 0x1C000020,
			 0x00000000,
	};
	static int shift_data[4] = {19, 12, 7, 1};
	int time_data[4] = {
			pz->min%10,
			pz->sec/10,
			pz->sec%10,
			pz->ms10/10,
	};

	cp_disp_data(disp_data, time_data, shift_data);

}

void show_clear_gt10min(Pazzle_time *pz){
	static int disp_data[16] = {
			 0x00000000,
			 0x3A0E6714,
			 0x42109494,
			 0x421EF714,
			 0x42109480,
			 0x3BCE9494,
			 0x00000000,
			 0x00000000,
			 0x1C000000,
			 0x2A001000,
			 0x49000000,
			 0x4D000000,
			 0x41000000,
			 0x22001000,
			 0x1C000000,
			 0x00000000,
	};
	static int shift_data[4] = {19, 14, 7, 2};
	int time_data[4] = {
			pz->min/10,
			pz->min%10,
			pz->sec/10,
			pz->sec%10,
	};

	cp_disp_data(disp_data, time_data, shift_data);
}

/*
 * get_7seg_dot 関数
 */
int get_7seg_dot (int n, int line)
{
    int dot;
    unsigned char dec = 0x0;
    switch(n){
    case 0x0: dec = 0x3f; break;
    case 0x1: dec = 0x06; break;
    case 0x2: dec = 0x5b; break;
    case 0x3: dec = 0x4f; break;
    case 0x4: dec = 0x66; break;
    case 0x5: dec = 0x6d; break;
    case 0x6: dec = 0x7d; break;
    case 0x7: dec = 0x27; break;
    case 0x8: dec = 0x7f; break;
    case 0x9: dec = 0x6f; break;
    case 0xa: dec = 0x77; break;
    case 0xb: dec = 0x7c; break;
    case 0xc: dec = 0x39; break;
    case 0xd: dec = 0x5e; break;
    case 0xe: dec = 0x79; break;
    case 0xf: dec = 0x71; break;
    default: return 0x0;
    }
    switch(line){
    case 0:
        dot = ((dec >> 0) & 0x1)?(0x6):(0x0);
        break;
    case 1:
    case 2:
        dot = (((dec >> 5) & 0x1)?(0x8):(0x0))
		|(((dec >> 1) & 0x1)?(0x1):(0x0));
        break;
    case 3:
        dot = ((dec >> 6) & 0x1)?(0x6):(0x0);
        break;
    case 4:
    case 5:
        dot = (((dec >> 4) & 0x1)?(0x8):(0x0))
		|(((dec >> 2) & 0x1)?(0x1):(0x0));
        break;
    case 6:
        dot = ((dec >> 3) & 0x1)?(0x6):(0x0);
        break;
    default :
    	dot = 0x0;
    	break;
    }
    return dot;
}

void add_10ms(Pazzle_time *pz){
	if(pz->ms10 == 99){
		pz->ms10 = 0;
		if (pz->sec == 59){
			pz->sec = 0;
			if(pz->min==99){
				pz->ms10 = 9;
				pz->sec = 59;
			}else{
				pz->min++;
			}
		}else{
			pz->sec++;
		}
	}else{
		pz->ms10++;
	}
}


void clr_pzltim(Pazzle_time *pz){
		pz->ms10 = 0;
		pz->sec = 0;
		pz->min = 0;
}



int is_push(SW_IF * sw)
{
	static uint32_t buf, din;
	buf = sw->reg->IDR;
	din = (((buf) >> (sw->port_num)) & 0x01);
	return din;
}

int is_posedge(SW_IF * sw)
{
	sw->swp2 = sw->swp1;
	sw->swp1 = sw->swc;
	sw->swc = is_push(sw);

	if ((sw->swp1 == sw->swc) && (sw->swp2 == sw->swp1)) {
		sw->now = sw->swc;
	}

	if (sw->now != sw->last) {
		sw->up = sw->now & ~ sw->last;
		sw->last = sw->now;
 	}
	return sw->up;
}

void pazzle_move(int btn){
	int is_move = 0;
	POS blank_pos = get_blank_pos();
	POS mov_pos = blank_pos;
	switch (btn) {
	case BTN_ESC:
	{
		switch(game_dat.state){
		case STATE_INIT:
			break;
		case -1:
		case STATE_PAZZLE:
		case STATE_CLEAR:
			state_change(STATE_INIT);
			break;
		}
	}
		break;
	case BTN_LEFT:{
		//is_timcnt = 1;
		is_move = 1;
		mov_pos.col++;
	}
		break;
	case BTN_DOWN:
	{
		//is_timcnt = 0;
		is_move = 1;
		mov_pos.row--;
	}
		break;
	case BTN_UP:
	{
		//if(!is_timcnt){
		//	clr_pzltim(game_dat.pzt);
		//}
		is_move = 1;
		mov_pos.row++;
	}
		break;
	case BTN_RIGHT:
	{
		is_move = 1;
		mov_pos.col--;
	}
		break;
	default:
		break;
	}
	if(is_move){
		if (game_dat.state == STATE_INIT)
			state_change(STATE_PAZZLE);
		if (game_dat.state == STATE_PAZZLE){
			move_blank(mov_pos);
			if(is_game_clear()){
				state_change(STATE_CLEAR);
			}
		}
	}
}

POS get_blank_pos()
{
    POS blank_pos = { -1, -1 };

    for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if(board[i][j] == BLANK_NUMBER){
				blank_pos.row = i;
				blank_pos.col = j;
			}
		}
	}

    return blank_pos;
}

void move_blank(POS mov_pos)
{
    if (mov_pos.col < 0
		|| mov_pos.col >= BOARD_SIZE
		|| mov_pos.row < 0
		|| mov_pos.row >= BOARD_SIZE)
    {
        return;
    }

    POS blank_pos = get_blank_pos();

    // swapping
    int tempBlank = board[blank_pos.row][blank_pos.col];
    board[blank_pos.row][blank_pos.col] = board[mov_pos.row][mov_pos.col];
    board[mov_pos.row][mov_pos.col] = tempBlank;
}

void state_change(int new_state){
	game_dat.flg |= FLG(CLS);
	if (game_dat.state == new_state){
		return;
	}
	switch(new_state){
	case STATE_INIT:
		clr_pzltim(game_dat.pzt);
		board_init();
		board_shuffle(1000, 2);
		break;
	case STATE_PAZZLE:
		is_timcnt = 1;
		break;
	case STATE_CLEAR:
		is_timcnt = 0;
		break;
	}
	game_dat.state = new_state;
}


void board_init(void){
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			board[i][j] = i * BOARD_SIZE + j + 1;
		}
	}
}

void board_shuffle(int times, int interval){
    for (int i = 0; i < times; ++i) {
		POS blank_pos = get_blank_pos();
		POS mov_pos = blank_pos;
		switch (rand()%4) {
		case 0:{
			mov_pos.col++;
		}
			break;
		case 1:
		{
			mov_pos.row--;
		}
			break;
		case 2:
		{
			mov_pos.row++;
		}
			break;
		case 3:
		{
			mov_pos.col--;
		}
			break;
		default:
			break;
		}
		move_blank(mov_pos);
		cl_disp_data();
		cp_data(0, rgb_data1,
				data_r0a, data_r1a,
				data_g0a, data_g1a,
				data_b0a, data_b1a);
		cp_data(1, rgb_data2,
				data_r0b, data_r1b,
				data_g0b, data_g1b,
				data_b0b, data_b1b);
		waite_func(interval);
	}
}

int is_game_clear(void){
	int flg_game_clear = 1;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			if(board[i][j] != i * BOARD_SIZE + j + 1){
				flg_game_clear = 0;
			}
		}
	}
	return flg_game_clear;
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
