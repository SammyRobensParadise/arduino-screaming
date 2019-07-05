//Header Libraries
//  LCD library
#include <LiquidCrystal.h>
//  Servo motor library
#include <Servo.h>
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
//current code: 4s,10s,14s
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
long counter_freq = 16000000/30000000;
//----------------------------------------------------------------
void setup() {
  digitalWrite(resetPin, HIGH);
  currentTime = millis();
  pinMode(DetectorPin, INPUT);
  pinMode(setComboSelectPin, INPUT);
  pinMode(unlockPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  pinMode(selectComboPin, INPUT);
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(90);
  lcd.begin(16, 2);
  attachInterrupt(digitalPinToInterrupt(DetectorPin), sound, LOW);
  attachInterrupt(digitalPinToInterrupt(setComboSelectPin), setComboDetect, CHANGE);
}
//---------------------------------------------------------------
void loop() {
  lcd.setCursor(0, 1);
  digitalRead(DetectorPin);
  lcd.print("LKD, Timer: ");
  if (state1 && state2 && state3) {
    lcd.clear();
    delay(20);
    lcd.print("UNLOCKED ");
    digitalWrite(unlockPin, HIGH);
    unlockedState = true;
    servo.write(0);
  };
  if(_getMicros()<= 30){
    LCDtime = _getMicros();
    lcd.print(LCDtime);
    }else if(_getMicros()> 30){
     LCDtime = _getMicros();
    lcd.print(LCDtime);
    }
    Serial.println(_getMicros());
  /* Serial.print("state 1: ");
    Serial.println(state1);
    Serial.print("state 2: ");
    Serial.println(state2);
    Serial.print("state 3: ");
    Serial.println(state3);*/
  if (((float)millis() / 1000) >= 30 && !unlockedState) {
    digitalWrite(resetPin, LOW);
  }
  while (comboState == HIGH) {
    lcd.clear();
    setCombo();
    Serial.print("code1 ");
    Serial.println(code1);
    Serial.print("code2 ");
    Serial.println(code2);
    Serial.print("code3 ");
    Serial.println(code3);
    Serial.print("NumofCodesSelected: ");
    Serial.println(NumofCodesSelected);
  }
  delay(500);
}
void sound() {
  Serial.println("SOUND DETECTED");
  currentTime = (float)micros() / 1000000;

  if (!state1 && !state2 && !state3 &&  code1 - allowedError <= currentTime && currentTime <= code1 + allowedError) {
    state1 = !state1;
    Serial.println("detected state 1");
  } else if (state1 && !state2 && !state3 && code2 - allowedError <= currentTime && currentTime <= code2 + allowedError) {
    state2 = !state2;
    Serial.println("detected state 2");
  } else if (state1 && state2 && !state3 && code3 - allowedError <= currentTime && currentTime <= code3 + allowedError) {
    Serial.println("detected state 3");
    state3 = !state3;
  } else {
    Serial.println("nothing detected");
  }
}

void setComboDetect() {
  Serial.println("setCombotriggered");
  if (setComboIndicator == 0) {
    buttonHoldDownTime = (float)micros() / 1000000.0;
    setComboIndicator++;
  } else if (setComboIndicator == 1 && ((((float)micros() / 1000000.0) - (float)buttonHoldDownTime) >= 4.0)) {
    lcd.clear();
    lcd.print("Set Combination Mode Enabled");
    comboState = !comboState;
    detachInterrupt(digitalPinToInterrupt(setComboSelectPin));
  } else {
    Serial.println("none triggered");
    setComboIndicator = 0;
  }
}
void setCombo() {
  lcd.setCursor(1, 0);
  if (addingState == false) {
    lcd.print("TT:LB,CT:RB");
  }
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
  lcd.clear();
  lcd.print("Time: ");
  lcd.print(chooseTime);
  lcd.print("s");
  delay(100);
}
long _getMicros(){
  return (float)micros() / 1000000.0;
  }
