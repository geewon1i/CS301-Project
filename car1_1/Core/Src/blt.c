/*
 * blt.c
 *
 *  Created on: Dec 23, 2025
 *      Author: iris1
 */
#include "blt.h"
#include <stdlib.h>
#define RX_BUFFER_SIZE 64
#define CMD_START 0x53
#define CMD_END 0x45
#define CMD_MOVE_TO 0x01

uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_data;
uint8_t rx_index = 0;
uint8_t command_received = 0;

int px = 0;
int py = 320;
//extern int move_time;
//extern int turn_time;

int current_angle=90;

/* 命令结构体 */

Bluetooth_Command current_cmd;
extern UART_HandleTypeDef huart1;

/* 初始化UART接收 */
void Bluetooth_Init(void)
{
    // 启动中断接收
    HAL_UART_Receive_IT(&huart1, &rx_data, 1);
}

/* UART接收完成回调函数 */
void mode1_init(void){
	px = 0;
	py = 320;
	current_angle=90;
	rx_index = 0;
	current_cmd.start_flag = rx_buffer[0];
}
/* 解析命令 */
void Parse_Command(void)
{
    if (rx_buffer[0] == CMD_START && rx_buffer[rx_index-1] == CMD_END)
    {
    	//HAL_UART_Transmit(&huart1, rx_buffer, 7, 50);
        current_cmd.start_flag = rx_buffer[0];
        current_cmd.command = rx_buffer[1];

        // 提取坐标
        current_cmd.x = (rx_buffer[2] << 8) | rx_buffer[3];
        current_cmd.y = (rx_buffer[4] << 8) | rx_buffer[5];

        current_cmd.end_flag = rx_buffer[rx_index-1];

        // 根据命令执行相应操作
        Execute_Command();
    }
}

/* 执行命令 */
void Execute_Command(void)
{
    switch (current_cmd.command)
    {
        case CMD_MOVE_TO:
            // 移动到指定坐标
            int x = current_cmd.x;
            int y = current_cmd.y;
            /*char info[30];
            sprintf(info, "OK: %d,%d", px,py);
            HAL_UART_Transmit(&huart1, (uint8_t*)info, strlen(info), 50);
            sprintf(info, "OK: %d,%d", x,y);
            HAL_UART_Transmit(&huart1, (uint8_t*)info, strlen(info), 50);*/

            // 计算移动角度和距离
            int diff_y = py - y;
            int diff_x = x - px;
            int turning_time = 0;
            int rot_angle = 0;

            // 角度计算
            if(diff_x == 0) {
                if(diff_y < 0) {
                    rot_angle = current_angle + 90;
                    current_angle = -90;
                }else{
                	rot_angle = 90 - current_angle;
                	current_angle = 90.0;
                }
            }
            else if(diff_y == 0) {
                if(diff_x > 0) {
                	rot_angle = current_angle;
                    current_angle = 0;
                } else {
                	if(current_angle > 0){
                		rot_angle = current_angle - 180;
                	}else{
                		rot_angle = current_angle + 180;
                	}
                	current_angle = -180;
                }
            }
            else {
                float rad = atan2f(diff_y, diff_x);
                int ang = rad * 60;
                rot_angle = current_angle - ang;

                // 角度归一化到[-180, 180]
                if(rot_angle > 180) rot_angle -= 360;
                else if(rot_angle < -180) rot_angle += 360;


                current_angle = ang;
            }

            //man!
            /*if(-10 < diff_x && diff_x < 10) {
                if(diff_y < 0) {
                    rot_angle = current_angle + 90;
                    current_angle = -90;
                }else{
                	rot_angle = 90 - current_angle;
                	current_angle = 90.0;
                }
            }
            else if(-10 < diff_y && diff_y < 10) {
                if(diff_x > 0) {
                	rot_angle = current_angle;
                    current_angle = 0;
                } else {
                	if(current_angle > 0){
                		rot_angle = current_angle - 180;
                	}else{
                		rot_angle = current_angle + 180;
                	}
                	current_angle = -180;
                }
            }
            else {
            	int ang=0;
                if(diff_y > 0 && diff_x > 0){
                	ang = 45;
                }else if(diff_y > 0 && diff_x < 0){
                	ang = 135;
                }else if (diff_y < 0 && diff_x >0){
                	ang = -45;
                }else{
                	ang = -135;
                }
                rot_angle = current_angle - ang;

                // 角度归一化到[-180, 180]
                if(rot_angle > 180) rot_angle -= 360;
                else if(rot_angle < -180) rot_angle += 360;


                current_angle = ang;
            }*/


            int distance = (int) sqrt(diff_x * diff_x + diff_y * diff_y);
            turning_time = 7 * abs(rot_angle);
            int moving_time = distance *10;
            px = x;
            py = y;

            turn_in_place(rot_angle);
            HAL_Delay(turning_time);
            stop_motors();

            forward_with_length();
            HAL_Delay(moving_time);
            stop_motors();
            /*sprintf(info, "OK2: %d,%d", turning_time,distance);
            HAL_UART_Transmit(&huart1, (uint8_t*)info, strlen(info), 50);*/

            // 更新当前位置

            // 发送回传信息
            //Send_Response("MOVED");
            break;


        default:
            //Send_Response("UNKNOWN_CMD");
        	turn_in_place(90);
        	HAL_Delay(7*90);stop_motors();
            //traj_update(0, 0);

        	forward_with_length();
        	HAL_Delay(25*100);stop_motors();
            //traj_update(0, 0);

        	turn_in_place(-90);
        	HAL_Delay(7*90*0.9);stop_motors();
            //traj_update(0, 0);

        	forward_with_length();
        	HAL_Delay(25 * 50);stop_motors();
            //traj_update(0, 0);

        	turn_in_place(-90);
        	HAL_Delay(7 * 90 *0.9);stop_motors();
            //traj_update(0, 0);

        	forward_with_length();
        	HAL_Delay(25*100);stop_motors();
            //traj_update(0, 0);

        	turn_in_place(90);
        	HAL_Delay(7 *90);stop_motors();
            //traj_update(0, 0);

        	forward_with_length();
        	HAL_Delay(25 * 50);stop_motors();
            //traj_update(0, 0);

        	turn_in_place(90);
        	HAL_Delay(7 * 90);stop_motors();
            //traj_update(0, 0);

        	forward_with_length();
        	HAL_Delay(25 * 100);stop_motors();
            //traj_update(0, 0);
            break;
    }
}

/* 发送回传信息 */
void Send_Response(char *response)
{
    char msg[32];
    sprintf(msg, "ACK:%s,X=%d,Y=%d", response, current_cmd.x, current_cmd.y);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
}
