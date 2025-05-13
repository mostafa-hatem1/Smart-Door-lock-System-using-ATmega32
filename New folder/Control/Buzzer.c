/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.c
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Mostafa Hatem
 *
 *******************************************************************************/

#include "Buzzer.h"
#include "gpio.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * Initializes the Buzzer pin as an output and ensures it is turned off initially.
 */
void Buzzer_init(void)
{
    GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

/*
 * Description:
 * Turns ON the Buzzer by setting the corresponding pin HIGH.
 */
void Buzzer_on(void)
{
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}

/*
 * Description:
 * Turns OFF the Buzzer by setting the corresponding pin LOW.
 */
void Buzzer_off(void)
{
    GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}


