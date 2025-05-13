 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/* I2C Status Bits in TWSR Register */
#define TWI_START         0x08  /* Start condition transmitted */
#define TWI_REP_START     0x10  /* Repeated start condition transmitted */
#define TWI_MT_SLA_W_ACK  0x18  /* SLA+W transmitted, ACK received */
#define TWI_MT_SLA_R_ACK  0x40  /* SLA+R transmitted, ACK received */
#define TWI_MT_DATA_ACK   0x28  /* Data transmitted, ACK received */
#define TWI_MR_DATA_ACK   0x50  /* Data received, ACK returned */
#define TWI_MR_DATA_NACK  0x58  /* Data received, NACK returned */

/* Custom Types for Configuration */
typedef uint8  TWI_AddressType;   /* 7-bit slave address */
typedef uint32 TWI_BaudRateType;  /* Bit rate (TWBR value) */

/* Configuration Structure */
typedef struct {
    TWI_AddressType  address;    /* Device's own address (if slave) */
    TWI_BaudRateType bit_rate;   /* TWBR value for SCL frequency */
} TWI_ConfigType;

/* Function Prototypes */
void TWI_init(const TWI_ConfigType *Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);

#endif /* TWI_H_ */
