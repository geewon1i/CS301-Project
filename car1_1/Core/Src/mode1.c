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

    // 等待开始信号
    while(1) {
        if(rx_ready) {
            parse_command(rx_buffer);
            rx_ready = 0;

            if(receiving_path) {
                break;
            }
        }
        HAL_Delay(10);
    }

    // 接收路径点直到结束
    while(receiving_path) {
        if(rx_ready) {
            parse_command(rx_buffer);
            rx_ready = 0;
        }
        HAL_Delay(10);
    }

    // 路径接收完成，开始运动
    if(path_received && point_count > 0) {
        execute_path();
    }

    // 重置状态
    path_received = 0;
    point_count = 0;
}

// 执行路径运动
void execute_path(void) {

    float current_angle = 90.0;  // 初始角度，指向正上方
    uint16_t x0 = 0;
    uint16_t y0 = 320;           // 假设起始点

    for(int i = 0; i < point_count; i++) {
        uint16_t x = path_points[i].x;
        uint16_t y = path_points[i].y;


        // 计算移动角度和距离
        int diff_y = y0 - y;
        int diff_x = x - x0;

        // 角度计算（保持你的原有逻辑）
        if(diff_x == 0) {
            if(diff_y > 0) {
                turn_in_place(180.0);
                HAL_Delay(turn_time * 2);
                stop_motors();
                current_angle -= 180.0;
                if(current_angle < -180) current_angle += 360.0;
            }
        }
        else if(diff_y == 0) {
            if(diff_x > 0) {
                turn_in_place(current_angle - 90.0);
                uint8_t turning_time = turn_time * fabs(current_angle - 90.0);
                HAL_Delay(turning_time);
                stop_motors();
                current_angle = 90.0;
            } else {
                turn_in_place(current_angle + 90.0);
                uint8_t turning_time = turn_time * fabs(current_angle + 90.0);
                HAL_Delay(turning_time);
                stop_motors();
                current_angle = -90.0;
            }
        }
        else {
            float rad = atan2f(diff_y, diff_x);
            float ang = rad * 180.0 / PI;
            float rot_angle = current_angle - ang;

            // 角度归一化到[-180, 180]
            if(rot_angle > 180) rot_angle -= 360.0;
            else if(rot_angle < -180) rot_angle += 360.0;

            turn_in_place(rot_angle);
            uint8_t turning_time = turn_time * fabs(rot_angle);
            HAL_Delay(turning_time);
            stop_motors();
            current_angle = ang;
        }

        // 计算移动距离
        float distance = sqrtf(diff_x * diff_x + diff_y * diff_y);
        forward_with_length();
        HAL_Delay(move_time * distance / 100);
        stop_motors();

        // 更新当前位置
        x0 = x;
        y0 = y;

        // 可选：发送当前位置回传
        //char feedback[30];
        //snprintf(feedback, 30, "AT:%d,%d", x, y);
        //bt_send_string(feedback);

        HAL_Delay(100);  // 点之间的小延迟
    }

}
