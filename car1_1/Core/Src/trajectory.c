#include "trajectory.h"
#include <math.h>
#include "stm32f1xx_it.h"
#include "usart.h"
#define DT       0.01f   // 10ms
#define L_WHEEL  14.0f   // cm

static Pose2D pose;
//extern UART_HandleTypeDef huart1;
void traj_init(void)
{
    pose.x = 100;//0;
    pose.y = 200;//320;
    pose.theta = 1.57f;//pi/2
}

void traj_update(float v_l, float v_r, int a)
{
    float v = (v_l + v_r) * 5.0f;
    float omega = (v_r - v_l) / L_WHEEL;
    static int pulse = 0;

    pose.theta += omega * DT;
    pose.x += v * cosf(pose.theta) * DT;
    pose.y -= v * sinf(pose.theta) * DT;
    if(a==1 && pose.x >0 && pose.y > 0){
    	if (pulse < 20){
    		pulse ++;
    		return;
    	}
    	if(pose.x > 240){
    		pose.x = 60;
    	}
    	if(pose.y>320){
    		pose.y=200;
    	}
    	uint16_t x =pose.x;
    	uint16_t y =pose.y;

        uint8_t tx_buffer[7];
        // 构建简单命令帧
        tx_buffer[0] = CMD_START;     // 开始标志
        tx_buffer[1] = 0x02;//CMD_MOVE_TO;   // 命令类型
        tx_buffer[3] = x & 0xFF;      // X坐标低字节
        tx_buffer[2] = (x >> 8) & 0xFF; // X坐标高字节
        tx_buffer[5] = y & 0xFF;      // Y坐标低字节
        tx_buffer[4] = (y >> 8) & 0xFF; // Y坐标高字节
        /*tx_buffer[3] = 0x80;
        tx_buffer[2] = 0x00;
        tx_buffer[5] = 0x01;
        tx_buffer[4] = 0x01;*/
        tx_buffer[6] = CMD_END;       // 结束标志
        //tx_buffer[7] = CMD_END;       // 结束标志
        //eg:53020043010145->x:0x0043,y:0x0101

        // 发送命令
        //HAL_UART_Transmit(&huart1, tx_buffer, 7, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, tx_buffer, 2, 50);
        HAL_Delay(100);
        HAL_UART_Transmit(&huart1, tx_buffer+2, 2, 50);
        HAL_Delay(100);
        HAL_UART_Transmit(&huart1, tx_buffer+4, 2, 50);
        HAL_Delay(100);
        HAL_UART_Transmit(&huart1, tx_buffer+6, 1, 50);
        HAL_Delay(100);
        /*for(int i=0;i<7;i++){
        	HAL_UART_Transmit(&huart1, tx_buffer+i, 1, HAL_MAX_DELAY);
        }*/
        pulse = 0;
    }
}

Pose2D traj_get_pose(void)
{
    return pose;
}
