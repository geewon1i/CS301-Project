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
#include "image.h"

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

bool judge(int direction){
	int type = maze[piy][pix];
	//	char pos[100];
	//	sprintf(pos, "%d", type);
	//	lcd_show_string(30, 210, 200, 24, 16, pos, RED);
	switch(type){
	case 1:
		return true;
	case 2:
		if(direction != 3)
			return true;
		else
			return false;
	case 3:
		if(direction != 1)
			return true;
		else
			return false;
	case 4:
		if(direction != 4)
			return true;
		else
			return false;
	case 5:
		if(direction != 2)
			return true;
		else
			return false;
	case 6:
		if(direction == 2 || direction == 4)
			return true;
		else
			return false;
	case 7:
		if(direction == 2 || direction == 3)
			return true;
		else
			return false;
	case 8:
		if(direction == 1 || direction == 3)
			return true;
		else
			return false;
	case 9:
		if(direction == 1 || direction == 4)
			return true;
		else
			return false;
	case 10:
		if(direction == 1 || direction == 2)
			return true;
		else
			return false;
	case 11:
		if(direction == 3 || direction == 4)
			return true;
		else
			return false;
	case 12:
		if(direction == 2)
			return true;
		else
			return false;
	case 13:
		if(direction == 3)
			return true;
		else
			return false;
	case 14:
		if(direction == 1)
			return true;
		else
			return false;
	case 15:
		if(direction == 4)
			return true;
		else
			return false;
	case 16:
		return false;
	}
	return false;
}

void drawtile(int id, int x, int y){ //绘制未被玩家探索的格子，格子长度
	lcd_fill(x+2, y+2, x+22, y+22, WHITE);
	switch(id){
	case 1:
		break;
	case 2:
		lcd_fill(x, y, x+2, y+24, BLACK);
		break;
	case 3:
		lcd_fill(x, y, x+24, y+2, BLACK);
		break;
	case 4:
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		break;
	case 5:
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		break;
	case 6:
		lcd_fill(x, y, x+2, y+24, BLACK);
		lcd_fill(x, y, x+24, y+2,  BLACK);
		break;
	case 7:
		lcd_fill(x, y, x+24, y+2,  BLACK);
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		break;
	case 8:
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		break;
	case 9:
		lcd_fill(x, y, x+2, y+24, BLACK);
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		break;
	case 10:
		lcd_fill(x, y, x+2, y+24, BLACK);
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		break;
	case 11:
		lcd_fill(x, y, x+24, y+2, BLACK);
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		break;
	case 12:
		lcd_fill(x, y, x+2, y+24, BLACK);
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		lcd_fill(x, y, x+24, y+2, BLACK);
		break;
	case 13:
		lcd_fill(x, y, x+24, y+2, BLACK);
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		break;
	case 14:
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		lcd_fill(x, y, x+2, y+24, BLACK);
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		break;
	case 15:
		lcd_fill(x, y, x+24, y+2, BLACK);
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		lcd_fill(x, y, x+2, y+24, BLACK);
		break;
	case 16:
		lcd_fill(x, y, x+24, y+2, BLACK);
		lcd_fill(x, y+22, x+24, y+24, BLACK);
		lcd_fill(x, y, x+2, y+24, BLACK);
		lcd_fill(x+22, y, x+24, y+24, BLACK);
		break;
	}
}

void drawmaze(){ //绘制整个迷宫
	HAL_Init();

	/* USER CODE BEGIN Init */
	Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函�?
	//	uart_init(115200);					//初始化串�?
	//	usmart_dev.init(84); 		  	  	//初始化USMART
	LED_Init();							//初始化LED
	KEY_Init();							//初始化按�?
	LCD_Init();							//初始化LCD
	tp_dev.init();				   		//触摸屏初始化

	delay_ms(100);
	lcd_fill(0, 0, 255, 300, WHITE);
	int x, y;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			x = 22 * i + 42;
			y = 22 * j + 10;
			drawtile(maze[i][j], y, x);
		}
	}
	px = 22;
	py = 142;
	pix = 0;
	piy = 4;
	drawplayer();
	lcd_show_string(50, 270, 200, 24, 24, "BACK", RED);
	play();
}

void play(){ // 开始游戏
	start_playing = true;
	menu = false;
}

void move(int direction){ // 玩家移动
	if(judge(direction) == false){
		return;
	}
	drawtrail(direction);
}

void rtp(int mode)
{
	u8 key;
	u8 i=0;
	while(1)
	{
		if(pix == 9 && piy == 4){
			gameover = true;
			pix = 0;
			piy = 0;
			break;
		}
		if(menu == false && mode == 0){
			break;
		}
		key=KEY_Scan(0);
		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
			if(mode == 0){
				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
				{
					if(tp_dev.x[0]>30&&tp_dev.y[0]>70&&tp_dev.x[0]<270&&tp_dev.y[0]<100){
						drawmaze();
						start_game_timer();
					}
//					else if(tp_dev.x[0]>30&&tp_dev.y[0]>110&&tp_dev.x[0]<270&&tp_dev.y[0]<140){
////						display_ranking();
//						while(!(tp_dev.sta&TP_PRES_DOWN));
//					}
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
						goto reset___;
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
						reset_ = true;
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
		i++;
		if(i%20==0)LED0=!LED0;
		if(menu == false)
			display_game_time(10, 270);
	}
	if(menu == false && gameover == false && mode != 2){
		rtp(1);
	}
	if(gameover == true || mode != 2){
		total_secs = seconds;
		summarize(true);
	}
	if(mode == 2 || (mode == 1 && reset_ == true)){
		reset___:
		reset();
	}
}

