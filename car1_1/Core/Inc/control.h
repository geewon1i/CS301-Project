#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

// 基本运动控制
void forward(void);
void backward(void);
void turn_left(void);
void turn_right(void);
void stop_motors(void);

// 初始化（开启 PWM，全速）
void init_motors(void);

#endif
