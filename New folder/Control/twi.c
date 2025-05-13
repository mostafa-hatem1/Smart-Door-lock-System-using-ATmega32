 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/
 
#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>

void TWI_init(const TWI_ConfigType *Config_Ptr) {
    /* Set bit rate register (TWBR) */
	TWBR =  (uint8)(((F_CPU / (Config_Ptr->bit_rate)) - 16)/2);

    /* Clear prescaler bits (TWPS = 00 for prescaler = 1) */
    TWSR = 0x00;

    /* Set device address (shifted left by 1, LSB = GC bit disabled) */
    TWAR = (Config_Ptr->address << 1);

    /* Enable TWI module */
    TWCR = (1 << TWEN);
}

void TWI_start(void) {
    /* Clear TWINT, send START condition, enable TWI */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    /* Wait for TWINT flag (start condition transmitted) */
    while (BIT_IS_CLEAR(TWCR, TWINT));
}

void TWI_stop(void) {
    /* Clear TWINT, send STOP condition, enable TWI */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data) {
    /* Load data into TWDR */
    TWDR = data;
    /* Clear TWINT, enable TWI */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag (data transmitted) */
    while (BIT_IS_CLEAR(TWCR, TWINT));
}

uint8 TWI_readByteWithACK(void) {
    /* Clear TWINT, enable ACK, enable TWI */
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
    /* Wait for TWINT flag (data received) */
    while (BIT_IS_CLEAR(TWCR, TWINT));
    return TWDR;
}

uint8 TWI_readByteWithNACK(void) {
    /* Clear TWINT, enable TWI (no ACK) */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag (data received) */
    while (BIT_IS_CLEAR(TWCR, TWINT));
    return TWDR;
}

uint8 TWI_getStatus(void) {
    /* Read status register (mask upper 3 bits) */
    return (TWSR & 0xF8);
}
