/*********************************************************************************************************************************************************************************************************************************
Author: heretic    < Andrew G >
File: gpio_driver.h
Dependencies: portable_dts.h (for ui8 typedef)
Date Created: 18/9/2025
Description: GPIO Driver for ATmega32A (8-bit Registers).
             Provides Single- and Multi-Pin Operations for DDRx, PORTx, and PINx registers,
             including Write, Read, Toggle, and generic Set/Clear/Toggle Bit macros.
Notes:
    + Future support can be added for ATmega328P (Arduino Uno)
    + Multi-Pin operations for generic functions
    + 
History:
    + 18/9/2025: Driver Created (Release 1.0)
	+ 20/9/2025: Made Driver Powered By Functions NOT Definitions And Separated It Into .h And .c (Update 1.1)
	+ 27/9/2025: Enhanced SuperWrite to be more Versatile and minor bug fixes (Update 1.2)
	+ 27/1/2026: Put Naming Convention "GPIO_" & Made IO/HL always after the Px(or its range) in Multi Writes (Update 1.3) -- LATEST
	+
*********************************************************************************************************************************************************************************************************************************/


#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "portable_dts.h"

#define NULL (void *)0

// -----------------------------------
// Pin Direction Definitions (DDR)
// -----------------------------------
#define OUT 1
#define IN  0

// -----------------------------------
// Pin Value Definitions (PORT)
// -----------------------------------
#define HIGH 1
#define LOW 0

volatile ui8* determineGRP_ddr(ui8 GRP);
volatile ui8* determineGRP_port(ui8 GRP);
volatile ui8* determineGRP_pin(ui8 GRP);

// DDRx OPERATIONS
void GPIO_DefinePurpose(ui8 GRP, ui8 Px, ui8 IO);
void GPIO_DefinePurpose_Mask(ui8 GRP, ui8 PxS, ui8 PxF, ui8 IO);

// PORTx OPERATIONS
void GPIO_WriteDig(ui8 GRP, ui8 Px, ui8 HL);
void GPIO_WriteDig_Mask(ui8 GRP, ui8 PxS, ui8 PxF, ui8 HL);
void GPIO_ToggleDig(ui8 GRP, ui8 Px);
void GPIO_ToggleDig_Mask(ui8 GRP, ui8 PxS, ui8 PxF);
void GPIO_WriteWholeGRP(ui8 GRP, ui8 WholePORTReg);
void GPIO_SuperWrite(ui8 GRP, ui8 PxS, ui8 PxF, ui8 WholePORTReg);

// PINx OPERATIONS
ui8 GPIO_ReadDig(ui8 GRP, ui8 Px);
ui8 GPIO_ReadDig_Mask(ui8 GRP, ui8 PxS, ui8 PxF);

// GENERIC REGISTERS BIT MANIPULATION
void GPIO_SetBit(volatile ui8* REG, ui8 BitNo);
void GPIO_ClrBit(volatile ui8* REG, ui8 BitNo);
void GPIO_TogBit(volatile ui8* REG, ui8 BitNo);



// ==========================================================================================================
// DDRx / PORTx / PINx REGISTERS MAPPING
// ==========================================================================================================
 #define DDRA   (*(volatile ui8*)0x3A)
 #define PORTA  (*(volatile ui8*)0x3B)
 #define PINA   (*(volatile ui8*)0x39)

 #define DDRB   (*(volatile ui8*)0x37)
 #define PORTB  (*(volatile ui8*)0x38)
 #define PINB   (*(volatile ui8*)0x36)

 #define DDRC   (*(volatile ui8*)0x34)
 #define PORTC  (*(volatile ui8*)0x35)
 #define PINC   (*(volatile ui8*)0x33)

 #define DDRD   (*(volatile ui8*)0x31)
 #define PORTD  (*(volatile ui8*)0x32)
 #define PIND   (*(volatile ui8*)0x30)


#endif



