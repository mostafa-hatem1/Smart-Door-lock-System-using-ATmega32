#include "pir.h"
#include "gpio.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void PIR_init(void) {
    /* Configure PIR pin as input (no pull-up needed for most PIR sensors) */
    GPIO_setupPinDirection(PIR_PORT_ID, PIR_PIN_ID, PIN_INPUT);
}

uint8 PIR_getState(void) {
    /* Read and return the PIR's digital output */
    return GPIO_readPin(PIR_PORT_ID, PIR_PIN_ID);
}
