
/*********************************************************************************************************************************************************************************************************************************
Author: heretic	< Andrew G >
File: l289n_driver.c
Dependencies: l298n_driver.h & gpio_driver.h & timer1_driver.h
Date Created: 7/2/2026
Description: L298N Motor Driver Control - Supports speed control (0-100%) for two independent motors
Notes:
	+ Speed parameter accepts 0-100 (%)
	+ 0% = stopped, 100% = full speed
	+ Motor control pins configured on PORT B
	+ PWM frequency: 1000 Hz
History:
    + 7/2/2026: (Release 1.0)
	+ 
*********************************************************************************************************************************************************************************************************************************/

#include "l298n_driver.h"

/*
*
*  In "speed" parameter, you write the (%) value of the speed you want to set for the motors. For example, if you want to set the speed to 50%, you write 50 in the "speed" parameter.
*  The "speed" parameter can take values from 0 to 100, where 0 means the motors are stopped and 100 means the motors are running at full speed.
*  Where 0% is MIN speed (stop)
*  Where 100% is MAX speed 
*
*/

// For Both Motors
#define L298N_INSGROUP 'B'
#define L298N_PWMFREQ 1000
// For Motor < 1 >
#define L298N_IN1 1
#define L298N_IN2 2
// For Motor < 2 >
#define L298N_IN3 3
#define L298N_IN4 4

// PWM For Motor < 1 >  [OC1A - PD5]
#define L298N_PWM1_GROUP 'D'
#define L298N_PWM1_PIN 5

// PWM For Motor < 2 >  [OC1B - PD4]
#define L298N_PWM2_GROUP 'D'
#define L298N_PWM2_PIN 4




void L298N_init(void){
    // Setting up PWM pins
    GPIO_DefinePurpose(L298N_PWM1_GROUP, L298N_PWM1_PIN, OUT);
    GPIO_DefinePurpose(L298N_PWM2_GROUP, L298N_PWM2_PIN, OUT);
    // 1 kHz PWM, motors initially stopped [0% Speed]
    T1_doPWM_DUAL(L298N_PWMFREQ, 0, 0);
    // Making all of them output pins
    GPIO_DefinePurpose(L298N_INSGROUP, L298N_IN1, OUT);
    GPIO_DefinePurpose(L298N_INSGROUP, L298N_IN2, OUT);
    GPIO_DefinePurpose(L298N_INSGROUP, L298N_IN3, OUT);
    GPIO_DefinePurpose(L298N_INSGROUP, L298N_IN4, OUT);
}


void L298N_moveForward(ui8 speed){ //Speed for both motors
    if(speed > 100) speed = 100; // Speed Clamping
    // Motor < 1 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN1, HIGH);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN2, LOW);
    // Motor < 2 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN3, HIGH);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN4, LOW);
    // Speed (PWM using Timer1)
    T1_setDutyCycle_DUAL(speed, speed);
}


void L298N_moveBackward(ui8 speed){ //Speed for both motors
    if(speed > 100) speed = 100; // Speed Clamping
    // Motor < 1 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN1, LOW);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN2, HIGH);
    // Motor < 2 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN3, LOW);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN4, HIGH);
    // Speed (PWM using Timer1)
    T1_setDutyCycle_DUAL(speed, speed);
}


void L298N_turnRight(ui8 speedR, ui8 speedL){
    if(speedR > 100) speedR = 100; // Speed Clamping
    if(speedL > 100) speedL = 100; // Speed Clamping
    // Motor < 1 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN1, HIGH);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN2, LOW);
    // Motor < 2 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN3, LOW);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN4, HIGH);
    // Speed (PWM using Timer1)
    T1_setDutyCycle_DUAL(speedR, speedL);
    /* CAN SET THE SPEEDL TO 0 FOR MORE 90DEG RIGHT TURN */
}


void L298N_turnLeft(ui8 speedR, ui8 speedL){
    if(speedR > 100) speedR = 100; // Speed Clamping
    if(speedL > 100) speedL = 100; // Speed Clamping
    // Motor < 1 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN1, LOW);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN2, HIGH);
    // Motor < 2 > - direction
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN3, HIGH);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN4, LOW);
    // Speed (PWM using Timer1)
    T1_setDutyCycle_DUAL(speedR, speedL);
    /* CAN SET THE SPEEDR TO 0 FOR MORE 90DEG LEFT TURN */
}


void L298N_stop(void){
    T1_setDutyCycle_DUAL(0, 0);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN1, LOW);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN2, LOW);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN3, LOW);
    GPIO_WriteDig(L298N_INSGROUP, L298N_IN4, LOW);
}