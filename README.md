## arduino-screaming
This a program that is written as part of a school project. It uses a sound sensor to detect when someone screams at certain points in time and then will open a lock box if the sequence is correct.

<b>Created by Sammy Robens-Paradise, Systems Design Engineering 2023, the University of Waterloo</b>

# Schematic breakdown
1. Interrupt Sound Detector Pin `int DetectorPin` : 2
* Sound Sensor is based on ACTIVE LOW tech, and is set  to low whever it detects a sound.
2. Servo Pin `int servoPin`: 5
* This pin is responsable for determining the angle to set the servo motor to when the correct sequence is entered or when
3. Interrupt Pin detecting whether the system should enter `SetCombo` state: 3
4. Indicates that the box is in an unlocked state
* `unclockPin` : 3
5. `resetPin` is  an <b>optional</b> pin that if dragged to the reset pin on arduino uno
will cause the arduino to reset after 30 seconds.  Use caution if using the pin, since it will cause the arduino to undergo a full reset and will also reset the `code1,code2,code3` values that maintain the current comination values
6. Analog pin A1 is used for detecting the flag that will exit the `setCombo` state
7. Analog pin A2 is used for locking  the safe (triggering the servo to turn back to 0 degrees)

# How the code works:
There are three states: `volatile byte state1, volatile byte state2,volatile byte state3`. Each state corresponds
to a specific condition that is held during the <b>ISR</b>. All three states must be set to `HIGH` or `true` in order for the conditional statement that triggers the servo motor to rotate to be executed, and `bool unlockedState` is set to `true`. press the red button to lock the safe again.
`int allowedError` is a value in seconds that is the error bound from the combo time that the system will accept as a valid trigger. for example, if there is a combo value at 4s (`code1 = 4;`) then code will accept a sound that falls between 2.5 seconds and 5.5 seconds. that value must be changed in the source code.



# Functions:
1. `void setup()`
* Standard arduino setup function, this is only run once after reset
2. `void loop()`
* Main loop of the arduino
3. `ISR(TIMER2_OVF_vect)`
* Overflow ISR interrupt that is  triggered whenever Timer 2 (TCNT2) overflows. In the interrupt it sets `TCNT2 = 250` causing the timer to overflow every milimsecond and increments the edge counter. if the edge counter is greater then 30,000, meaning that the Timer has overflowed 30 000 times, (30 seconds) then `edgeCounter` is reset to 0.
4. `void sound()`
* Interrrupt that is triggered whenever there is a sound detected. It will check if the sound matches a specific state and if the time aligns with a `code` and  if it does, then it will update the state of the unlocking bytes.
5. `void setComboDetect()`
* Interrupt triggered that will interrupt the code into the `setCombo  state`.
6. `void setCombo()` 
* sets the new combo for the safe. Note that if the arduino is reset, these values will no be preserved and will be overwritten to the default (4s,10s,14s).
7. `long _getMicros()`
* returns the value of `micros()` from TCNT0 in seconds as a `long`.


# How to set a combination:
1. Hold down the set combination button for more than 4 seconds and release
2. The LCD should now display that you are in a state where you can set the combination
3. click the set combination button button and observe that the timer increments, when you would like to set the value, press the <b>set</b> button, this will store the value of that combination
4. Repeat for each value, (A,B,C)
5. when you are done, select the <b>done</b> button. This will exit the set combination


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
* note that LiquidCrystal.h requires access to TCCR1B, and TCCR1A, thus any manipulation that is done for prescaling must be done with extreme care. 

# Additional Notes:

contact info: srobensp@edu.uwaterloo.ca