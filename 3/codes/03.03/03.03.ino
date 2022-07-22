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
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'C', '0', '=', '+'}
};
byte rowPins[ROWS] = {39, 40, 41, 42}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {43, 44, 45, 46}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// End of Keypad settup

String opnd1;
String opnd2;
char optr;
int rslt;
bool op = false;
int op1;
int op2;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.waitForKey();

  switch(key) {
    case 'C':
      lcd.clear();
      lcd.setCursor(0, 0);
      opnd1 = "";
      opnd2 = "";
      optr = "";
      op = false;
      break;
    case '=':
      op1 = opnd1.toInt();
      op2 = opnd2.toInt();
      switch(optr) {
        case '/':
          rslt = op1 / op2;
          break;
        case '*':
          rslt = op1 * op2;
          break;
        case '-':
          rslt = op1 - op2;
          break;
        case '+':
          rslt = op1 + op2;
        break;
      }
      lcd.setCursor(0, 1);
      lcd.print(rslt);
      break;
    default:
      if (key >= 48 && key <= 57) {
        if (!op)
          opnd1 += key;
        else
          opnd2 += key;
        
        lcd.print(key);
      }
      else if (!op) {
        optr = key;
        op = true;
        lcd.print(key);
      }
      break;
  }
}
