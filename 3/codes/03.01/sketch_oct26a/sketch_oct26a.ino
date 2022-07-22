#include <LiquidCrystal.h>

#define ENB 12
#define RS 13
#define DB4 11
#define DB5 10
#define DB6 9
#define DB7 8

LiquidCrystal lcd(RS, ENB, DB4, DB5, DB6, DB7);

byte cursorPos = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.setCursor(cursorPos % 5, (cursorPos / 5) % 2);
  lcd.print("CONQUISTADOR");
  cursorPos = ++cursorPos;
  delay(400);
}
