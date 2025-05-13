/*
 * Motor.h
 *
 *  Created on: Mar 10, 2025
 *      Author: Hatem
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"
#include "gpio.h"

/* Enum for DC Motor states */
typedef enum {
    MOTOR_CW,
    MOTOR_ACW,
    MOTOR_STOP
} DcMotor_State;

/* Motor Configuration */
#define MOTOR_IN1_PORT_ID  PORTD_ID
#define MOTOR_IN1_PIN_ID   PIN6_ID
#define MOTOR_IN2_PORT_ID  PORTD_ID
#define MOTOR_IN2_PIN_ID   PIN7_ID

/* Function Declarations */
void Motor_init(void);
void Motor_rotate(DcMotor_State state, uint8 speed);

#endif /* MOTOR_H_ */
