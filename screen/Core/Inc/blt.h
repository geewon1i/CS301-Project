/*
 * blt.h
 *
 *  Created on: Dec 23, 2025
 *      Author: iris1
 */

#ifndef INC_BLT_H_
#define INC_BLT_H_

#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
typedef struct {
    uint8_t start_flag;
    uint8_t command;
    uint16_t x;
    uint16_t y;
    uint8_t end_flag;
} Bluetooth_Command;
void Process_Response(uint8_t data);
#endif /* INC_BLT_H_ */
