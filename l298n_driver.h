

#include "gpio_driver.h"
#include "timer1_driver.h"

void L298N_init(void);
void L298N_moveForward(ui8 speed);
void L298N_moveBackward(ui8 speed);
void L298N_turnRight(ui8 speedR, ui8 speedL);
void L298N_turnLeft(ui8 speedR, ui8 speedL);
void L298N_stop(void);



/*
====================== L298N CONNECTIONS ======================

MCU: ATmega32A

-------------------- MOTOR DRIVER (L298N) ---------------------

L298N        →     ATmega32A
------------------------------------------
IN1          →     PB1   (Motor 1 Direction)
IN2          →     PB2   (Motor 1 Direction)

IN3          →     PB3   (Motor 2 Direction)
IN4          →     PB4   (Motor 2 Direction)

ENA          →     PD5   (OC1A - PWM Motor 1)
ENB          →     PD4   (OC1B - PWM Motor 2)

-------------------- POWER CONNECTIONS ------------------------

L298N VCC (12V)   →   External Motor Supply (e.g. 7V–12V)
L298N GND         →   MCU GND (COMMON GROUND REQUIRED)
L298N 5V          →   (Optional, depends on module jumper)

MCU VCC           →   5V
MCU GND           →   GND

-------------------- IMPORTANT NOTES --------------------------

1. REMOVE ENA/ENB JUMPERS on L298N if using PWM control.
2. PD5 and PD4 MUST be connected correctly for PWM to work.
3. ALWAYS use a common ground between MCU and L298N.
4. Motors may NOT start at very low duty cycles (<30%).
5. Ensure power supply can handle motor current.

==============================================================
*/