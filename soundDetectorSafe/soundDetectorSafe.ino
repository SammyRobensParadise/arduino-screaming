const int DetectorPin = 2;
//current Pin: 4s,10s,14s
//const float expectedTime1 = 4;
//const float expectedTime2 = 10;
//const float expectedTime3 = 14;
float currentTime;
volatile int bypassNumber = 0;
bool hearSound;
bool allowAccess = false;
void setup() {
  currentTime = millis();
  // put your setup code here, to run once:
  pinMode(DetectorPin, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(DetectorPin), sound, LOW);
}

void loop() {
  digitalRead(DetectorPin);
  Serial.println((float)millis() / 1000);
  Serial.print("currentBypass Number ");
  Serial.println(bypassNumber);
  delay(500);
}
void sound() {
  Serial.println("SOUND DETECTED");
  currentTime = (int)micros() / 1000;
  if (bypassNumber == 0) {
    if ( 3.5 <= currentTime && currentTime <= 9) {
      bypassNumber= bypassNumber+1;
      Serial.println("HITTING IF");
      Serial.println(bypassNumber);
    }
    if (bypassNumber == 1) {
      if (9.5 <= currentTime && currentTime <= 10.5) {
        bypassNumber= bypassNumber+1;
              Serial.println("HITTING IF 2");
      Serial.println(bypassNumber);
      } else {
        bypassNumber = 0;
      }
    }
    if (bypassNumber == 2) {
      if (13.5 <= currentTime && currentTime <= 14.5) {
        allowAccess = true;
      } else {
        bypassNumber = 0;
      }
    }
  }
}
