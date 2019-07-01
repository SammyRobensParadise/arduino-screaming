#include <Servo.h>
int servoPin = 9;
Servo servo;  
int servoAngle = 0;
const int DetectorPin = 2;
const int unlockPin = 10;
const int resetPin = 4;
//current Pin: 4s,10s,14s
volatile byte state1 = LOW;
volatile byte state2 = LOW;
volatile byte state3 = LOW;
bool unlockedState = false;
float currentTime;
bool hearSound;
bool allowAccess = false;
void setup() {
  digitalWrite(resetPin, HIGH);
  currentTime = millis();
  // put your setup code here, to run once:
  pinMode(DetectorPin, INPUT);
  pinMode(unlockPin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(90); 
  attachInterrupt(digitalPinToInterrupt(DetectorPin), sound, LOW);
}

void loop() {
  digitalRead(DetectorPin);
  Serial.println((float)micros() / 1000000);
 /* if (state1) {
    Serial.println("true state 1");
    Serial.println("");
  }
  if (state2) {
    Serial.println("true state 2");
    Serial.println("");
  }
  if (state3) {
    Serial.println("true state 3");
    Serial.println("");
  }*/
  if (state1 && state2 && state3) {
    Serial.println("UNLOCKED");
    digitalWrite(unlockPin, HIGH);
    unlockedState = true;
    servo.write(0); 
  };

  Serial.print("state 1: ");
  Serial.println(state1);
  Serial.print("state 2: ");
  Serial.println(state2);
  Serial.print("state 3: ");
  Serial.println(state3);
  
  if (((float)millis() / 1000) >= 30 && !unlockedState) {
    digitalWrite(resetPin, LOW);
  }
  delay(500);
}
void sound() {
  Serial.println("SOUND DETECTED");
  currentTime = (float)micros() / 1000000;
  
  if (!state1 && !state2 && !state3 &&  3.5 <= currentTime && currentTime <= 5) {
    state1 = HIGH;
    Serial.println("HITTING IF 1");
  }else if (state1 && !state2 && !state3 && 9.5 <= currentTime && currentTime <= 11) {
    state2 = HIGH;
    Serial.println("HITTING IF 2");
  }else if (state1 && state2 && !state3 && 13.5 <= currentTime && currentTime <= 14.5) {
    Serial.println("HITTING IF 3");
    state3 = !state3;
  } else {
    Serial.println("nothing detected");
  }
}
