#include "mode1.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "math.h"
#include "control.h"
#include "stm32f1xx_it.h"

#define PI 3.14159265

struct Point{
    uint16_t x;
    uint16_t y;
}current_point,next_point;
int turn_time = 70;
int move_time = 25;
void mode1_loop(void) {
    // 留给路线规划组
    float current_angle = 90.0; // init(0,1), -180~180
    uint16_t x0 = 0;
    uint16_t y0 = 320;
    char ch[100];
    // 假设 rxBuffer 是以 '\0' 结尾的字符串，ch 有足够空间
    //const char *p = rxBuffer;
    //if (*p == '\n') p++;        // 跳过单个 '\n'
    //strcpy(ch, p);              // 复制剩余内容
    //sscanf(ch,"%d,%d",x0,y0);
    //bt_transmit
    uint16_t order = 1; // 1: getting points 0: over
    uint8_t current_count = 0;
    uint16_t x;
    uint16_t y;
    num_count=0;
    receive_number(&order, &current_count);
    while(order != 0){
    	receive_number(&x, &current_count);
    	receive_number(&y, &current_count);
        int diff_y = y0 - y;//!
        int diff_x = x - x0;
        if(diff_x == 0){
            if(diff_y > 0){
                turn_in_place(180.0);
                HAL_Delay(turn_time*2);stop_motors();
                current_angle -= 180.0;
                if(current_angle < -180){
                    current_angle += 360.0;
                }
            }
        }else if (diff_y == 0){
        	if(diff_x > 0){
        		turn_in_place(current_angle);
        		uint8_t turning_time = turn_time * (current_angle);
        		HAL_Delay(turning_time);stop_motors();
        		current_angle = 1;
        	}else{
        		if(current_angle<0){
        			turn_in_place(current_angle+180.0);
        			uint8_t turning_time = turn_time * (current_angle+180.0);
        			HAL_Delay(turning_time);stop_motors();
        		}else{
        			turn_in_place(current_angle-180.0);
        			uint8_t turning_time = turn_time * (current_angle-180.0);
        			HAL_Delay(turning_time);stop_motors();
        		}
        		current_angle = -179;
        	}
        }
        else{
            float rad = atan2f(diff_y, diff_x);
            float ang = rad * 180.0 / PI;
            float rot_angle = current_angle - ang;
            if(rot_angle > 180){
                rot_angle -= 360.0;
            }else if(rot_angle < -180){
                rot_angle += 360.0;
            }
            turn_in_place(rot_angle);
            uint8_t turning_time = turn_time * rot_angle;
            HAL_Delay(turning_time);stop_motors();
            current_angle = ang;
        }
        float length = diff_y * diff_y + diff_x * diff_x;
        length = sqrtf(length);
        forward_with_length();
        HAL_Delay(move_time * length /100);
        stop_motors();
        x0 = x;
        y0 = y;
        receive_number(&order, &current_count);
    }
    if(order == 0){
    	num_count = 0;
    }
    /*while(order == 2){
    	int turn_time = 630;
    	int move_time = 2500;
    	init_motors();
    	turn_in_place(90);
    	HAL_Delay(turn_time);stop_motors();
        traj_update(0, 0);

    	forward_with_length(100);
    	HAL_Delay(move_time);stop_motors();
        traj_update(0, 0);

    	turn_in_place(-90);
    	HAL_Delay(turn_time*0.9);stop_motors();
        traj_update(0, 0);

    	forward_with_length(50);
    	HAL_Delay(move_time / 2);stop_motors();
        traj_update(0, 0);

    	turn_in_place(-90);
    	HAL_Delay(turn_time*0.9);stop_motors();
        traj_update(0, 0);

    	forward_with_length(100);
    	HAL_Delay(move_time);stop_motors();
        traj_update(0, 0);

    	turn_in_place(90);
    	HAL_Delay(turn_time);stop_motors();
        traj_update(0, 0);

    	forward_with_length(50);
    	HAL_Delay(move_time / 2);stop_motors();
        traj_update(0, 0);

    	turn_in_place(90);
    	HAL_Delay(turn_time);stop_motors();
        traj_update(0, 0);

    	forward_with_length(100);
    	HAL_Delay(move_time);stop_motors();
        traj_update(0, 0);
    	break;
    }*/
}
