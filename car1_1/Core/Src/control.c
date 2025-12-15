#include "control.h"
#include "tim.h"
#include "gpio.h"

/* 外部句柄 */
extern TIM_HandleTypeDef htim8;   // 硬件 PWM（PC9 / PC7）
extern TIM_HandleTypeDef htim3;   // 软件 PWM 定时器

/* 固定半速 */
#define HALF_PWM 500     // ARR = 999 → 半速

/* 软件 PWM 状态 */
volatile uint8_t sw_pwm_on = 0;

/* ================= 初始化 ================= */

void init_motors(void)
{
    /* 软件 PWM 定时器启动 */
    //HAL_TIM_Base_Start_IT(&htim3);

    /* 硬件 PWM：PC9 / PC7 */   //有问题
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);

    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, HALF_PWM);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, HALF_PWM);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,  GPIO_PIN_SET);

    sw_pwm_on = 0;

}

/* ================= 软件 PWM（PC10 / PC2） ================= */
/* TIM6 中断，周期建议 1ms */



/* ================= 方向控制 ================= */

void forward_half(void)
{
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

	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, HALF_PWM);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, HALF_PWM);




}

void turn_left_half(void)
{
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

	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, HALF_PWM);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, HALF_PWM);

}

void turn_right_half(void)
{
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
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, HALF_PWM);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, HALF_PWM);

}

void stop_motors(void)
{
    /* 硬件 PWM 清零 */
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 0);



    /* 方向清零 */
    HAL_GPIO_WritePin(IN1_L_GPIO_Port, IN1_L_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN2_L_GPIO_Port, IN2_L_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN3_L_GPIO_Port, IN3_L_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN4_L_GPIO_Port, IN4_L_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(IN1_R_GPIO_Port, IN1_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN2_R_GPIO_Port, IN2_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN3_R_GPIO_Port, IN3_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN4_R_GPIO_Port, IN4_R_Pin, GPIO_PIN_RESET);
}

void turn_in_place(float angle) {
    const uint16_t turn_time_per_degree = 10; //tbd
    uint32_t turn_time = fabs(angle) * turn_time_per_degree;
    
    if(angle < 0) {  // turn left(ccw)
        turn_left_half();
        HAL_Delay(turn_time);
    } else {  // turn right(cw)
        turn_right_half();
        HAL_Delay(turn_time);
    }
    
    stop_motors();
}
void forward_with_length(float length) {
    //2m * 2m, 200 units * 200 units 
    const uint16_t forward_time_per_centi_meter = 10; //tbd
    uint32_t forward_time = fabs(length) * forward_time_per_centi_meter;
    forward_half();
    HAL_Delay(turn_time);
    
    stop_motors();
}
