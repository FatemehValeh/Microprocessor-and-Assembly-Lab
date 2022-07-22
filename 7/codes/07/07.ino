#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>

// LCD settup
#define ENB 23
#define RS 22
#define DB4 24
#define DB5 25
#define DB6 26
#define DB7 27

#define DEVICE_ADDRESS 0b1010000
// Memory addresses:
#define RESUMABLE_ADDRESS 0x00
#define REMAININGTIME_ADDRESS 0x07 // 4 address spaces for remaining times, 0x06, 0x07, 0x08, 0x09
#define FLAG_ADDRESS 0x0B // check whether the memory was empty or fulled by the user. The value 0xAA means the memory is not empty


LiquidCrystal lcd(RS, ENB, DB4, DB5, DB6, DB7);
// End of LCD settup

// Keypad settup
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {31, 32, 33, 34}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {28, 29, 30}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// End of Keypad settup

String entry;
int stage;
// stage = -1: Waiting for the user to press (*)/(#) to (start or resume the programme)/(change the default settings)
// stage = 0: Waiting for the user to enter the default step followed by an '*'
// stage = 1: Waiting for the user to enter stop time for the 1st step followed by an '*'
// stage = 2: Waiting for the user to enter stop time for the 2nd step followed by an '*'
// stage = 3: Waiting for the user to enter stop time for the 3rd step followed by an '*'
// stage = 4: Waiting for the user to enter stop time for the 4th step followed by an '*'
// stage = 5: Waiting for the washings to finish: Reporting the current step and the remaining time
// stage = 6: Paused while washing

int curStep;
int defaultStep;
int stopTime[4];
int remainingTime[4];
long lastUpdateTime;
boolean resumable;
boolean lcdOn;

boolean updateTimer() {
  if (millis() - lastUpdateTime >= 300) {
    --remainingTime[curStep - 1];
    if (remainingTime[curStep - 1] < 0)
      ++curStep;
    if (curStep > 4) {
      notifyUser();
      return false;
    }
    lastUpdateTime = millis();
    return true;
  }
  return false;
}

void notifyUser() {
  resumable = false;
  saveData();
  lcd.setCursor(0, 0);
  lcd.print("    Finished");
  if (millis() - lastUpdateTime >= 100) {
    if (lcdOn) {
      lcd.clear();
      lcdOn = false;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("    Finished");
      lcdOn = true;
    }
    lastUpdateTime = millis();
  }
}

void resumeWashing() {
  if (updateTimer()) {
    saveData(curStep);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Step: ");
    lcd.print(curStep);
    lcd.print(" Time: ");
    lcd.print(remainingTime[curStep - 1]);
    lcd.setCursor(0, 1);
    lcd.print("Prs # to pause");
  }
  
}

boolean readData() {
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(FLAG_ADDRESS);
  Wire.endTransmission();

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.requestFrom(DEVICE_ADDRESS, 1);
  byte flag = Wire.read();
  Wire.endTransmission();
  if (flag != 0xAA){ // memory is empty
    defaultStep = 1;
    curStep = defaultStep;
    resumable = false;
    for(int i=0; i<4; i++)
      stopTime[i] = 2;
    for(int i=0; i<4; i++)
      remainingTime[i] = stopTime[i];
    return false;
  }
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(RESUMABLE_ADDRESS);
  Wire.endTransmission();

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.requestFrom(DEVICE_ADDRESS, 11);
  defaultStep = Wire.read();
  curStep = Wire.read();
  resumable = (Wire.read() == 0xAA) ? true : false;
  for(int i=0; i<4; i++)
    stopTime[i] = Wire.read();
  if (!resumable) {
    for(int i=0; i<4; i++)
      remainingTime[i] = stopTime[i];
    curStep = defaultStep;
  }
  else {
    for(int i=0; i<4; i++)
      remainingTime[i] = Wire.read();
  }
  return resumable;
}

void saveData() {
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(RESUMABLE_ADDRESS);
  Wire.write(defaultStep);
  Wire.write(curStep);
  if (resumable)
    Wire.write(0xAA);
  else
    Wire.write(0);
  for(int i=0; i<4; i++){
    Wire.write(stopTime[i]);
  }
  for(int i=0; i<4; i++){
    Wire.write(remainingTime[i]);
  }
  Wire.write(0xAA);
  
  Wire.endTransmission();
}

void saveData(int cs) {
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(0x01);
  Wire.write(cs);
  Wire.endTransmission();

  delay(50);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(REMAININGTIME_ADDRESS + cs - 1);
  Wire.write(remainingTime[cs - 1]);
  Wire.endTransmission();
}

void handleKey(char key) {

  switch (key) {
    case '#':
      lcd.clear();
      lcd.setCursor(0, 0);
      if (stage == -1) {
        stage = 0;
        lcd.print("Enter dflt step:");
        lcd.setCursor(0, 1);
      } else if (stage >= 0 && stage <= 4) {
        lcd.print("Prs * to start");
        lcd.setCursor(0, 1);
        lcd.print("Prs # to set");
        entry = "";
        stage = -1;
      } else if (stage == 5) {
        lcd.print("Prs # to end");
        lcd.setCursor(0, 1);
        lcd.print("Prs * to unpause");
        stage = 6;
      } else if (stage == 6) {
        for (int i = 0; i < 4; ++i)
          remainingTime[i] = stopTime[i];
        curStep = defaultStep;
        resumable = false;
        saveData();
        lcd.print("Prs * start");
        lcd.setCursor(0, 1);
        lcd.print("Prs # to set");
        stage = -1;
      }
      break;
    case '*':
      lcd.clear();
      lcd.setCursor(0, 0);
      if (stage == -1 || stage == 6) {
        resumable = true;
        saveData();
        stage = 5;
      }
      else if (entry.length() != 0) {
        if (stage == 0) {
          defaultStep = entry.toInt();
          stage = 1;
          lcd.print("Time 4 step: ");
          lcd.print(stage);
          lcd.setCursor(0, 1);
        } else if (stage != 5) {
          stopTime[stage - 1] = entry.toInt();
          if (stage == 4) {
            stage = -1;
            curStep = defaultStep;
            for (int i = 0; i < 4; ++i)
              remainingTime[i] = stopTime[i];
            lcd.print("Prs * to start");
            lcd.setCursor(0, 1);
            lcd.print("Prs # to set");
            saveData();
          } else {
            ++stage;
            lcd.print("Time 4 step: ");
            lcd.print(stage);
            lcd.setCursor(0, 1);
          }
        }
        entry = "";
      }
      break;
    default:
      if (stage >= 0 || stage <= 4) {
      entry = entry + key;
      lcd.print(key);
      }
  }
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  for (int i = 8; i < 12; ++i) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  stage = -1;
  lcdOn = true;
  lastUpdateTime = millis();

  Wire.begin();
  if (readData()) {
    lcd.print("Prs * to res");
  } else {
    lcd.print("Prs * to start");
  }
  lcd.setCursor(0, 1);
  lcd.print("Prs # to set");
}

void setLEDs() {
  if (stage != 5 && stage != 6)
    for (int i = 0; i < 4; ++i)
      digitalWrite(8 + i, LOW);
  if (curStep > 4)
    for (int i = 0; i < 4; ++i)
      digitalWrite(8 + i, HIGH);
  else
    for (int i = 0; i < 4; ++i) {
      if (i == curStep - 1)
        digitalWrite(8 + i, HIGH);
      else
        digitalWrite(8 + i, LOW);
    }
}

void loop() {
  // put your main code here, to run repeatedly:

  setLEDs();
  
  if (stage == 5)
    resumeWashing();
  
  char key = keypad.getKey();
  if (key)
    handleKey(key);
}
