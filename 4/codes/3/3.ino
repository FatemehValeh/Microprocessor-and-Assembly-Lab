#include <Servo.h>

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);
  Serial.begin(9600);
  Serial.setTimeout(100000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    int entry = Serial.parseInt();
    Serial.println(entry);
    myServo.write(-1 * entry + 90);
  }
}
