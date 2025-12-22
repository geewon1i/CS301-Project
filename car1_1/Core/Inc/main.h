/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define RX_BUFFER_SIZE 50

// 接收相关变量
extern char rx_buffer[RX_BUFFER_SIZE];  // 接收缓冲区
extern uint8_t rx_index;            // 缓冲区索引
extern volatile uint8_t rx_ready;   // 数据就绪标志

// 路径规划相关变量
typedef struct {
    uint16_t x;
    uint16_t y;
} Point;

extern Point path_points[100];           // 存储接收到的路径点
extern uint8_t point_count;          // 接收到的点数
extern uint8_t path_received;        // 路径接收完成标志
extern uint8_t receiving_path;       // 正在接收路径标志
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void parse_command(const char* cmd);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define EN2_L_Pin GPIO_PIN_2
#define EN2_L_GPIO_Port GPIOC
#define K3_Pin GPIO_PIN_0
#define K3_GPIO_Port GPIOA
#define K1_Pin GPIO_PIN_4
#define K1_GPIO_Port GPIOC
#define K2_Pin GPIO_PIN_5
#define K2_GPIO_Port GPIOC
#define L1_Pin GPIO_PIN_0
#define L1_GPIO_Port GPIOB
#define IN3_L_Pin GPIO_PIN_12
#define IN3_L_GPIO_Port GPIOB
#define IN1_L_Pin GPIO_PIN_13
#define IN1_L_GPIO_Port GPIOB
#define IN2_R_Pin GPIO_PIN_14
#define IN2_R_GPIO_Port GPIOB
#define IN4_R_Pin GPIO_PIN_15
#define IN4_R_GPIO_Port GPIOB
#define EN2_R_Pin GPIO_PIN_7
#define EN2_R_GPIO_Port GPIOC
#define EN2_LC9_Pin GPIO_PIN_9
#define EN2_LC9_GPIO_Port GPIOC
#define EN1_L_Pin GPIO_PIN_10
#define EN1_L_GPIO_Port GPIOC
#define L2_Pin GPIO_PIN_2
#define L2_GPIO_Port GPIOD
#define IN4_L_Pin GPIO_PIN_6
#define IN4_L_GPIO_Port GPIOB
#define IN2_L_Pin GPIO_PIN_7
#define IN2_L_GPIO_Port GPIOB
#define IN1_R_Pin GPIO_PIN_8
#define IN1_R_GPIO_Port GPIOB
#define IN3_R_Pin GPIO_PIN_9
#define IN3_R_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
