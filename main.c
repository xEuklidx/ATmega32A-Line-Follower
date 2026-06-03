/*********************************************************************************************************************************************************************************************************************************
Author: heretic	< Andrew G >
File: main.c
Dependencies: l298n_driver.h & irarr_driver.h 
Date Created: 18/9/2025
Description: Main program entry point - Line follower robot control system
Notes:
	+ Implements PID control for line following (FUTURE UPDATE)
	+ Uses infrared array sensor for line detection
	+ Controls dual motors via L298N driver
	+ Adjusts speed based on sensor feedback
History:
    + 18/9/2025: (Release 1.0)
	+ 
*********************************************************************************************************************************************************************************************************************************/

#include "l298n_driver.h"
#include "irarr_driver.h"


int main(void)
{

    L298N_init();
    IRARR_init();

    int currentError  = 0;
    int lastError = 0;  // store last known line direction

    while(1)
    {
        currentError = IRARR_read();

        // ================= LOST LINE =================
        if(currentError == IRARR_LOSTLINE){
            while(IRARR_read() == IRARR_LOSTLINE)
            {
                // Spin toward where we last saw the line
                if(lastError <= 0){
                    L298N_turnLeft(0, 35);
                }
                else{
                    L298N_turnRight(35, 0);
                }
            }
        }

        // ================= CENTER =================
        else if(currentError <= 25 && currentError >= -25){
            lastError = currentError;
            L298N_moveForward(40);
        }

        // ================= SLIGHT LEFT =================
        else if(currentError < -25 && currentError >= -100){
            lastError = currentError;
            L298N_turnLeft(10, 55);
        }

        // ================= HARD LEFT =================
        else if(currentError < -100){
            lastError = currentError;
            L298N_turnLeft(0, 50);
        }

        // ================= SLIGHT RIGHT =================
        else if(currentError > 25 && currentError <= 100){
            lastError = currentError;
            L298N_turnRight(55, 10);
        }

        // ================= HARD RIGHT =================
        else if(currentError > 100){
            lastError = currentError;
            L298N_turnRight(50, 0);
        }
    }

    return 0;
}
