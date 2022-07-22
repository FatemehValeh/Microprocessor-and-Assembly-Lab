#include <LiquidCrystal.h>

#define ENB 12
#define RS 13
#define DB4 11
#define DB5 10
#define DB6 9
#define DB7 8

LiquidCrystal lcd(RS, ENB, DB4, DB5, DB6, DB7);

byte cursorPos = 0;
char entry = 'A';
int period = 1000;
unsigned long previous_time = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long current_time = millis();
  if(current_time - previous_time >= period){
    lcd.clear();
    lcd.setCursor(cursorPos % 16, (cursorPos % 2));
    lcd.write(entry);
    cursorPos = ++cursorPos;
    previous_time = current_time;
  }
}
