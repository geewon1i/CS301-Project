#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

// 基本运动控制
void init_motors(void);
void forward_half(void);
void turn_left_half(void);
void turn_right_half(void);
extern void stop_motors(void);
extern void turn_in_place(int angle);
extern void forward_with_length(void);
#endif
