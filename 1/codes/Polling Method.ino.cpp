#include <Arduino.h>
#line 1 "F:\\Arduino Sketchbook\\sketch_oct10a\\sketch_oct10a.ino"
#line 1 "F:\\Arduino Sketchbook\\sketch_oct10a\\sketch_oct10a.ino"
void setup();
#line 19 "F:\\Arduino Sketchbook\\sketch_oct10a\\sketch_oct10a.ino"
void loop();
#line 30 "F:\\Arduino Sketchbook\\sketch_oct10a\\sketch_oct10a.ino"
void stepLEDs();
#line 38 "F:\\Arduino Sketchbook\\sketch_oct10a\\sketch_oct10a.ino"
void blinkLEDs();
#line 50 "F:\\Arduino Sketchbook\\sketch_oct10a\\sketch_oct10a.ino"
void goOff();
#line 1 "F:\\Arduino Sketchbook\\sketch_oct10a\\sketch_oct10a.ino"
void setup() {
  // put your setup code here, to run once:

  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);
  
  // Pins 0 to 6 are used as output pins
  for (int i = 0; i <= 6; ++i) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  // Pins 18 to 20 are used as input ones
  for (int i = 18; i <= 20; ++i) {
    pinMode(i, INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(18) == LOW)
    stepLEDs();
  else if (digitalRead(19) == LOW)
    blinkLEDs();
  else if (digitalRead(20) == LOW)
    goOff();
}

// Function stepLEDs turns LEDs on from left to right one by one
void stepLEDs() {
  for (int i = 0; i <= 6; ++i) {
    digitalWrite(i, HIGH);
    delay(500);
  }
}

// Function blinkLEDs blinks them LEDs 3 times, then keeps them on
void blinkLEDs() {
  byte state = LOW;
  for (int i = 0; i <= strlen("Amir") * 2 + 1; ++i) {
    for (int i = 0; i <= 6; ++i) {
      digitalWrite(i, state);
    }
    state = !state;
    delay(500);
  }
}

// Function goOff turns all the LEDs off at once
void goOff() {
  for (int i = 0; i <= 6; ++i) {
    digitalWrite(i, LOW);
  }
}

