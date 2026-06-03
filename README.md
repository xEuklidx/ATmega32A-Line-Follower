# ATmega32A-Line-Follower
Bare-metal C firmware for an autonomous line-following robot using the ATmega32A microcontroller. Implements a modular, layered architecture with zero external libraries and direct register manipulation.

# COMPONENTS 

-> 5 TCT IR sensors array module

-> L298N Motor Driver

-> 2 18650 Li-Ion Batteries

-> 5A Buck Converter (7.4v to 5v)

-> 16Mhz Crystal Oscillator with accompanying 2x 22pF caps

-> HC-SR02 sensor holder (to secure the IR array)

-> Jumpers & Breadboard

-> 2 Wheeled Chassis with caster wheel

-> Cap on 5v rail to protect ATmega32A from voltage spikes

-> A slice of PCB with terminal blocks to act as the 7.4v rail


# CODE FILES 

TOP (LOGIC ALGO) LAYER:      main.c

MIDDLE (COMPONENT) LAYER:   l298n_driver.h/.c  &  irarr_driver.h/.c

LOWER (BARE-METAL) LAYER:   timer1_driver.h/.c &  gpio_driver.h/.c


# LOGIC 

#### 1. Fixed-Point Weighted Sensor Mapping
To eliminate the computational overhead of floating-point division on an 8-bit AVR core, the tracking math is optimized using scaled integer values (shifting calculations into the hundreds):
* **Far Left Sensor:** `-200`
* **Mid Left Sensor:** `-100`
* **Center Sensor:** `0`
* **Mid Right Sensor:** `100`
* **Far Right Sensor:** `200`
The error vector is calculated by summing the weights of all active detectors and dividing by the number of sensors currently detecting the line. Scaling the values prevents truncation errors during integer division since the ATmega32A lacks a dedicated hardware floating-point unit (FPU).

#### 2. Multi-Tiered Differential Drive Steering
The robot runs an adapted Bang-Bang control scheme featuring discrete correction tiers based on the calculated error magnitude:
* **Low Error (Minor Drift):** Triggers a low-differential speed offset between the motors for smooth, sweeping path corrections along gradual curves.
* **High Error (Sharp Turn):** Triggers a high-differential or counter-rotational drive offset to aggressively pivot the chassis around sudden, jagged geometric angles.

#### 3. State-Latch Recovery (Line Loss Handling)
When the robot completely loses the line (yielding a distinct error code of `1000`), it doesn't halt or drift blindly. The firmware utilizes a memory-latch state that preserves the last known valid tracking direction. It maintains a sharp recovery turn in that direction until the sensor array re-acquires the tape line, allowing the system to seamlessly survive aggressive track gaps or severe overshoots.




*(Note: The driver design pre-allocates sensory weight tracking to allow an inline drop-in migration to a fully tuned Proportional-Integral-Derivative (PID) control loop in future iterations).*

=====================================================================

Done By: Andrew H. Ghobrial <Heretic>

MY LINKEDIN: www.linkedin.com/in/andrew-ghobrial-769974349


