#include <Arduino.h>
#line 1 "F:\\Arduino Sketchbook\\sketch_oct10b\\sketch_oct10b.ino"
int interruptType = 0;
int counter = 0;
volatile byte state = LOW;

#line 5 "F:\\Arduino Sketchbook\\sketch_oct10b\\sketch_oct10b.ino"
void setup();
#line 27 "F:\\Arduino Sketchbook\\sketch_oct10b\\sketch_oct10b.ino"
void loop();
#line 37 "F:\\Arduino Sketchbook\\sketch_oct10b\\sketch_oct10b.ino"
void stepLEDs();
#line 55 "F:\\Arduino Sketchbook\\sketch_oct10b\\sketch_oct10b.ino"
void blinkLEDs();
#line 73 "F:\\Arduino Sketchbook\\sketch_oct10b\\sketch_oct10b.ino"
void goOff();
#line 5 "F:\\Arduino Sketchbook\\sketch_oct10b\\sketch_oct10b.ino"
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
  
  attachInterrupt(digitalPinToInterrupt(18), stepLEDs, LOW);
  attachInterrupt(digitalPinToInterrupt(19), blinkLEDs, LOW);
  attachInterrupt(digitalPinToInterrupt(20), goOff, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (interruptType == 1)
    stepLEDs();
  else if (interruptType == 2)
    blinkLEDs();
  delay(500);
}

// Function stepLEDs turns LEDs on from left to right one by one
void stepLEDs() {
  if (interruptType != 1) {
    for (int i = 0; i <= 6; ++i) {
      digitalWrite(i, LOW);
    }
    interruptType = 1;
  }
  else if (counter <= 6) {
    digitalWrite(counter, HIGH);
    ++counter;
  }
  else {
    interruptType = 0;
    counter = 0;
  }
}

// Function blinkLEDs blinks them LEDs 3 times, then keeps them on
void blinkLEDs() {
  if (interruptType != 2) {
    state = LOW;
    interruptType = 2;
  }
  else if (counter <= strlen("Amir") * 2 + 1) {
    for (int i = 0; i <= 6; ++i)
      digitalWrite(i, state);
    state = !state;
    ++counter;
  }
  else {
    interruptType = 0;
    counter = 0;
  }
}

// Function goOff turns all the LEDs off at once
void goOff() {
  interruptType = 0;
  counter = 0;
  for (int i = 0; i <= 6; ++i) {
    digitalWrite(i, LOW);
  }
}

