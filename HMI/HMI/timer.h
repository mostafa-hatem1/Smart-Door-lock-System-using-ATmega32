#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Timer IDs                                   *
 *******************************************************************************/
typedef enum {
    TIMER0_ID,    // 8-bit timer
    TIMER1_ID,    // 16-bit timer
    TIMER2_ID     // 8-bit timer (async)
} Timer_ID_Type;

/*******************************************************************************
 *                              Prescaler Options                             *
 *******************************************************************************/
typedef enum {
    TIMER_PRESCALER_OFF,
    TIMER_PRESCALER_1,
    TIMER_PRESCALER_8,
    TIMER_PRESCALER_64,
    TIMER_PRESCALER_256,
    TIMER_PRESCALER_1024
} Timer_PrescalerType;

/*******************************************************************************
 *                                Timer Modes                                 *
 *******************************************************************************/
typedef enum {
    TIMER_MODE_NORMAL,
    TIMER_MODE_CTC
} Timer_ModeType;

/*******************************************************************************
 *                          Configuration Structure                            *
 *******************************************************************************/
typedef struct {
    uint16 initial_value;
    uint16 compare_value;
    Timer_ID_Type timer_id;
    Timer_ModeType mode;
    Timer_PrescalerType prescaler;
} Timer_ConfigType;

/*******************************************************************************
 *                          Function Prototypes                                *
 *******************************************************************************/
void Timer_init(const Timer_ConfigType *config);
void Timer_deInit(Timer_ID_Type timer_id);

/* Separate registration for each type of interrupt */
void Timer_setCallBack_CTC(void (*callback)(void), Timer_ID_Type timer_id);
void Timer_setCallBack_OVF(void (*callback)(void), Timer_ID_Type timer_id);

#endif /* TIMER_H_ */
