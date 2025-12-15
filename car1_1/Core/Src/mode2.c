#include "mode2.h"
#include "control.h"
#include "tim.h"
#include "gpio.h"
#include "trajectory.h"

#define BASE_SPEED 0.5f   // 基础前进速度（0~1）
#define Kp 0.4f           // 转向比例系数
#define DEAD_ZONE 0.05f   // 偏差死区
#define V_HALF  20.0f   // 半速前进 ≈ 20 cm/s（你实测后改）

// PWM 映射函数
extern uint16_t pwm_map(float duty);


void mode2_loop(void){
    init_motors();

    while(1){
        // 读取左右避障传感器数字信号
    	uint8_t left_detect  = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4); // 0 = 左边有物体
    	uint8_t right_detect = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5); // 0 = 右边有物体


        // --- 1️⃣ 无物体 → 停止 ---
        if(left_detect && right_detect){
            stop_motors();
            //traj_update(0, 0);

        }
        // --- 2️⃣ 左无右有 → 原地右转 ---
        else if(!left_detect && right_detect){
            turn_right_half();
            //traj_update(V_HALF, 0);
        }
        // --- 3️⃣ 右无左有 → 原地左转 ---
        else if(left_detect && !right_detect){
            turn_left_half();
            //traj_update(0, V_HALF);
        }
        // --- 4️⃣ 左右都检测到 → 前进 / 平滑跟随 ---
        else if(!left_detect && !right_detect){


            forward_half();
            //traj_update(V_HALF, V_HALF);
        }

        HAL_Delay(10); // 控制周期 10ms
    }
}
