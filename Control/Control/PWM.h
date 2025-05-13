/*
 * PWM.h
 *
 *  Created on: Mar 12, 2025
 *      Author: Hatem
 */

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"
#include "gpio.h"

#define TIMER_INITIAL_VALUE 0

#define PWM_PORT_ID PORTB_ID
#define PWN_PIN_ID	PIN3_ID

void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
