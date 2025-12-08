#include "mode2.h"
#include "control.h"
#include "tim.h"
#include "gpio.h"

#define BASE_SPEED 0.5f   // 基础前进速度（0~1）
#define Kp 0.4f           // 转向比例系数
#define DEAD_ZONE 0.05f   // 偏差死区


// PWM 映射函数
extern uint16_t pwm_map(float duty);
extern TIM_HandleTypeDef htim3;

// 前进 / 停止 / 转向函数来自 control.c
extern void init_motors(void);
extern void forward(void);
extern void stop_motors(void);
extern void turn_left_inplace(void);
extern void turn_right_inplace(void);

void mode2_loop(void){
    init_motors();

    while(1){
        // 读取左右避障传感器数字信号
    	uint8_t left_detect  = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4); // 0 = 左边有物体
    	uint8_t right_detect = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5); // 0 = 右边有物体


        // --- 1️⃣ 无物体 → 停止 ---
        if(left_detect && right_detect){
            stop_motors();
        }
        // --- 2️⃣ 左有u右无 → 原地右转 ---
        else if(!left_detect && right_detect){
            turn_right_half();
        }
        // --- 3️⃣ 右有左无 → 原地左转 ---
        else if(left_detect && !right_detect){
            turn_left_half();
        }
        // --- 4️⃣ 左右都检测到 → 前进 / 平滑跟随 ---
        else if(!left_detect && !right_detect){


            forward_half();
        }

        HAL_Delay(10); // 控制周期 10ms
    }
}
