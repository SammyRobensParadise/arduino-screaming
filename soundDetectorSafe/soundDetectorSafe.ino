//Header Libraries
//Libraries
//  LCD library
#include <LiquidCrystal.h>
//  Servo motor library
#include <Servo.h>
//-----------------------------
//int declaration for LCD
const int rs = 6, en = 7, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float LCDtime;
// Servo motor declaration
int servoPin = 5;
Servo servo;
int servoAngle = 0;
// Program Pin Declaration
//  Sound input pin, active LOW
const int DetectorPin = 2;
//  Unlocked Indicator Pin
const int setComboSelectPin = 3;
const int unlockPin = 4;
//  Reset Arduino after 30s pin
const int resetPin = 12;
//  Combination pins
//DEFAULT CODE: 4s,10s,14s
//If the arduino is reset, then the default code will be triggered at code1 = 4s,code2 = 10s, code3 = 14s 
int code1 = 4;
int code2 = 10;
int code3  = 14;
// allowed error in seconds from combination values
int allowedError = 1.5;
// Lock states
//  state 1
volatile byte state1 = LOW;
//  state 2
volatile byte state2 = LOW;
//  state 3
volatile byte state3 = LOW;
// bool to store the state of the safe, whether it is locked or unlocked
bool unlockedState = false;
// stores the current time
float currentTime;
// detects whether a sound was heard
bool hearSound;
// state for allowing access to the safe
bool allowAccess = false;
//Combination Declaration Valriables
volatile int setComboIndicator = 0;
volatile float buttonHoldDownTime;
volatile byte comboState = LOW;
int chooseTime = 0;
int NumofCodesSelected = 1;
bool addingState = false;
const int selectComboPin = 13;
volatile unsigned int edgeCounter = 0;
//----------------------------------------------------------------
void setup() {
  cli();
  //Timer code---------
  TCCR1A = 0; // Reset control registers timer1 /not needed, safety
  TCCR1B = 0; // Reset control registers timer1 // not needed, safety
  TIMSK2 |= _BV(TOIE1);
  //------------------
  sei();
  digitalWrite(resetPin, HIGH);
  currentTime = millis();
  pinMode(DetectorPin, INPUT);
  pinMode(setComboSelectPin, INPUT);
  pinMode(unlockPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(selectComboPin, INPUT);
  pinMode(A1,INPUT);
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(90);
  lcd.begin(16, 2);
  attachInterrupt(digitalPinToInterrupt(DetectorPin), sound, LOW);
  attachInterrupt(digitalPinToInterrupt(setComboSelectPin), setComboDetect, CHANGE);
}
//---------------------------------------------------------------
void loop() {
  Serial.println(digitalRead(A1));
  lcd.setCursor(0, 1);
  digitalRead(DetectorPin);
  lcd.print("LKD, Timer: ");
  lcd.print((float)edgeCounter / 1000);
  if (state1 && state2 && state3) {
    lcd.clear();
    delay(20);
    lcd.print("UNLOCKED ");
    digitalWrite(unlockPin, HIGH);
    unlockedState = true;
    servo.write(0);
  };

  /* Serial.print("state 1: ");
    Serial.println(state1);
    Serial.print("state 2: ");
    Serial.println(state2);
    Serial.print("state 3: ");
    Serial.println(state3);*/
  /*if (((float)millis() / 1000) >= 30 && !unlockedState) {
    digitalWrite(resetPin, LOW);
    }*/
  while (comboState == HIGH) {
   if(digitalRead(A1)==LOW){
    comboState = LOW;
      lcd.clear();
      lcd.print("Code Set");
      attachInterrupt(digitalPinToInterrupt(setComboSelectPin),setComboDetect, CHANGE);
      lcd.setCursor(0,1);
      lcd.clear();
      //reseting value for next combo selection
      setComboIndicator = 0;
      //requires the button to be pressed for 200+ milliseconds
      delay(200);
      } 
    lcd.clear();
    setCombo();
  /*  Serial.print("code1 ");
    Serial.println(code1);
    Serial.print("code2 ");
    Serial.println(code2);
    Serial.print("code3 ");
    Serial.println(code3);
    Serial.print("NumofCodesSelected: ");
    Serial.println(NumofCodesSelected); */
  }
  Serial.println("inLoop");
  Serial.println(edgeCounter);
  delay(500);

}

ISR(TIMER2_OVF_vect) {
  TCNT2 = 250;
  edgeCounter = edgeCounter + 1;
  if (edgeCounter > 30050) {
    edgeCounter = 0;
  }
}
void sound() {
 // Serial.println("SOUND DETECTED");
  currentTime = (float)edgeCounter / 1000;

  if (!state1 && !state2 && !state3 &&  code1 - allowedError <= currentTime && currentTime <= code1 + allowedError) {
    state1 = !state1;
    //  Serial.println("detected state 1");
  } else if (state1 && !state2 && !state3 && code2 - allowedError <= currentTime && currentTime <= code2 + allowedError) {
    state2 = !state2;
    //    Serial.println("detected state 2");
  } else if (state1 && state2 && !state3 && code3 - allowedError <= currentTime && currentTime <= code3 + allowedError) {
    //   Serial.println("detected state 3");
    state3 = !state3;
  } else {
    //    Serial.println("nothing detected");
  }
}

void setComboDetect() {
  if (setComboIndicator == 0) {
    buttonHoldDownTime = _getMicros();
    setComboIndicator++;
  } else if (setComboIndicator == 1 && ((_getMicros() - (float)buttonHoldDownTime) >= 4.0)) {
    lcd.clear();
    lcd.print("Set Combination Mode Enabled");
    comboState = !comboState;
    detachInterrupt(digitalPinToInterrupt(setComboSelectPin));
  } else {
    setComboIndicator = 0;
  }
}
//Is called from the main loop on a state update that allows the user
//to select their combination.

void setCombo() {
 /* if (addingState == false) {
    lcd.print("TT:LB,CT:RB");
  } */
  if (digitalRead(3) == LOW) {
    if (chooseTime == 1) {
      addingState = true;
    }
    if (chooseTime >= 30) {
      chooseTime = 0;
    }
    chooseTime++;
    delay(150);
  }
  if (digitalRead(selectComboPin) == LOW) {
    if (NumofCodesSelected == 1) {
      code1  = chooseTime;
      NumofCodesSelected++;
    } else if (NumofCodesSelected == 2) {
      code2  = chooseTime;
      NumofCodesSelected++;
    } else if (NumofCodesSelected == 3) {
      code3  = chooseTime;
      NumofCodesSelected++;
    } else {
      NumofCodesSelected = 1;
    }
    delay(400);
  }
  if(comboState == HIGH){
  lcd.clear();
  lcd.print("Time: ");
  lcd.print(chooseTime);
  lcd.print("s");
  if (NumofCodesSelected == 1) {
    lcd.print("  set A");
  }
  else if (NumofCodesSelected == 2) {
    lcd.print("  set B");
  } else if (NumofCodesSelected == 3) {
    lcd.print("  set C");
  }
  lcd.setCursor(0, 2);
  lcd.print("A:");
  lcd.print(code1);
  lcd.print(" ");
  lcd.print("B:");
  lcd.print(code2);
  lcd.print(" ");
  lcd.print("C:");
  lcd.print(code3);
  delay(100);
  }
}
//returns a value in seconds from micros TCCR0
long _getMicros() {
  return (float)micros() / 1000000.0;
}
