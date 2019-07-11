## arduino-screaming
This a program that is written as part of a school project. It uses a sound sensor to detect when someone screams at certain points in time and then will open a lock box if the sequence is correct.

# schematic breakdown
1. Sound Detector Pin `int DetectorPin` : 2
* Sound Sensor is based on ACTIVE LOW tech, and is set  to low whever it detects a sound.
3. Servo Pin `int servoPin`: 9
* This pin is responsable for determining the angle to set the servo motor to when the correct sequence is entered or when
the arduino is reset via the manual reset button or `resetPin`;

# How the code works:
There are three states: `volatile byte state1, volatile byte state2,volatile byte state3`. Each state corresponds
to a specific condition that is held during the <b>ISR</b>. All three states must be set to `HIGH` or `true` in order for the
conditional statement that triggers the servo motor to rotate to be executed.

# How to set a  combination:
For debugging purposes, currently the ranges where a sound may be detected are hard-coded into the conditional statements of
`void sound()` which is the interrupt that is triggered whenever there is a sound detected. it is a rough error bound
to ensure that a sound can be detected in the timeframe. the inteerrupt `void sound()` uses  the `micros()` function that returns
the time since start in microseconds (seperate from `millis()`) which can be accessed in interrupts. Note that `micros()` will overflow
after about 70 minutes, however because the timer is reset after 30s, we do not care about this in the design. However it is important to note that this strategy does not scale to timeframes larger then about an hour or use. 

<b>Note:</b>
The next phase in the project is to ammend the conditional logic in this phase to make it more robist and  easier to manipulate.

# Hardware Requirements:
1. Arduino UNO
2. WaveShare Sound sensor (https://www.waveshare.com/sound-sensor.htm)
3. 2x Tower Pro Mirco Servo 9g (SG90)
3. LED light as indicator (optional)
4. Jumper wires
5. Bread board
6. 4x standard breadboard buttons
7. Hardware required to construct the physical safe
8. 9V battery if the intent is to use the safe for a specific level  of functionality

# Software Dependencies:
1. Servo library available under: Sketch/Include Libraries/servo.h
* add `#include <servo.h>` to the top of your code.
2. Liquid Crystal librarby available under: Sketch/Include Libraries/LiquidCrystal.h
* add `#include <LiquidCrystal.h>` to the top of your code.

# Additional Notes:

contact info: srobensp@edu.uwaterloo.ca