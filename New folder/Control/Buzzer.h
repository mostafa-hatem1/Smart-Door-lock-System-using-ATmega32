/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.h
 *
 * Description: Header file for the Buzzer driver
 *
 * Author: Mostafa Hatem
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID   PORTC_ID
#define BUZZER_PIN_ID    PIN7_ID

/*******************************************************************************
 *                              Function Prototypes                            *
 *******************************************************************************/

/*
 * Description:
 * Initializes the Buzzer pin as output.
 */
void Buzzer_init(void);

/*
 * Description:
 * Activates the Buzzer by setting the pin to HIGH.
 */
void Buzzer_on(void);

/*
 * Description:
 * Deactivates the Buzzer by setting the pin to LOW.
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */

