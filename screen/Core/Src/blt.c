/*
 * blt.c
 *
 *  Created on: Dec 23, 2025
 *      Author: iris1
 */
#include "blt.h"
#include "maze.h"
#include "lcd.h"
#include "touch.h"
#define RX_BUFFER_SIZE 64
#define CMD_START 0x53
#define CMD_END 0x45
#define CMD_MOVE_TO 0x01
#define	CMD_DRAW	0x02

uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_data;
uint8_t rx_index = 0;
uint8_t command_received = 0;

uint16_t pre_x = 0;
uint16_t pre_y = 320;
uint8_t p;
extern int move_time;
extern int turn_time;
extern uint8_t mode;
extern uint16_t  POINT_COLOR;

float current_angle=90.0;

/* 命令结构体 */

Bluetooth_Command current_cmd;
extern UART_HandleTypeDef huart1;


void Send_Move_Command(uint16_t x, uint16_t y)
{
    uint8_t tx_buffer[7];

    // 构建简单命令帧
    tx_buffer[0] = CMD_START;     // 开始标志
    tx_buffer[1] = 0x01;   // 命令类型
    tx_buffer[3] = x & 0xFF;      // X坐标低字节
    tx_buffer[2] = (x >> 8) & 0xFF; // X坐标高字节
    tx_buffer[5] = y & 0xFF;      // Y坐标低字节
    tx_buffer[4] = (y >> 8) & 0xFF; // Y坐标高字节
    tx_buffer[6] = CMD_END;       // 结束标志

    // 发送命令
    //HAL_UART_Transmit(&huart1, tx_buffer, 7, 10);
    HAL_UART_Transmit(&huart1, tx_buffer, 2, 50);
    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, tx_buffer+2, 2, 50);
    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, tx_buffer+4, 2, 50);
    HAL_Delay(100);
    HAL_UART_Transmit(&huart1, tx_buffer+6, 1, 50);
    HAL_Delay(100);
}

/* 主开发板也可以使用中断接收回传信息 */
void Master_UART_Init(void)
{
    // 如果需要接收小车的回传信息，也启用中断接收
    HAL_UART_Receive_IT(&huart1, &rx_data, 1);
}
void Process_Response(uint8_t data)
{
	rx_buffer[rx_index++] = data;
	char info[30];
	/*snprintf(info, 30, "Receiving: %u", data);
	lcd_show_string(30, 130+20*rx_index, 200, 16, 16, info, BLUE);*/
	/*snprintf(info, 30, "Here is: %u", rx_buffer[rx_index-1]);
	lcd_show_string(30, 100, 200 , 16, 16, info, BLUE);*/
	//HAL_UART_Transmit(&huart1,(uint8_t *)rx_buffer,7,0xff);
    if (rx_index >6 && rx_buffer[0] == CMD_START && rx_buffer[6] == CMD_END)
    {
        current_cmd.start_flag = rx_buffer[0];
        current_cmd.command = rx_buffer[1];//0x02

        // 提取坐标
        current_cmd.x = (rx_buffer[2] << 8) | rx_buffer[3];
        current_cmd.y = (rx_buffer[4] << 8) | rx_buffer[5];

        if(mode == 2){
        	uint16_t x = current_cmd.x;
        	uint16_t y = current_cmd.y;
			current_cmd.end_flag = 0;
			TP_Draw_Big_Point(x,y,BLUE);
			p++;
			LCD_ShowString(30, 200, 200, 16, 16, rx_buffer);
			snprintf(info, 30, "Get: %u,%u", x,y);
			lcd_show_string(30, 100, 200, 16, 16, info, BLUE);
			//Send_Move_Command(x,y);
			if(p == 2){
				uint16_t temp = POINT_COLOR;
				POINT_COLOR=BLUE;
				LCD_DrawLine(pre_x, pre_y, x, y);
				POINT_COLOR = temp;
				pre_x = x;
				pre_y = y;
				p = 0;
			}
        }else{
        	pre_x = 0;
			pre_y = 320;
			p = 0;
		}

        current_cmd.end_flag = rx_buffer[rx_index-1];
        rx_index = 0;

    }
    if(rx_index > 6){
    	rx_index = 0;
    }

}
