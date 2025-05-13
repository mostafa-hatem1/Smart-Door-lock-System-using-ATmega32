/*
 * PWM.c
 *
 *  Created on: Mar 12, 2025
 *      Author: Hatem
 */
#include <avr/io.h>
#include "PWM.h"
#include "common_macros.h"
#include "std_types.h"

void PWM_Timer0_Start(uint8 duty_cycle)
{
	TCNT0 = TIMER_INITIAL_VALUE; /* Set Timer Initial Value to 0*/

	OCR0  = duty_cycle; /* Set Compare Value*/

	SET_BIT(DDRB,PB3); /* Configure PB3/OC0 as output pin*/


	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/1024 CS00=1 CS01=0 CS02=1
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
