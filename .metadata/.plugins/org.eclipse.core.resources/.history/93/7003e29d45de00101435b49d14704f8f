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
volatile uint32_t game_start_tick = 0;  // 游戏开始时的tick值
void start_game_timer(void)
{
	game_start_tick = HAL_GetTick();
}

uint32_t get_game_time_ms(void)
{
	return HAL_GetTick() - game_start_tick;
}
uint32_t seconds;
void display_game_time(uint16_t x, uint16_t y)
{
	uint32_t ms = get_game_time_ms();
	seconds = ms / 1000;

	char time_str[8];
	sprintf(time_str, "%03ds", seconds);  // %03d表示至少3位，不足用0填充
	lcd_show_string(x, y, 200, 16, 16, time_str, RED);
}

void drawplayer(){ // 绘制玩家
	lcd_fill(px-9, py-9, px+9, py+9, BLUE);

}

void drawtrail(int direction){
	lcd_fill(px-9, py-9, px+10, py+9, CYAN);
	switch(direction){
	case 1:
		px += 0;
		py -= 22;
		piy--;
		break;
	case 2:
		px += 0;
		py += 22;
		piy++;
		break;
	case 3:
		px -= 22;
		py += 0;
		pix--;
		break;
	case 4:
		px += 22;
		py += 0;
		pix++;
		break;
	}
	drawplayer();
}

void drawmaze(){ //绘制整个迷宫
	HAL_Init();

}

void play(){ // 开始游戏
	start_playing = true;
	menu = false;
}

void move(int direction){ // 玩家移动
	switch(direction){
	case 1:
		//上
		break;
	case 2:
		//下
		break;
	case 3:
		//左
		break;
	case 4:
		//右
		break;
	default:break;
	}
}

void rtp(int mode)
{
	u8 key;
	u8 i=0;
	while(1)
	{

		key=KEY_Scan(0);
		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(mode == 0){//在菜单界面
				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
				{
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<336&&tp_dev.y[0]<110){
						//						drawmaze();
						lcd_show_string(30, 70, 200, 24, 24, "ROUTE PLANNING", RED);
					}
					else if(tp_dev.x[0]>30&&tp_dev.y[0]>110&&tp_dev.x[0]<216&&tp_dev.y[0]<150){
						//						drawmaze();
						lcd_show_string(30, 110, 200, 24, 24, "FOLLOWING", RED);
					}
					else if(tp_dev.x[0]>30&&tp_dev.y[0]>150&&tp_dev.x[0]<360&&tp_dev.y[0]<190){
						//						drawmaze();
						lcd_show_string(30, 150, 200, 24, 24, "SD CARD STORING", RED);
					}
				}
			}
			if(mode == 1){
				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height && start_playing) // 在屏幕范围内
				{
					if(tp_dev.x[0]>px-23 && tp_dev.x[0]<px+23 && tp_dev.y[0]>py-67 && tp_dev.y[0]<py-14){ // 往上
						move(1);
					}
					else if(tp_dev.x[0]>px-23 && tp_dev.x[0]<px+23 && tp_dev.y[0]>py+14 && tp_dev.y[0]<py+67){ // 往下
						move(2);
					}
					else if(tp_dev.x[0]>px-67 && tp_dev.x[0]<px-14 && tp_dev.y[0]>py-23 && tp_dev.y[0]<py+23){ // 往左
						move(3);
					}
					else if(tp_dev.x[0]>px+14 && tp_dev.x[0]<px+67 && tp_dev.y[0]>py-23 && tp_dev.y[0]<py+23){ // 往右
						move(4);
					}
					else if(tp_dev.x[0]>50 && tp_dev.y[0]>270 && tp_dev.y[0]<300 && tp_dev.x[0]<170){
						//						goto reset___;
					}
				}
				else{ // 在菜单界面
					if(tp_dev.x[0]>70 && tp_dev.y[0]>30 && tp_dev.y[0]<54){ // 往上
						HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
					}
				}
			}
			if(mode == 2){
				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height && start_playing) // 在屏幕范围内
				{
					if(tp_dev.x[0]>30 && tp_dev.x[0]<198 && tp_dev.y[0]>230 && tp_dev.y[0]<260){ // 往右
						//						reset_ = true;
						break;
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

