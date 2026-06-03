/****************************************************************************************************************************************************************************
Author: heretic    < Andrew G >
File: irarr_driver.c
Date Created: 1/5/2026
Dependencies: irarr_driver.h
Description: IR Array (5-sensor line detection) driver for line-following robot
             Returns weighted position of line (-100 to +100, or 1000 if lost)
Notes:
    +
History:
    + 1/5/2026 (Release 1.0)
    + 
****************************************************************************************************************************************************************************/

#include "irarr_driver.h"


#define F_CPU 16000000UL

#define IRARR_GROUP 'A'
#define IRARR_FIRSTPIN 3
#define IRARR_LASTPIN 7


void IRARR_init(void){
    GPIO_DefinePurpose_Mask(IRARR_GROUP, IRARR_FIRSTPIN, IRARR_LASTPIN + 1, IN);
}


ui8 IRARR_ReadingsArr[5];
si16 IRARR_read(void){
    si16 IRARR_error = 0;
    si16 IRARR_weightSum = 0;
    si16 IRARR_leftWeight = -2;
    // si16 IRARR_rightWeight = 2;
    si16 IRARR_Currentvalue = 0;
	for(ui8 i = IRARR_FIRSTPIN; i <= IRARR_LASTPIN; i++){
        IRARR_Currentvalue = !(GPIO_ReadDig(IRARR_GROUP,i));
        IRARR_ReadingsArr[i - IRARR_FIRSTPIN] = IRARR_Currentvalue;
        IRARR_error += (IRARR_Currentvalue * IRARR_leftWeight++);
        IRARR_weightSum += IRARR_Currentvalue;
	}
    if(IRARR_weightSum == 0) return IRARR_LOSTLINE;
	return (IRARR_error * 100) / IRARR_weightSum;
}


/*
============================  Func IRARR_read() Explanation  ============================
------------------------------------------------------------
IR Array Line Position Calculation (5 Sensors)

Sensors layout:
    S0   S1   S2   S3   S4
   -2   -1    0   +1   +2   (weights)

Each sensor returns:
    1 → sees line (black)
    0 → no line

------------------------------------------------------------
How it works:

1. Multiply each sensor reading by its weight:
   error += sensor * weight

2. Count how many sensors see the line:
   sum += sensor

3. Compute average position:
   position = error / sum

4. Scale by 100 to avoid floats:
   return (error * 100) / sum

------------------------------------------------------------
Examples:

Case 1:
    Sensors: 0 1 1 0 0
    Weights:-2 -1 0 1 2

    error = (-1 + 0) = -1
    sum = 2

    result = (-1 * 100) / 2 = -50
    → line is slightly LEFT

------------------------------------------------------------

Case 2:
    Sensors: 0 1 0 0 0

    error = -1
    sum = 1

    result = (-1 * 100) / 1 = -100
    → line is more LEFT

------------------------------------------------------------

Case 3:
    Sensors: 0 0 1 0 0

    error = 0
    sum = 1

    result = 0
    → line is CENTER

------------------------------------------------------------

Case 4:
    Sensors: 1 0 1 1 0

    error = (-2 + 0 + 1) = -1
    sum = 3

    result = (-1 * 100) / 3 ≈ -33
    → slightly LEFT (closer to center than Case 1)

------------------------------------------------------------

Case 5:
    Sensors: 0 0 0 0 0

    sum = 0 → no line detected

    return 1000
    → special value meaning "LOST LINE"

------------------------------------------------------------

Final meaning of output:
    negative → line is LEFT
    0        → line is CENTER
    positive → line is RIGHT
    1000     → line LOST

------------------------------------------------------------
*/
