/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pazzle15.h"
#include "mid_pazzle15.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef uint32_t * uintp;

typedef GPIO_TypeDef * GPIOx;


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

typedef struct TAG_LP_IF{
	GPIOx reg;
	int32_t port_num;
	int32_t is_long_push;
	int32_t lp_start;
	int32_t en_cnt;
} LP_IF;

// ゲーム管理データ
typedef struct TAG_GAME_DATA{
	int flg;
	int state;
	int posedge_buttons;
	Pazzle_time * pzt;
	int move_cnt;
} GAME_DATA;


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
	// 何かのスイッチ長押し
	SW_LP,
	// USBになんか来た
	RECV_USART,
};

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

#define NUM_SWIF 7
#define NUM_LPIF 7

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define FLG(flg_val) (((uint32_t)0x00000001)<<(flg_val))
#define HAS_FLG(flg_dat, flg_val) flg_dat & FLG(flg_val)
#define CLR_FLG(flg_dat, flg_val) flg_dat &= ~FLG(flg_val)
#define SET_FLG(flg_dat, flg_val) flg_dat |= FLG(flg_val)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */


void  init_system(void);
void waite_func(int);


// LED Matrix

void LEDMATv3(int, int, int, int, int, int, int, int);

void cp_data(int, int[16][3], int[16], int[16], int[16], int[16], int[16], int[16]);

void cl_disp_data(void);

////// Show ClearTime

void show_clear_disp(Pazzle_time *);
void show_clear_lt1min(Pazzle_time *);
void show_clear_lt10min(Pazzle_time *);
void show_clear_gt10min(Pazzle_time *);
int get_7seg_dot (int, int);

int pzt_toint(Pazzle_time * pzt);
void add_10ms(Pazzle_time *);
void clr_pzltim(Pazzle_time *);

void usart2_send_time(void);
void usart2_tx(char* out_dtr);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LEDMAT_A0_PIN0_Pin GPIO_PIN_0
#define LEDMAT_A0_PIN0_GPIO_Port GPIOA
#define LEDMAT_A1_PIN1_Pin GPIO_PIN_1
#define LEDMAT_A1_PIN1_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LEDMAT_A2_PIN2_Pin GPIO_PIN_4
#define LEDMAT_A2_PIN2_GPIO_Port GPIOA
#define LEDMAT_D13_PIN3_Pin GPIO_PIN_5
#define LEDMAT_D13_PIN3_GPIO_Port GPIOA
#define LEDMAT_D12_R0_Pin GPIO_PIN_6
#define LEDMAT_D12_R0_GPIO_Port GPIOA
#define LEDMAT_D11_R1_Pin GPIO_PIN_7
#define LEDMAT_D11_R1_GPIO_Port GPIOA
#define LEDMAT_D7_G0_Pin GPIO_PIN_8
#define LEDMAT_D7_G0_GPIO_Port GPIOA
#define LEDMAT_D8_G1_Pin GPIO_PIN_9
#define LEDMAT_D8_G1_GPIO_Port GPIOA
#define LEDMAT_D2_B0_Pin GPIO_PIN_10
#define LEDMAT_D2_B0_GPIO_Port GPIOA
#define LEDMAT_PA11_B1_Pin GPIO_PIN_11
#define LEDMAT_PA11_B1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LEDMAT_D4_CLK_Pin GPIO_PIN_5
#define LEDMAT_D4_CLK_GPIO_Port GPIOB
#define LEDMAT_PB6_STB_Pin GPIO_PIN_6
#define LEDMAT_PB6_STB_GPIO_Port GPIOB
#define LEDMAT_PB7_OE_Pin GPIO_PIN_7
#define LEDMAT_PB7_OE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
