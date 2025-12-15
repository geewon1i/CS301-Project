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

void game(){
	lcd_show_string(30, 30, 200, 24, 24, "Little car uwu", RED);
	lcd_show_string(30, 70, 200, 24, 24, "Route planning", RED);
	lcd_show_string(30, 110, 200, 24, 24, "Following", RED);
	lcd_show_string(30, 150, 200, 24, 16, "SD card storing", RED);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	lcd_show_string(30, 180, 200, 24, 16, "    ", RED);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	HAL_Delay(200);
	rtp(0);
}
