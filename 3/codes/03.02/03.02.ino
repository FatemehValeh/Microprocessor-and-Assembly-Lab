#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD settup
#define ENB 12
#define RS 13
#define DB4 11
#define DB5 10
#define DB6 9
#define DB7 8

LiquidCrystal lcd(RS, ENB, DB4, DB5, DB6, DB7);

byte cursorPos = 0;
// End of LCD settup

// Keypad settup
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
// End of Keypad settup

String entry;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");
  lcd.setCursor(0, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.waitForKey();

  switch(key) {
    case '#':
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Enter Password");
      lcd.setCursor(0, 1);
      entry = "";
      break;
    case '*':
      if (!entry.equals("123456")) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Wrong Password");
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Correct Password");
      }
      break;
    default:
      entry = entry + key;
      lcd.print(key);
  }
}
