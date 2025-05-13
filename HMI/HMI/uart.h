/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
    UART_5_BIT_DATA = 0,  /* UCSZ1:0 = 00 */
    UART_6_BIT_DATA = 1,  /* UCSZ1:0 = 01 */
    UART_7_BIT_DATA = 2,  /* UCSZ1:0 = 10 */
    UART_8_BIT_DATA = 3   /* UCSZ1:0 = 11 */
} UART_BitDataType;

typedef enum {
    UART_PARITY_DISABLED,
    UART_PARITY_EVEN = 2,
    UART_PARITY_ODD = 3
} UART_ParityType;

typedef enum {
    UART_1_STOP_BIT,
    UART_2_STOP_BITS
} UART_StopBitType;

typedef struct {
    UART_BitDataType bit_data;
    UART_ParityType parity;
    UART_StopBitType stop_bit;
    uint32 baud_rate;
} UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif 
