Motor control testing form the Ali Express 24GP-2430 motor
My Model number is DC24V169RPM - 24v 169 Max RPM

The data on the Ali express site is somewhat lacking.  
The Pinouts for this motor are:

    Orange - Motor Direction.  Set HIGH for Clockwis (Facing output shaft)
    White - PWM Input.  The motor is most effecient when using a PWM frequency of 20KHz-30KHz
    Yellow - FG or Motor Shaft Encoder. This will provide 9 pulses per motor shaft revolution
    Red - Supply Voltage
    Black - Ground
 
The speed controll for this line of motors is inverted. 
A 0% duty cycle is full speed. 100% duty cycle stops the motor
For the TimerOne Library, 1023 is 100% duty cycle - full stop

PWM Notes:
The standard Arduino PWM freqs (500Hz, 1KHz) are not suitable for driving this motor. 
While these appear to work (the motor moves) the current draw is 3.5X greater then when
using the proper PWM frequencies.

You can achive higer frequencies by directly controlling the Arduino Timers. However,
The best solution I've found is to use the TimerOne.h library.  This library allows for PWM
Frequencies upto 1MHz. I am using 25KHz, which is in the middle of the recomended range for this motor.   

Motor Encoder Notes:
The yellow wire provides 9 pulses per motor revolution.  At top, specified, speed (100 RPS) there will be 900 pps.

Analog pin A0 is used to set the target speed. Use a pot for adjustable speed or, for full speed, tie this pin to +5v.
Other Pins are noted in the code.

