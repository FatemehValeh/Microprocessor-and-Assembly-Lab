#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'6', '5', '4', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {31, 33, 35, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {29, 27, 25, 23}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial2.begin(9600);
  Serial2.setTimeout(100000);
  for (int i = 0; i <= 8; ++i) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}

void loop() {
  if (Serial2.available() > 0) {
    int entry = Serial2.parseInt();
    Serial2.println(entry);
    if (entry >= 0 && entry <= 9 ) {
      int i;
      for (i = 0; i < entry; ++i)
        digitalWrite(i, HIGH);
      for (; i < 9; ++i)
        digitalWrite(i, LOW);
    }
    else
      Serial2.println("Invalid Number");
  }
}
