#include "gpio.h"
#include "main.h"
#include "lcd_v4.h"
#include "lcd.h"
#include "time.h"
#include <math.h>
#include <stdbool.h>
#include "image.h"
#include "maze.h"
#include "lcd.h"

int number, index_; //学号和对应的学号坐标
bool flag;
char str[4]="0___";
int total_secs, id;
bool reset_;

void game(){

	LCD_ShowPicture(0, 0, 255, 338, (uint16_t*)gImage_eddie);
	lcd_show_string(30, 30, 200, 24, 24, "Maze Quest", RED);
	lcd_show_string(30, 70, 200, 24, 24, "Start Game", RED);
	lcd_show_string(30, 110, 200, 24, 24, "Ranking", RED);
	lcd_show_string(30, 150, 200, 24, 16, "Enter your id below:", RED);
	reset_ = false;
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

	number = 0;
	index_ = 0;
	int timer = 0;
	start_playing = false;
	id_number:
	while(HAL_GPIO_ReadPin(KEY_WK_UP_GPIO_Port, KEY_WK_UP_Pin) !=
			GPIO_PIN_SET); // do nothing before wake up is pressed
	lcd_show_string(30, 180, 200, 24, 16, str, RED);
	id = 0;
	while(true){ // 学号输入环节
		detect_keys();
		if(flag == true)
			break;
	}
	lcd_show_string(30, 180, 200, 24, 16, "    ", RED);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	rtp(0);
}

void summarize(bool state){
	if(state == true){
		if(reset_ == true)
			goto reset__;
		char str[100];
		lcd_fill(0, 0, 255, 300, MAGENTA);
		lcd_show_string(30, 80, 200, 24, 24, "MAZE DONE", BLACK);
		lcd_show_string(30, 110, 200, 24, 24, "ID: ", BLACK);
		sprintf(str, "%04d", id);
		lcd_show_string(30, 140, 200, 24, 24, str, BLACK);
		lcd_show_string(30, 170, 200, 24, 24, "TIME SPENT:", BLACK);
		sprintf(str, "%03ds", total_secs);
		lcd_show_string(30, 200, 200, 24, 24, str, BLACK);
		lcd_show_string(30, 230, 200, 24, 24, "RESTART?", BLACK);
		rtp(2);
	}
	reset__:
	if(reset_ == true)
		reset();
}

void detect_keys(){ // 学号输入，已初步实现
	if(flag==true)return;
	if(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET){
		str[index_] = (char)(number + (int)('0'));//给这一位赋值
		id += number;
		number = 0;
		if(index_+1>3){
			flag = true; //结束输入
			return;
		}
		id *= 10;
		index_++;
		str[index_]='0'; //挪到下一位
		lcd_show_string(30, 180, 200, 24, 16, str, RED);
		HAL_Delay(200);
	}
	else if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET){ //循环数字
		number++;
		str[index_] = (char)(number + (int)('0'));
		if(number > 9) number = 0;
		lcd_show_string(30, 180, 200, 24, 16, str, RED);
		HAL_Delay(200);
	}
}

void reset(){
	//	append_game_record(id, total_secs);
	number = 0;
	index_ = 0; //学号和对应的学号坐标
	flag = false;
	str[0]='0';
	for(int i=1;i<4;i++)str[i]='_';
	total_secs=0;
	id=0;
	reset_=false;
	menu = true;
	start_playing = false;
	timer = 0;
	px=22, py=142, pix=0, piy=4;
	gameover = false;
	//	game_start_tick = 0;  // 游戏开始时的tick值
	game();
}
