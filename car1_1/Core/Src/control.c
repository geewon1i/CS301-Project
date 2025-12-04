#include "control.h"
#include "tim.h"
#include "gpio.h"

extern TIM_HandleTypeDef htim3; // PWM 使用 TIM3 CH1/CH2
#define FULL_SPEED 999  // 对应 ARR = 999

// ---------- 电机方向控制 ----------
void forward() {
    // 左轮
    HAL_GPIO_WritePin(IN1_L_GPIO_Port, IN1_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_L_GPIO_Port, IN2_L_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN3_L_GPIO_Port, IN3_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_L_GPIO_Port, IN4_L_Pin, GPIO_PIN_SET);
    // 右轮
    HAL_GPIO_WritePin(IN1_R_GPIO_Port, IN1_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_R_GPIO_Port, IN2_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN3_R_GPIO_Port, IN3_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_R_GPIO_Port, IN4_R_Pin, GPIO_PIN_SET);
}

void stop_motors() {
    HAL_GPIO_WritePin(IN1_L_GPIO_Port, IN1_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_L_GPIO_Port, IN2_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN3_L_GPIO_Port, IN3_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_L_GPIO_Port, IN4_L_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(IN1_R_GPIO_Port, IN1_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_R_GPIO_Port, IN2_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN3_R_GPIO_Port, IN3_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_R_GPIO_Port, IN4_R_Pin, GPIO_PIN_RESET);
}

// ---------- 原地旋转 ----------
void turn_left_inplace() {
    // 左轮后退
    HAL_GPIO_WritePin(IN1_L_GPIO_Port, IN1_L_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN2_L_GPIO_Port, IN2_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN3_L_GPIO_Port, IN3_L_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN4_L_GPIO_Port, IN4_L_Pin, GPIO_PIN_RESET);
    // 右轮前进
    HAL_GPIO_WritePin(IN1_R_GPIO_Port, IN1_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_R_GPIO_Port, IN2_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN3_R_GPIO_Port, IN3_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_R_GPIO_Port, IN4_R_Pin, GPIO_PIN_SET);

    // PWM 全速或可调
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, FULL_SPEED / 2);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, FULL_SPEED / 2);
}

void turn_right_inplace() {
    // 左轮前进
    HAL_GPIO_WritePin(IN1_L_GPIO_Port, IN1_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_L_GPIO_Port, IN2_L_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN3_L_GPIO_Port, IN3_L_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_L_GPIO_Port, IN4_L_Pin, GPIO_PIN_SET);
    // 右轮后退
    HAL_GPIO_WritePin(IN1_R_GPIO_Port, IN1_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN2_R_GPIO_Port, IN2_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN3_R_GPIO_Port, IN3_R_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN4_R_GPIO_Port, IN4_R_Pin, GPIO_PIN_RESET);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, FULL_SPEED / 2);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, FULL_SPEED / 2);
}

// ---------- 初始化 PWM ----------
void init_motors() {
//    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
//    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
//    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, FULL_SPEED / 2);
//    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, FULL_SPEED / 2);
	stop_motors();
}

// ---------- PWM 映射函数 ----------
uint16_t pwm_map(float duty) {
    if(duty < 0) duty = 0;
    if(duty > 1) duty = 1;
    return (uint16_t)(duty * FULL_SPEED);
}
