#include <Keypad.h>

#include "pitches.h"
#include "MarioMain.h"
#include "EyIran.h"
#include "MarioUnderWorld.h"
#include "PiratesOfCaribbean.h"
#include "ode-to-joy.h"
#include "the-imperial-march.h"

#define PIN 8

// Keypad settup
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {25, 26, 27, 28}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {24, 23, 22}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// End of Keypad settup

float scale = 0;
int *melody = melody0;
int *noteDurations = noteDurations0;
int noteSize;

void setup() {
  // put your setup code here, to run once:
}

void handleKey(char key) {

  switch (key) {
    case '1':
      melody = melody0;
      noteDurations = noteDurations0;
      noteSize = sizeof(noteDurations0) / sizeof (int);
      break;
    case '2':
      melody = melody1;
      noteDurations = noteDurations1;
      noteSize = sizeof(noteDurations1) / sizeof (int);
      break;
    case '3':
      melody = melody2;
      noteDurations = noteDurations2;
      noteSize = sizeof(noteDurations2) / sizeof (int);
      break;
    case '4':
      melody = melody3;
      noteDurations = noteDurations3;
      noteSize = sizeof(noteDurations3) / sizeof (int);
      break;
    case '5':
      melody = melody4;
      noteDurations = noteDurations4;
      noteSize = sizeof(noteDurations4) / sizeof (int);
      break;
    case '6':
      melody = melody5;
      noteDurations = noteDurations5;
      noteSize = sizeof(noteDurations5) / sizeof (int);
  }
  if (key >= '1' && key <= '6')
    playTune();
}

void playTune() {
  for(int note = 0; note < noteSize; note++){
    scale = analogRead(A0) / 512.0;
    int noteDuration = 70 / noteDurations[note];
    tone(PIN, (int)(16 * melody[note] * scale), noteDuration);
    delay((int) (1.1 * noteDuration));
  }  
}

void loop() {

  char key = keypad.getKey();
  if (key)
    handleKey(key);
}
