/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "control.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
#define RX_BUFFER_SIZE 50

// 接收相关变量
char rx_buffer[RX_BUFFER_SIZE];  // 接收缓冲区
uint8_t rx_index = 0;            // 缓冲区索引
volatile uint8_t rx_ready = 0;   // 数据就绪标志

// 路径规划相关变量


Point path_points[100];           // 存储接收到的路径点
uint8_t point_count = 0;          // 接收到的点数
uint8_t path_received = 0;        // 路径接收完成标志
uint8_t receiving_path = 0;       // 正在接收路径标志

// 接收单个字符的缓冲区
uint8_t rx_char;

// UART初始化（在主函数初始化中调用）
void uart_init(void) {
	uint8_t tx_char = 'w';
	HAL_UART_Transmit(&huart1, &tx_char, 1, HAL_MAX_DELAY);
    HAL_UART_Receive_IT(&huart1, &rx_char, 1);  // 启动接收中断
    forward_with_length();
    HAL_Delay(500);
}

// 接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart->Instance == USART1) {
        // 如果收到换行符，表示一条命令结束
        if(rx_char == '\n' || rx_char == '\r') {
            if(rx_index > 0) {
                rx_buffer[rx_index] = '\0';  // 添加字符串结束符
                rx_ready = 1;                // 设置数据就绪标志
            }
            rx_index = 0;  // 重置索引
        }
        // 如果是普通字符，存入缓冲区
        else if(rx_index < RX_BUFFER_SIZE - 1) {
            rx_buffer[rx_index++] = rx_char;
        }
        // 缓冲区溢出，重置
        else {
            rx_index = 0;
        }

        // 重新启动接收
        HAL_UART_Receive_IT(&huart1, &rx_char, 1);
    }
}

// 解析接收到的命令
void parse_command(const char* cmd) {
    // 调试输出（可选）

    // 检查命令类型
    if(strncmp(cmd, "START", 5) == 0) {
        // 开始接收新路径
        point_count = 0;
        receiving_path = 1;
        path_received = 0;
    }
    else if(strncmp(cmd, "END", 3) == 0) {
        // 路径接收完成
        receiving_path = 0;
        path_received = 1;
        printf("路径接收完成，共 %d 个点\n", point_count);
    }
    else if(strncmp(cmd, "P:", 2) == 0 && receiving_path) {
        // 解析坐标点
        uint16_t x, y;
        if(sscanf(cmd, "P:%hu,%hu", &x, &y) == 2) {
            if(point_count < 100) {
                path_points[point_count].x = x;
                path_points[point_count].y = y;
                point_count++;

                // 调试输出
                printf("收到点 %d: (%d, %d)\n", point_count, x, y);
            }
        }
    }
}
int detect_keys(){
	if(HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_RESET &&
			HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_SET &&
			HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_RESET)
		return 1;
	else if(HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_SET &&
			HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_RESET &&
			HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_RESET)
		return 2;
	else if(HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin) == GPIO_PIN_SET &&
			HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin) == GPIO_PIN_SET &&
			HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin) == GPIO_PIN_SET)
		return 3;
	else
		return 0;
}


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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	turn_in_place(1);
	HAL_Delay(500);
	uart_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		int key = detect_keys();
		/*if(rxBuffer[1] == 'r'){
			key = 1;
		}*/
		if (key == 1) {
			mode1_loop(); // 路径规划模式
		}
		else if (key == 2) {
			mode2_loop(); // 红外跟踪模式
		}
		else {
			stop_motors();
		}
//		HAL_UART_Transmit(&huart1, (uint8_t *)"01234", 5, 0xffff);
//		HAL_Delay(1000);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
