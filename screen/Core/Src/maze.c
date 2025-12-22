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
//#include "font.h"

extern UART_HandleTypeDef huart1;
struct Point{
	u16 x;
	u16 y;
}points[255];

u16 x;
u16 y;
u8 pulse = 0;
u16 count = 0;

void rtp(int mode)
{
	u8 key;
	u8 i=0;
	while(1)
	{
		if(false){
			mode0:mode = 0;
			lcd_init();
		}
		if(false){
			mode1:mode = 1;
			lcd_init();
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
		key=KEY_Scan(0);
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
					points[0].x = 0;
					points[0].y = 0;
					u16 px = 0;
					u16 py = lcddev.height;
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<336&&tp_dev.y[0]<110){
						goto mode0;
					}
					if(tp_dev.x[0]>30&&tp_dev.y[0]>110){
						uint8_t message[100];
						uint16_t POINT_COLOR;
						TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);//画图
						pulse += 1;
						x = tp_dev.x[0];
						y = tp_dev.y[0];
						if(pulse == 5 /*sampling_para*/){
							POINT_COLOR=BLUE;
							u16 temp = POINT_COLOR;
							LCD_DrawLine(px, py, x, y);
							count += 1;
							points[count].x = x;
							points[count].y = y;
							px = x;
							py = y;
							POINT_COLOR = temp;
							pulse = 0;
						}
						sprintf(message, "%d,%d", x, y);
						HAL_UART_Transmit(&huart1, message, 5, 0xffff);
					}
				}
				if(mode == 2){
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<336&&tp_dev.y[0]<110){
						goto mode0;
						HAL_UART_Transmit(&huart1, (uint8_t *)"stop", 5, 0xffff);
					}
				}
				if(mode == 3){
					u32 sd_size;
					u8 t=0;
					sd_size=SD_GetSectorCount();//得到扇区数
					while(1)
					{
						key=KEY_Scan(0);
						if(key==KEY0_PRES){
							SD_Read_Sectorx(0);//KEY0按,读取SD卡扇区0的内容
							HAL_Delay(2000);
							break;
						}
						t++;
						delay_ms(10);
						if(t==20)
						{
							LED0=!LED0;
							t=0;
						}
					}
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


