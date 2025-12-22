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
    pose.x = 0;
    pose.y = 0;
    pose.theta = 0;
}

void traj_update(float v_l, float v_r, int a)
{
    float v = (v_l + v_r) * 0.5f;
    float omega = (v_r - v_l) / L_WHEEL;

    pose.theta += omega * DT;
    pose.x += v * cosf(pose.theta) * DT;
    pose.y += v * sinf(pose.theta) * DT;
    if(a==1){
    	char buf[32];
    	int len = sprintf(buf, "%.2f,%.2f\r\n", pose.x, pose.y);
    	HAL_UART_Transmit(&huart1,(uint8_t *)buf,len,0xffff);
    }
}

Pose2D traj_get_pose(void)
{
    return pose;
}
