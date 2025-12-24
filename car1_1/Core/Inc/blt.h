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
#include "control.h"
#include "math.h"
typedef struct {
    uint8_t start_flag;
    uint8_t command;
    uint16_t x;
    uint16_t y;
    uint8_t end_flag;
} Bluetooth_Command;

void Parse_Command(void);
void Execute_Command(void);
void Send_Response(char *response);
void mode1_init(void);

#endif /* INC_BLT_H_ */
