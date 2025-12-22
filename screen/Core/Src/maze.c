#include "gpio.h"
#include "main.h"
#include "lcd_v4.h"
#include "time.h"
#include <math.h>
#include <stdbool.h>
#include "touch.h"
#include "led.h"
#include "sys.h"
#include "delay.h"
//#include "usart.h"
#include "key.h"
#include "lcd.h"
//#include "usmart.h"
#include "touch.h"
#include "24cxx.h"
#include "24l01.h" //通信驱动 基于spi进行通信
#include "ui.h"
#include "maze.h"
//#include "font.h"



extern UART_HandleTypeDef huart1;
struct Point{
	u16 x;
	u16 y;
}points[255];

#define MAX_CMD_LEN 20

char tx_buffer[MAX_CMD_LEN];


u16 x;
u16 y;
u8 pulse = 0;
u16 count = 0;

void rtp(int mode)
{
	u8 i=0;

	points[0].x = 0;
	points[0].y = lcddev.height;
	while(1)
	{
		if(false){
			mode0:mode = 0;
			lcd_init();
			lcd_show_string(30, 30, 200, 24, 24, "Little car", RED);
			lcd_show_string(30, 70, 200, 24, 24, "Route planning", RED);
			lcd_show_string(30, 110, 200, 24, 24, "Following", RED);
			lcd_show_string(30, 150, 200, 24, 24, "SD card storing", RED);
			lcd_show_string(30, 270, 200, 24, 24, "Cannot connect :`(", RED);
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			HAL_Delay(200);//闪烁用以调试
			//				rtp(0);
		}
		if(false){
			mode1:mode = 1;
			lcd_init();
			//HAL_UART_Transmit(&huart1, (uint8_t *)"route", 5, 0xffff);
			lcd_show_string(30, 30, 200, 24, 24, "Route planning", RED);
			lcd_show_string(30, 70, 200, 24, 24, "BACK", RED);
		}
		if(false){
			mode2:mode = 2;
			lcd_init();
			lcd_show_string(30, 30, 200, 24, 24, "Following", RED);
			lcd_show_string(30, 70, 200, 24, 24, "BACK", RED);
		}
		if(false){
			mode3:mode = 3;
			lcd_init();
			lcd_show_string(30, 30, 200, 24, 24, "SD Card storing", RED);
			lcd_show_string(30, 70, 200, 24, 24, "BACK", RED);
		}
		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{
				if(mode == 0){//在菜单界面
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<336&&tp_dev.y[0]<110){
						lcd_show_string(30, 70, 200, 24, 24, "ROUTE PLANNING", RED);
						goto mode1;
					}
					else if(tp_dev.x[0]>30&&tp_dev.y[0]>110&&tp_dev.x[0]<216&&tp_dev.y[0]<150){
						lcd_show_string(30, 110, 200, 24, 24, "FOLLOWING", RED);
						goto mode2;
					}
					else if(tp_dev.x[0]>30&&tp_dev.y[0]>150&&tp_dev.x[0]<360&&tp_dev.y[0]<190){
						lcd_show_string(30, 150, 200, 24, 24, "SD CARD STORING", RED);
						goto mode3;
					}
				}
				if(mode == 1){
					u16 order = 1;
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<336&&tp_dev.y[0]<110){
				        // 发送结束信号
				        bt_send_end();

				        // 可选：发送提示信息
				        lcd_show_string(30,150,200,16,16,"WOW！", BLUE);
				        HAL_Delay(1000);

				        count = 0;
				        points[0].x = 0;
				        points[0].y = lcddev.height;
				        goto mode0;
					}
					else if(tp_dev.x[0]>30&&tp_dev.y[0]>110){
				        // 如果是第一个点，发送开始信号
				        if(count == 0) {
				            bt_send_start();
				            HAL_Delay(10);  // 等待接收端准备
				        }

				        // 绘制点
				        uint16_t POINT_COLOR;
				        TP_Draw_Big_Point(tp_dev.x[0], tp_dev.y[0], RED);

				        pulse += 1;
				        x = tp_dev.x[0];
				        y = tp_dev.y[0];

				        if(pulse == 5) {  // 每5个点采样一次
				            u16 temp = POINT_COLOR;
				            POINT_COLOR = BLUE;
				            LCD_DrawLine(points[count].x, points[count].y, x, y);

				            // 发送坐标点
				            bt_send_point(x, y);

				            count += 1;
				            points[count].x = x;
				            points[count].y = y;
				            POINT_COLOR = temp;
				            pulse = 0;

				            // 可选：显示发送的坐标
				            char info[30];
				            snprintf(info, 30, "send: %d,%d", x, y);
				            lcd_show_string(30, 130, 200, 16, 16, info, BLUE);
				        }
					}
				}
				if(mode == 2){
					u16 pre_x = 0;
					u16 pre_y = 300;
					u16 x = 30;//bluetooth
					u16 y = 50;
					static int pulse = 0;
					TP_Draw_Big_Point(x,y,BLUE);
					pulse++;
					uint16_t POINT_COLOR;
					if(pulse == 5){
						//lcd_draw_bline(pre_x, pre_y, x, y, 3, RED);
						u16 temp = POINT_COLOR;
						POINT_COLOR=BLUE;
						LCD_DrawLine(pre_x, pre_y, x, y);
						POINT_COLOR = temp;
						pre_x = x;
						pre_y = y;
						pulse = 0;
					}
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<336&&tp_dev.y[0]<110){

						goto mode0;
						//HAL_UART_Transmit(&huart1, (uint8_t *)"stop", 5, 0xffff);
					}
				}
				if(mode == 3){
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<336&&tp_dev.y[0]<110){
						goto mode0;
					}
				}
			}
		}
		i++;
		if(i%20==0)LED0=!LED0;
	}
}
// 发送字符串（自动添加换行）
void bt_send_string(const char* str) {
    char buffer[MAX_CMD_LEN];
    snprintf(buffer, MAX_CMD_LEN, "%s\n", str);
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
    HAL_Delay(5);  // 小延迟确保数据发送完成
}

// 发送坐标点
void bt_send_point(uint16_t x, uint16_t y) {
    snprintf(tx_buffer, MAX_CMD_LEN, "P:%d,%d", x, y);
    bt_send_string(tx_buffer);
}

// 发送开始信号
void bt_send_start() {
    bt_send_string("START");
}

// 发送结束信号
void bt_send_end(void) {
    bt_send_string("END");
}
