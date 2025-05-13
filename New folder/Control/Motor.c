/*
 * Motor.c
 *
 *  Created on: Mar 10, 2025
 *      Author: Hatem
 */
#include "Motor.h"
#include "gpio.h"
#include "PWM.h"
#include "std_types.h"

void Motor_init(void) {
    /* Set motor control pins as output */
    GPIO_setupPinDirection(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
    GPIO_setupPinDirection(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);

    /* Stop the motor initially */
    GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID,LOGIC_LOW);
    GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
}

void Motor_rotate(DcMotor_State state, uint8 speed) {
    /* Adjust motor direction */
    switch (state) {
        case MOTOR_CW:
            GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_HIGH);
            GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
            break;
        case MOTOR_ACW:
            GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
            GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
            break;
        case MOTOR_STOP:
            GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
            GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
            break;
        default:
            /* Handle unexpected state - Stop the motor as a safe fallback */
            GPIO_writePin(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
            GPIO_writePin(MOTOR_IN2_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
            break;
    }

    /* Set motor speed using PWM (0 to 100%) */
    PWM_Timer0_Start(speed);
}
