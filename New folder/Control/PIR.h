#ifndef PIR_H_
#define PIR_H_

#include "gpio.h"
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* PIR Sensor Connection (Update these if hardware changes) */
#define PIR_PORT_ID     PORTC_ID
#define PIR_PIN_ID      PIN2_ID

/*******************************************************************************
 *                              Function Prototypes                            *
 *******************************************************************************/

/**
 * @brief Initialize the PIR sensor pin as input.
 */
void PIR_init(void);

/**
 * @brief Read the PIR sensor's current state.
 * @return uint8: LOGIC_HIGH (motion detected) or LOGIC_LOW (no motion).
 */
uint8 PIR_getState(void);

#endif /* PIR_H_ */
