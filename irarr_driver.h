/****************************************************************************************************************************************************************************
Author: heretic    < Andrew G >
File: irarr_driver.h
Date Created: 1/5/2026
Dependencies: gpio_driver.h
Description: IR Array (5-sensor line detection) driver for line-following robot
             Returns weighted position of line (-100 to +100, or 1000 if lost)
Notes:
    +
History:
    + 1/5/2026 (Release 1.0)
    + 
****************************************************************************************************************************************************************************/


#include "gpio_driver.h"


#define IRARR_LOSTLINE 1000

void IRARR_init(void);
si16 IRARR_read(void);
extern ui8 IRARR_ReadingsArr[5];

