#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#include <SPI.h>

// Servo setup
Servo myservo;
#define SERVOPIN 9

#define BUZZERPIN 17
// LCD settup
#define ENB 23
#define RS 22
#define DB4 24
#define DB5 25
#define DB6 26
#define DB7 27

LiquidCrystal lcd(RS, ENB, DB4, DB5, DB6, DB7);

byte cursorPos = 0;
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
byte stage;
// stage = 0: Waiting for the user to press '*' and start the programme
// stage = 1: Waiting for the user to enter temperature followed by an '*'
// stage = 2: Waiting for the user to enter stop time followed by an '*'
// stage = 3: Waiting for temperature to ruse up to threshold
// stage = 4: Counting down the timer
// stage = 5: Finished cooking, thereby notifying the user

int temperature;
int stopTime;
int lastRecordedTemp;
long lastUpdateTime;
boolean lcdOn;
char buff[50];
volatile byte indx;
volatile boolean process;

boolean updateTimer() {
  --stopTime;
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print(stopTime);
  lastUpdateTime = stopTime;
  delay(50);
  return !stopTime;
}

void notifyUser() {
  playBuzzer();
  lcd.setCursor(0, 0);
  lcd.print("    Finished");
  if (millis() - lastUpdateTime >= 300) {
    if (lcdOn) {
      lcd.clear();
      lcdOn = false;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("    Finished");
      //playBuzzer();
    }
    lastUpdateTime = millis();
  }
}

void playBuzzer(){
  for (int k=0; k<5; k++){
    digitalWrite(BUZZERPIN, HIGH);
    delay(50);
    digitalWrite(BUZZERPIN, LOW);
    delay(50);
    }
  }

void reportTemp() {
  int curTemp = analogRead(A0) / 308.0 * 150;
  if (lastRecordedTemp - curTemp >= 1
      || curTemp - lastRecordedTemp >= 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(curTemp);
    lcd.setCursor(0, 1);
    lcd.print("Press # to reset");
    lastRecordedTemp = curTemp;
  }
  if (curTemp >= temperature)
      stage = 4;
}

void handleKey(char key) {

  if (stage != 3 && key == '*' || key == '#') {
    lcd.clear();
    lcd.setCursor(0, 0);
  }

  switch (key) {
    case '#':
      closeDoor();
      lcd.print("Press * to start");
      lcd.setCursor(0, 1);
      entry = "";
      stage = 0;
      break;
    case '*':
      if (stage == 0) {
      lcd.print("Enter Temp:");
      lcd.setCursor(0, 1);
      stage = 1;
              
      } else if (entry.length() != 0) {
        if (stage == 1) {
          temperature = entry.toInt();
          lcd.print("Enter Time:");
          lcd.setCursor(0, 1); 
          stage = 2;
          entry = "";
        } else if (stage == 2) {
          stopTime = entry.toInt();
          stage = 3;
          openDoor();
        }
      }
      break;
    default:
      if (stage == 1 || stage == 2) {
      entry = entry + key;
      lcd.print(key);
      }
  }
}

void openDoor(){
  myservo.write(180);
}

void closeDoor(){
  myservo.write(90);
}

ISR (SPI_STC_vect) {
   byte c = SPDR;
   if (indx < sizeof buff) {
      buff[indx++] = c;
      if (c == '\r')
      process = true;
   }
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press * to start");
  lcd.setCursor(0, 1);
  stage = 0;
  lcdOn = true;
  lastUpdateTime = 0;
  lastRecordedTemp = -1;
  myservo.attach(SERVOPIN);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(MISO, OUTPUT);
  digitalWrite(MISO,LOW);
  SPCR |= _BV(SPE);
  indx = 0;
  process = false;
  SPI.attachInterrupt();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (process) {
    process = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    if (buff[0] == 'S') {
      lcd.print("Restarting...");    
      delay(200);
      handleKey('#');
    }
    if (buff[0] == 'T') {
      lcd.print("D A N G E R");
      lcd.setCursor(0, 1);
      lcd.print("Turning Off");
      myservo.write(0);
      stage = 10;
      delay(500);
    }
    indx= 0;
  }
  
  if (stage == 3)
    reportTemp();
  
  else if (stage == 4) {
    if (updateTimer())
      stage = 5;
  }
  else if (stage == 5)
    notifyUser();
  
  char key = keypad.getKey();
  if (key)
    handleKey(key);
}
