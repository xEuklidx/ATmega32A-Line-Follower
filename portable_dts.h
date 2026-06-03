/****************************************************************************************************************************************************************************
Author: heretic    < Andrew G >
File: portable_dts.h
Date Created: 9/18/2025
Dependencies: <stdint.h>
Description: Standard portable datatypes for embedded C (AVR/8-bit): CONFIGURE FIRST
Notes:
    +
History:
    + 9/18/2025 (Release 1.0)
    + 
****************************************************************************************************************************************************************************/


#ifndef PORTABLE_DTS_H
#define PORTABLE_DTS_H

#include <stdint.h>  // Standard C99 integer types

// Unsigned integers
typedef uint8_t  ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

// Signed integers
typedef int8_t   si8;
typedef int16_t  si16;
typedef int32_t  si32;
typedef int64_t  si64;

// 16-bit fixed point: 8 bits integer, 8 bits fraction
typedef ui16  uq8;  	// Unsigned
typedef si16  sq8;   	// Signed

// 32-bit fixed point: 16 bits integer, 16 bits fraction
typedef ui32  uq16;   	// Unsigned
typedef si32  sq16;   	// Signed

#endif
