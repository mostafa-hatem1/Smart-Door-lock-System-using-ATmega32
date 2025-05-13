#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*---- Callback function pointers for timer interrupts ----*/
static void (*Timer0_Callback_CTC)(void) = NULL_PTR;
static void (*Timer0_Callback_OVF)(void) = NULL_PTR;
static void (*Timer1_Callback_CTC)(void) = NULL_PTR;
static void (*Timer1_Callback_OVF)(void) = NULL_PTR;
static void (*Timer2_Callback_CTC)(void) = NULL_PTR;
static void (*Timer2_Callback_OVF)(void) = NULL_PTR;

/*---- Internal function to set timer prescaler ----*/
static void Timer_setPrescaler(Timer_ID_Type timer_id, Timer_PrescalerType prescaler) {
    switch (timer_id) {
        case TIMER0_ID:
            /*---- Clear and set Timer0 prescaler bits ----*/
            TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
            TCCR0 |= prescaler;
            break;
        case TIMER1_ID:
            /*---- Clear and set Timer1 prescaler bits ----*/
            TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
            TCCR1B |= prescaler;
            break;
        case TIMER2_ID:
            /*---- Clear and set Timer2 prescaler bits ----*/
            TCCR2 &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
            TCCR2 |= prescaler;
            break;
    }
}

/*---- Initialize timer with specified configuration ----*/
void Timer_init(const Timer_ConfigType *config) {
    switch (config->timer_id) {
        case TIMER0_ID:
            /*---- Timer0 initialization ----*/
            TCNT0 = (uint8)config->initial_value;
            TCCR0 = (1 << FOC0); /*---- Force Output Compare ----*/
            if (config->mode == TIMER_MODE_CTC) {
                /*---- Configure Timer0 CTC mode ----*/
                TCCR0 |= (1 << WGM01);
                OCR0 = (uint8)config->compare_value;
                TIMSK |= (1 << OCIE0); /*---- Enable CTC interrupt ----*/
            } else {
                TIMSK |= (1 << TOIE0); /*---- Enable overflow interrupt ----*/
            }
            break;

        case TIMER1_ID:
            /*---- Timer1 initialization ----*/
            TCNT1 = config->initial_value;
            TCCR1A = 0; /*---- Normal port operation ----*/
            if (config->mode == TIMER_MODE_CTC) {
                /*---- Configure Timer1 CTC mode ----*/
                TCCR1B = (1 << WGM12);
                OCR1A = config->compare_value;
                TIMSK |= (1 << OCIE1A); /*---- Enable CTC interrupt ----*/
            } else {
                TIMSK |= (1 << TOIE1); /*---- Enable overflow interrupt ----*/
            }
            break;

        case TIMER2_ID:
            /*---- Timer2 initialization ----*/
            TCNT2 = (uint8)config->initial_value;
            TCCR2 = (1 << FOC2); /*---- Force Output Compare ----*/
            if (config->mode == TIMER_MODE_CTC) {
                /*---- Configure Timer2 CTC mode ----*/
                TCCR2 |= (1 << WGM21);
                OCR2 = (uint8)config->compare_value;
                TIMSK |= (1 << OCIE2); /*---- Enable CTC interrupt ----*/
            } else {
                TIMSK |= (1 << TOIE2); /*---- Enable overflow interrupt ----*/
            }
            break;
    }

    /*---- Set the configured prescaler ----*/
    Timer_setPrescaler(config->timer_id, config->prescaler);
}

/*---- Deinitialize timer and disable interrupts ----*/
void Timer_deInit(Timer_ID_Type timer_id) {
    switch (timer_id) {
        case TIMER0_ID:
            /*---- Disable Timer0 and clear callbacks ----*/
            TCCR0 = 0;
            TIMSK &= ~((1 << OCIE0) | (1 << TOIE0));
            Timer0_Callback_CTC = NULL_PTR;
            Timer0_Callback_OVF = NULL_PTR;
            break;
        case TIMER1_ID:
            /*---- Disable Timer1 and clear callbacks ----*/
            TCCR1A = 0;
            TCCR1B = 0;
            TIMSK &= ~((1 << OCIE1A) | (1 << TOIE1));
            Timer1_Callback_CTC = NULL_PTR;
            Timer1_Callback_OVF = NULL_PTR;
            break;
        case TIMER2_ID:
            /*---- Disable Timer2 and clear callbacks ----*/
            TCCR2 = 0;
            TIMSK &= ~((1 << OCIE2) | (1 << TOIE2));
            Timer2_Callback_CTC = NULL_PTR;
            Timer2_Callback_OVF = NULL_PTR;
            break;
    }
}

/*---- Set callback function for CTC interrupt ----*/
void Timer_setCallBack_CTC(void (*callback)(void), Timer_ID_Type timer_id) {
    switch (timer_id) {
        case TIMER0_ID: Timer0_Callback_CTC = callback; break;
        case TIMER1_ID: Timer1_Callback_CTC = callback; break;
        case TIMER2_ID: Timer2_Callback_CTC = callback; break;
    }
}

/*---- Set callback function for overflow interrupt ----*/
void Timer_setCallBack_OVF(void (*callback)(void), Timer_ID_Type timer_id) {
    switch (timer_id) {
        case TIMER0_ID: Timer0_Callback_OVF = callback; break;
        case TIMER1_ID: Timer1_Callback_OVF = callback; break;
        case TIMER2_ID: Timer2_Callback_OVF = callback; break;
    }
}

/*******************************************************************************
 *                      Interrupt Service Routines                            *
 *******************************************************************************/

/*---- Timer0 Compare Match Interrupt ----*/
ISR(TIMER0_COMP_vect) {
    if (Timer0_Callback_CTC != NULL_PTR) Timer0_Callback_CTC();
}

/*---- Timer0 Overflow Interrupt ----*/
ISR(TIMER0_OVF_vect)  {
    if (Timer0_Callback_OVF != NULL_PTR) Timer0_Callback_OVF();
}

/*---- Timer1 Compare Match A Interrupt ----*/
ISR(TIMER1_COMPA_vect) {
    if (Timer1_Callback_CTC != NULL_PTR) Timer1_Callback_CTC();
}

/*---- Timer1 Overflow Interrupt ----*/
ISR(TIMER1_OVF_vect)   {
    if (Timer1_Callback_OVF != NULL_PTR) Timer1_Callback_OVF();
}

/*---- Timer2 Compare Match Interrupt ----*/
ISR(TIMER2_COMP_vect) {
    if (Timer2_Callback_CTC != NULL_PTR) Timer2_Callback_CTC();
}

/*---- Timer2 Overflow Interrupt ----*/
ISR(TIMER2_OVF_vect)  {
    if (Timer2_Callback_OVF != NULL_PTR) Timer2_Callback_OVF();
}
