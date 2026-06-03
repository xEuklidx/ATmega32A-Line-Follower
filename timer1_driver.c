
/****************************************************************************************************************************************************************************
Author: xEuklidx	< Andrew G >
File: timer1_driver.c
Date Created: 5/2/2026
Dependencies: "timer1_driver.h"
Description: TOTAL functions for Timer1 (16-bit) on AVR microcontrollers, NO Input Capture
Notes:
    +
History:
    + 2/5/2026: (Release 1.0)
    + 
****************************************************************************************************************************************************************************/


#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "timer1_driver.h"

// ========== OC1A is on PD5 (Pin of timer1 output A) ==========
// ========== OC1B is on PD4 (Pin of timer1 output B) ==========

ui8 currentMode;

ui16 top = 0;




void T1_init(void) {                        // Step << 1 >>
    // Initialize Timer1 with default settings
    TCCR1A = 0x00;       // Disable Timer1 & set to normal mode
    TCCR1B = 0x00;       // No prescaler 
    TCNT1 = 0x0000;        // Reset counter
    OCR1A = 0xFFFF;        // Set compare value to maximum
    OCR1B = 0xFFFF;        // Set compare value to maximum for channel B
    TIMSK &= ~0xC3;      // Disable all Timer1 interrupts   - ONLY
    TIFR |= 0xC3;        // Clear any pending Timer1 flags  - ONLY
    currentMode = T1_MODE_NORMAL;
}




void T1_setMode(ui8 mode) {
    // Clear all WGM bits first
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B &= ~((1 << WGM12) | (1 << WGM13));
    
        // WGM13 - WGM12 - WGM11 - WGM10
        // Bit 4   Bit 3   Bit 1   Bit 0
        //     TCCR1B          TCCR1A

    // Set the desired mode
    switch(mode) {
        case T1_MODE_NORMAL:
            // WGM13:0 = 0000 (already cleared)
            break;
            
        case T1_MODE_CTC_SINGLE:  // Mode 4
            // WGM13:0 = 0100
            TCCR1B |= (1 << WGM12);
            break;
            
        case T1_MODE_CTC_DUAL:   // Mode 12
            // WGM13:0 = 1100
            TCCR1B |= (1 << WGM12) | (1 << WGM13);
            break;
       
        case T1_MODE_FASTPWM_SINGLE:  // Mode 15
            // WGM13:0 = 1111
            TCCR1A |= (1 << WGM10) | (1 << WGM11);
            TCCR1B |= (1 << WGM12) | (1 << WGM13);
            break;

        case T1_MODE_FASTPWM_DUAL:  // Mode 14
            // WGM13:0 = 1110
            TCCR1A |= (1 << WGM11);
            TCCR1B |= (1 << WGM12) | (1 << WGM13);
            break;

        case T1_MODE_PHASECORRECT_SINGLE:  // Mode 10
            // WGM13:0 = 1010
            TCCR1A |= (1 << WGM11);  
            TCCR1B |= (1 << WGM13);
            break;

        case T1_MODE_PHASECORRECT_DUAL:  //  be Mode 8
            // WGM13:0 = 1000
            TCCR1B |= (1 << WGM13);
            break;
    }
    
    currentMode = mode;
}




void T1_setOutputAction_SINGLE(ui8 action) {
    // For channel B : OCR1B reached : PD4 (OC1B pin)
    // OCR1A is not used in SINGLE compare mode AS it holds the TOP value
    // ICR1 is not used

    TCCR1A &= ~(3 << 4);     // Clear COM1B bits first (Bits 5:4)
    TCCR1A |= (action << 4);  // Channel B (bits 5:4)
}

void T1_setOutputAction_DUAL(ui8 action1, ui8 action2) {
    // For channel A : OCR1A reached : PD5 (OC1A pin)
    // For channel B : OCR1B reached : PD4 (OC1B pin)
    // ICR1 holds the TOP value instead of OCR1A in DUAL compare mode

    TCCR1A &= ~((3 << 6) | (3 << 4));  // Clear COM1A and COM1B bits first
    TCCR1A |= (action1 << 6);   // Channel A (bits 7:6)
    TCCR1A |= (action2 << 4);   // Channel B (bits 5:4)
}




// ========================================================     // Step << 4 >>

// For SINGLE modes - set the period (TOP)
void T1_setTOP_SINGLE(ui16 value) {
    OCR1A = value;  // OCR1A is the TOP
}

// For DUAL modes - set the period (TOP)
void T1_setTOP_DUAL(ui16 value) {
    ICR1 = value;   // ICR1 is the TOP
}




// ========================================================     // Step << 5 >>

// For SINGLE modes - set the compare value
void T1_setCompare_SINGLE(ui16 value) {
    OCR1B = value;  // OCR1B is your compare
}

// For DUAL modes - set compare values
void T1_setCompare_DUAL(ui16 valueA, ui16 valueB) {
    OCR1A = valueA; // Both are free
    OCR1B = valueB;
}




void T1_enableInterrupts(void) {                            // Step << 6 >>
    // Clear both Timer1 interrupt bits first
    TIMSK &= ~(0x0F << 2);  // Clear bits 2,3,4,5 in TIMSK 
    // and thus disable (TOV1, OCF1B, OCF1A, ICF1) flags in TIFR

    if(currentMode == T1_MODE_NORMAL) {
        // Enable Overflow Interrupt for Normal mode        
        TIMSK |= (1 << 2);  // enable TOV1 -- Bit 2 in TIFR
    } 
    else if(currentMode == T1_MODE_CTC_SINGLE) {
        // Enable Output Compare Match Interrupt for Single CTC mode
        TIMSK |= (1 << 4);  // enable OCF1A -- Bit 4 in TIFR ( MAIN )
        if(OCR1B != 0) {
            TIMSK |= (1 << 3);  // enable OCF1B -- Bit 3 in TIFR ( OPTIONAL )
        }
    } 
    else if(currentMode == T1_MODE_CTC_DUAL) {
        // Enable Output Compare Match Interrupts for Dual CTC mode
        TIMSK |= (1 << 4) | (1 << 3);  // enable OCF1A and OCF1B -- Bits 4 and 3 in TIFR
    }
    else {
        // No interrupts for PWM modes (Fast PWM and Phase Correct PWM)
    }

}




void T1_start(ui8 prescaler) {             // Step << 7 >> -- START
    // Set the prescaler for Timer0 
    // AND thus START the timer automatically
    TCCR1B = (TCCR1B & 0xF8) | prescaler;
}




void T1_doPWM_SINGLE(ui16 frequency, ui8 dutyCycle) {    // => QoL Feature (Simple PWM Control))
    T1_init();
    ui8 prescaler = T1_PWM_PRE_VAL;
    top = F_CPU / (frequency * prescaler) - 1;

    T1_setMode(T1_MODE_FASTPWM_SINGLE);
    
    // Set TOP (frequency)
    T1_setTOP_SINGLE(top);
    
    // Set duty cycle as percentage of TOP
    T1_setCompare_SINGLE((ui32)(dutyCycle * top) / 100);
    
    // Configure PWM output (non-inverting mode)
    T1_setOutputAction_SINGLE(T1_OUT_LOW);  // PWM on OC1B
    
    T1_start(T1_PWM_PRE);
}

void T1_doPWM_DUAL(ui16 frequency, ui8 dutyCycleA, ui8 dutyCycleB) {   // => QoL Feature (Simple PWM Control))
    T1_init();
    ui8 prescaler = T1_PWM_PRE_VAL;
    top = F_CPU / (frequency * prescaler) - 1;

    T1_setMode(T1_MODE_FASTPWM_DUAL);
    
    // Set TOP (frequency)
    T1_setTOP_DUAL(top);
    
    // Set duty cycles as percentage of TOP
    T1_setCompare_DUAL(((ui32)dutyCycleA * top) / 100,  // PWM on OC1A : PD4
                       ((ui32)dutyCycleB * top) / 100); // PWM on OC1B : PD5
    
    // Configure PWM outputs (non-inverting mode)
    T1_setOutputAction_DUAL(T1_OUT_LOW, T1_OUT_LOW);
    
    T1_start(T1_PWM_PRE);
}




// // ================================================= USE THIS -- after using PWM setup function

void T1_setDutyCycle_SINGLE(ui8 dutyCycle) {
    // Set duty cycle as percentage of TOP
    T1_setCompare_SINGLE((ui32)(dutyCycle * top) / 100); // PWM on OC1B : PD4 
    T1_start(T1_PWM_PRE);  
}

void T1_setDutyCycle_DUAL(ui8 dutyCycleA, ui8 dutyCycleB) {
    // Set duty cycles as percentage of TOP
    T1_setCompare_DUAL(((ui32)dutyCycleA * top) / 100,  // PWM on OC1A : PD5
                       ((ui32)dutyCycleB * top) / 100); // PWM on OC1B : PD4
    T1_start(T1_PWM_PRE);
}

// // ================================================================================================




void T1_stop(void) {                         // -- Optional Step
    // Stop Timer1 by clearing the prescaler bits
    // AND the counting register is reset to 0
    TCCR1B &= 0xF8;
    TCNT1 = 0x0000;
}




void T1_doOutputAction_A(void) {               // -- Optional Step
    // Set the output action for Timer1 Channel A
    TCCR1A |= (1 << 3);
}

void T1_doOutputAction_B(void) {               // -- Optional Step
    // Set the output action for Timer1 Channel B
    TCCR1A |= (1 << 2);
}




ui16 T1_getCount(void) {                      // -- Optional Step
    // Get the current count value of Timer1
    return TCNT1;
}