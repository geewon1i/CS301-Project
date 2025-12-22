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

extern UART_HandleTypeDef huart1;

bool menu = true;
bool start_playing;
int maze[10][10] = // 迷宫地图，数字参考mazereference.png
{
		{	15,	11,	11,	3,	11,	3,	11,	11,	7,	12,	},
		{	6,	11,	7,	14,	6,	8,	6,	7,	9,	4,	},
		{	9,	7,	9,	11,	8,	6,	4,	14,	6,	8,	},
		{	6,	8,	6,	11,	11,	8,	10,	6,	8,	12,	},
		{	8,	6,	5,	11,	11,	7,	14,	10,	15,	1,	},
		{	12,	10,	6,	3,	13,	9,	11,	8,	6,	4,	},
		{	2,	8,	10,	9,	3,	11,	13,	6,	4,	10,	},
		{	10,	15,	5,	13,	9,	11,	3,	8,	10,	14,	},
		{	9,	11,	11,	7,	6,	7,	9,	13,	9,	7,	},
		{	15,	11,	11,	5,	8,	9,	11,	11,	11,	8	}
};
int timer = 0;
int px=22, py=142, pix, piy;
bool gameover = false;

void rtp(int mode)
{
	u8 key;
	u8 i=0;
	while(1)
	{
		if(false)
			mode0:mode = 0;
		if(false)
			mode1:mode = 1;
		if(false)
			mode2:mode = 2;
		if(false)
			mode3:mode = 3;
		key=KEY_Scan(0);
		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(mode == 0){//在菜单界面
				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
				{
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
			}
		}
		if(key==KEY0_PRES)
		{
			LCD_Clear(WHITE);	//清屏
			TP_Adjust();  		//屏幕校准
			TP_Save_Adjdata();
		}
		if(key==WKUP_PRES)
		{
			HAL_UART_Transmit(&huart1, "01234", 5, 0xffff);
		}
		i++;
		if(i%20==0)LED0=!LED0;
	}
}

