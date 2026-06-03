/*********************************************************************************************************************************************************************************************************************************
Author: heretic    < Andrew G >
File: gpio_driver.c
Dependencies: gpio_driver.h
Date Created:18/9/2025
Description: GPIO Driver for ATmega32A (8-bit Registers).
             Provides Single- and Multi-Pin Operations for DDRx, PORTx, and PINx registers,
             including Write, Read, Toggle, and generic Set/Clear/Toggle Bit macros.
Notes: 
    + Future support can be added for ATmega328P (Arduino Uno)
    + Multi-Pin operations for generic functions
    + 
History: 
    + 18/9/2025: Release 1.0
	+ 20/9/2025: Made Driver Powered By Functions NOT Definitions And Separated It Into .h And .c (Update 1.1)
	+ 27/9/2025: Enhanced SuperWrite to be more Versatile and minor bug fixes (Update 1.2)
	+ 27/1/2026: Put Naming Convention "GPIO_" & Made IO/HL always after the Px(or its range) in Multi Writes (Update 1.3) -- LATEST
	+ 
*********************************************************************************************************************************************************************************************************************************/

#include "gpio_driver.h"
//#include <avr/io.h>

volatile ui8* determineGRP_ddr(ui8 GRP) {
    switch (GRP) {
        case 'A': return &DDRA;
        case 'B': return &DDRB;
        case 'C': return &DDRC;
        case 'D': return &DDRD;
        default: return NULL;
    }
}

volatile ui8* determineGRP_port(ui8 GRP) {
    switch (GRP) {
        case 'A': return &PORTA;
        case 'B': return &PORTB;
        case 'C': return &PORTC;
        case 'D': return &PORTD;
        default: return NULL;
    }
}

volatile ui8* determineGRP_pin(ui8 GRP) {
    switch (GRP) {
        case 'A': return &PINA;
        case 'B': return &PINB;
        case 'C': return &PINC;
        case 'D': return &PIND;
        default: return NULL;
    }
}

void GPIO_DefinePurpose(ui8 GRP, ui8 Px, ui8 IO) {
    volatile ui8 *ddr_reg = determineGRP_ddr(GRP);
    if (ddr_reg) {
        if (IO == OUT) {
            (*ddr_reg) |= (1 << Px);
        } else {
            (*ddr_reg) &= ~(1 << Px);
        }
    }
}

void GPIO_DefinePurpose_Mask(ui8 GRP, ui8 PxS, ui8 PxF, ui8 IO) {
    volatile ui8 *ddr_reg = determineGRP_ddr(GRP);
    if (ddr_reg) {
        ui8 MASK = ((1 << (PxF - PxS + 1)) - 1) << PxS;
        if (IO == IN) {
            (*ddr_reg) &= ~MASK;
        } else {
            (*ddr_reg) |= MASK;
        }
    }
}

void GPIO_WriteDig(ui8 GRP, ui8 Px, ui8 HL) {
    volatile ui8 *port_reg = determineGRP_port(GRP);
    if (port_reg) {
        if (HL == HIGH) {
            (*port_reg) |= (1 << Px);
        } else {
            (*port_reg) &= ~(1 << Px);
        }
    }
}

void GPIO_WriteDig_Mask(ui8 GRP, ui8 PxS, ui8 PxF, ui8 HL) {
    volatile ui8 *port_reg = determineGRP_port(GRP);
    if (port_reg) {
        ui8 MASK = ((1 << (PxF - PxS + 1)) - 1) << PxS;
        if (HL == LOW) {
            (*port_reg) &= ~MASK;
        } else {
            (*port_reg) |= MASK;
        }
    }
}

void GPIO_ToggleDig(ui8 GRP, ui8 Px) {
    volatile ui8 *port_reg = determineGRP_port(GRP);
    if (port_reg) {
        (*port_reg) ^= (1 << Px);
    }
}

void GPIO_ToggleDig_Mask(ui8 GRP, ui8 PxS, ui8 PxF) {
    volatile ui8 *port_reg = determineGRP_port(GRP);
    if (port_reg) {
        (*port_reg) ^= ((1 << (PxF - PxS + 1)) - 1) << PxS;
    }
}

void GPIO_WriteWholeGRP(ui8 GRP, ui8 WholePORTReg) {
    volatile ui8 *port_reg = determineGRP_port(GRP);
    if (port_reg) {
        (*port_reg) = WholePORTReg;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO_SuperWrite(ui8 GRP, ui8 PxS, ui8 PxF, ui8 WholePORTReg) {
	volatile ui8 *port_reg = determineGRP_port(GRP);
	if (port_reg) {
		ui8 MASK_CLEAR = ~(((1 << (PxF - PxS + 1)) - 1) << PxS);
		ui8 MASK_WRITE = (WholePORTReg & ((1 << (PxF - PxS + 1)) - 1)) << PxS;
		(*port_reg) &= MASK_CLEAR;
		(*port_reg) |= MASK_WRITE;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

ui8 GPIO_ReadDig(ui8 GRP, ui8 Px) {
    volatile ui8 *pin_reg = determineGRP_pin(GRP);
    if (pin_reg) {
        return ((*pin_reg) >> Px) & 1;
    }
    return 0; // Return 0 on error
}

ui8 GPIO_ReadDig_Mask(ui8 GRP, ui8 PxS, ui8 PxF) {
    volatile ui8 *pin_reg = determineGRP_pin(GRP);
    if (pin_reg) {
        return (((*pin_reg) >> PxS) & ((1 << (PxF - PxS + 1)) - 1));
    }
    return 0; // Return 0 on error
}


// GENERIC FOR OTHER REGISTERS

void GPIO_SetBit(volatile ui8* REG, ui8 BitNo) {
    (*REG) |= (1 << BitNo);
}

void GPIO_ClrBit(volatile ui8* REG, ui8 BitNo) {
    (*REG) &= ~(1 << BitNo);
}

void GPIO_TogBit(volatile ui8* REG, ui8 BitNo) {
    (*REG) ^= (1 << BitNo);
}


