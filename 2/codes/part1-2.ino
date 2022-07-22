#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'6', '5', '4', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
int key = 0;
byte rowPins[ROWS] = {31, 33, 35, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {29, 27, 25, 23}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial2.begin(9600);
  for (int i = 0; i <= 8; ++i) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  key = keypad.waitForKey();
  if (key == 48) {
    goOff();
  }
  else if (key > 48 && key <= 57) {
    int i;
    for (i = 0; i < key - 48; ++i)
      digitalWrite(i, HIGH);
    for (; i <= 57; ++i)
      digitalWrite(i, LOW);
  }
  Serial2.write(key);
}
void goOff() {
  for (int i = 0; i <= 8; ++i) {
    digitalWrite(i, LOW);
  }
}
