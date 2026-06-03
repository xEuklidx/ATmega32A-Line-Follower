
/****************************************************************************************************************************************************************************
Author: xEuklidx	< Andrew G >
File: timer1_driver.h
Date Created: 5/2/2026
Dependencies: "portable_dts.h"
Description: TOTAL functions for Timer1 (16-bit) on AVR microcontrollers, NO Input Capture
Notes:
    +
History:
    + 2/5/2026: (Release 1.0)
    + 
****************************************************************************************************************************************************************************/

#ifndef TIMER1_DRIVER_H
#define TIMER1_DRIVER_H

#include "portable_dts.h"
// #include <avr/io.h>

// ========== OC1A is on PD5 (Pin of timer1 output A) ==========
// ========== OC1B is on PD4 (Pin of timer1 output B) ==========




enum {  
        // << TCCR1A >> COM bits =>  7:6 (CTC output A) -- 5:4 (CTC output B) 
    T1_OUT_NONE = 0, 
    T1_OUT_TOGGLE = 1, 
    T1_OUT_LOW = 2, 
    T1_OUT_HIGH = 3
};

// << TCCR1A >> FOC bits =>  3 (Force OUT pin OC1A) --  2 (Force OUT OC1B) 

enum {
        // << TCCR1A >> 2 Lower WGM bits =>  1:0 (Choose Timer's Mode)
        // << TCCR1B >> 2 Higher WGM bits =>  4:3 (Choose Timer's Mode)
    T1_MODE_NORMAL = 0,
    T1_MODE_CTC_SINGLE,
    T1_MODE_CTC_DUAL,
    T1_MODE_FASTPWM_SINGLE,
    T1_MODE_FASTPWM_DUAL,
    T1_MODE_PHASECORRECT_SINGLE,
    T1_MODE_PHASECORRECT_DUAL
};

enum {
        // << TCCR1B >> CS bits =>  2:0 (Choose Timer's Prescaler)
    T1_PRE_STOP = 0,
    T1_PRE_1 = 1,
    T1_PRE_8 = 2,
    T1_PRE_64 = 3,
    T1_PRE_256 = 4,
    T1_PRE_1024 = 5,
    T1_PRE_EXT_FALLING = 6,
    T1_PRE_EXT_RISING = 7
};

// ======== FOR PWM ONLY ========
#define T1_PWM_PRE T1_PRE_8
#define T1_PWM_PRE_VAL 8
//===============================



// ========== Timer1 Driver Function Prototypes ==========

void T1_init(void);
void T1_setMode(ui8 mode);
void T1_setOutputAction_SINGLE(ui8 action);
void T1_setOutputAction_DUAL(ui8 action1, ui8 action2);

void T1_setTOP_SINGLE(ui16 top);
void T1_setTOP_DUAL(ui16 top);
void T1_setCompare_SINGLE(ui16 value);
void T1_setCompare_DUAL(ui16 value1, ui16 value2);

void T1_enableInterrupts(void);

void T1_start(ui8 prescaler);

void T1_doPWM_SINGLE(ui16 frequency, ui8 dutyCycle);
void T1_doPWM_DUAL(ui16 frequency, ui8 dutyCycleA, ui8 dutyCycleB);
void T1_setDutyCycle_SINGLE(ui8 dutyCycle);
void T1_setDutyCycle_DUAL(ui8 dutyCycleA, ui8 dutyCycleB);

void T1_stop(void);
void T1_doOutputAction_A(void);
void T1_doOutputAction_B(void);
ui16 T1_getCount(void);





// ========== Timer1 Register Bits Definitions ==========

// For the Modes selection
#define WGM10 0
#define WGM11 1
#define WGM12 3
#define WGM13 4

#endif
#define WGM13 4

// All Timer1 Registers Definitions
#define TCCR1A  (*(volatile ui8*)0x4F)
#define TCCR1B  (*(volatile ui8*)0x4E)
#define TCNT1   (*(volatile ui16*)0x4C)
#define OCR1A   (*(volatile ui16*)0x4A)
#define OCR1B   (*(volatile ui16*)0x48)
#define ICR1    (*(volatile ui16*)0x46)
#define TIMSK   (*(volatile ui8*)0x59)
#define TIFR    (*(volatile ui8*)0x58)



/*
*
*  Use Timer1 Fast PWM with ICR1 as TOP (mode 14)
*  PD5 = OC1A, PD4 = OC1B → both can output servo PWM when TOP = ICR1
*
*  TOP (ICR1) is calculated by:
*  ICR1 = F_CPU / (frequency * prescaler) - 1
*
*  For 50 Hz servo signal and prescaler = 8:
*  ICR1 = [16000000 / (50 * 8)] - 1 = 39999
*
*  Timer tick time:
*  tick = prescaler / F_CPU = 8 / 16000000 = 0.5 µs
*
*  IMPORTANT: Servo does NOT use duty cycle %.
*  Servo cares about pulse width in microseconds.
*
*  OCR value is calculated by:
*  OCR1x = pulse_time_us / tick_time_us
*
*  Examples:
*  1.0 ms  → 1000 / 0.5 = 2000   (≈ 0°)
*  1.5 ms  → 1500 / 0.5 = 3000   (≈ 90°)
*  2.0 ms  → 2000 / 0.5 = 4000   (≈ 180°)
*
*  Put these values in OCR1A or OCR1B to control the servo position.
*
*/
