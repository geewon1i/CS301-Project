#include "mode1.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "math.h"

#define PI 3.14159265

struct Point{
    uint16_t x;
    uint16_t y;
}current_point,next_point;
void mode1_loop(void) {
    // 留给路线规划组
    float current_angle = 90.0; // init(0,1), -180~180
    uint16_t x0 = 0;
    uint16_t y0 = 0;
    //bt_transmit
    uint8_t order = 2; // 1: getting points 0: over
    while(order == 1){
    	uint16_t x = 20;
    	uint16_t y = 30;
        //x = bt...
        int diff_y = y - y0;
        int diff_x = x - x0;
        if(x0 == 0){
            if(y < 0){
                turn_in_place(180.0);
                current_angle -= 180.0;
                if(current_angle < -180){
                    current_angle += 360.0;
                }
            }
        }else{
            float rad = atan2f(diff_y, diff_x);
            float ang = rad * 180.0 / PI;
            float rot_angle = current_angle - ang;
            if(rot_angle > 180){
                rot_angle -= 360.0;
            }else if(rot_angle < -180){
                rot_angle += 360.0;
            }
            turn_in_place(rot_angle);
            current_angle = ang;
        }
        float length = diff_y * diff_y + diff_x * diff_x;
        length = sqrtf(length);
        forward_with_length(length);
        x0 = x;
        y0 = y;
    }
    while(order == 2){
    	turn_in_place(90);
    	forward_with_length(100);
    	turn_in_place(-90);
    	forward_with_length(50);
    	turn_in_place(-90);
    	forward_with_length(100);
    	turn_in_place(90);
    	forward_with_length(50);
    	turn_in_place(90);
    	forward_with_length(100);
    	break;
    }
}
