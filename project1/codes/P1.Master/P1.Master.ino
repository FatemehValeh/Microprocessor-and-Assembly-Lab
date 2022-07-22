#include <LiquidCrystal.h>
#include <Servo.h>
#include <math.h>
#include <SPI.h>

// Servo setup
Servo myservo;
#define SERVOPIN 9

#define BUZZERPIN 17

// LCD setup
#define ENB 23
#define RS 22
#define DB4 24
#define DB5 25
#define DB6 26
#define DB7 27
LiquidCrystal lcd(RS, ENB, DB4, DB5, DB6, DB7);

#define SS1 41
#define SS2 42

int lastRecordedTemp;
int temperature;
long lastUpdateTime;
long points;

byte stage;
// stage = 0: Stopped
// stage = 1: Working normally
// stage = 2: In the desired range
// stage = 3: Above the threshold, in danger

void pickTemperature() {
  temperature = analogRead(A0) / 308.0 * 150;
  if (temperature < 60)
    stage = 1;
  else if (temperature >= 60 && temperature <= 70) {
    stage = 2;
    lastUpdateTime = millis();
  }
  else if (temperature > 70)
    stage = 3;
}

void reportTemperature(int mode) {
  if (abs(temperature - lastRecordedTemp) >= 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    if (mode == 1) {
      lcd.setCursor(0, 1);
      lcd.print("Points: ");
      lcd.print(points);
    }
    lastRecordedTemp = temperature;    
  }
}

void updatePoints() {
  int newPoints = millis() - lastUpdateTime;
  points += newPoints;
  lcd.setCursor(0, 1);
  lcd.print("Points: ");
  lcd.print(points);
  if (newPoints >= 1)
    lastUpdateTime = millis();
}

void warn() {
  warnOtherDevices();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("D A N G E R");
  lcd.setCursor(0, 1);
  lcd.print("Stopping ");
  int timer = 6;
  int buzzerOn = 1;
  while (timer > 0) {
    lastUpdateTime = millis();
    --timer;
    if (buzzerOn == 1) {
      digitalWrite(BUZZERPIN, HIGH);
      digitalWrite(19, HIGH);
      buzzerOn = 0;
    }
    else {
      digitalWrite(BUZZERPIN, LOW);
      digitalWrite(19, LOW);
      buzzerOn = 1;
    }
    delay(100);
  }
}

void warnOtherDevices() {
  digitalWrite(SS1, LOW);
  digitalWrite(SS2, LOW);

  const char *str = "T";

  for (int i = 0; i < strlen(str); ++i)
    SPI.transfer(str[i]);
  SPI.transfer('\r');
  free(str);

  digitalWrite(SS1, HIGH);
  digitalWrite(SS2, HIGH);
}

void restartOtherDevices() {
  digitalWrite(SS1, LOW);
  digitalWrite(SS2, LOW);

  const char *str = "S";

  for (int i = 0; i < strlen(str); ++i)
    SPI.transfer(str[i]);
  SPI.transfer('\r');
  free(str);

  digitalWrite(SS1, HIGH);
  digitalWrite(SS2, HIGH);
}

void restart() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Restarting in:");
  lcd.setCursor(0, 1);
  int timer = 5;
  lcd.print(timer);
  lcd.print("s");
  while (timer > 0) {
    lastUpdateTime = millis();
    --timer;
    lcd.setCursor(0, 1);
    lcd.print(timer);
    delay(100);
  }
  restartOtherDevices();
  stage = 1;
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(A0, INPUT);
  lastRecordedTemp = -1;
  lastUpdateTime = 0;
  points = 0;
  myservo.attach(SERVOPIN);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(SS1, OUTPUT);
  digitalWrite(SS1, HIGH);
  pinMode(SS2, OUTPUT);
  digitalWrite(SS2, HIGH);
  pinMode(19, OUTPUT);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop() {
  // put your main code here, to run repeatedly:
  pickTemperature();
 
  myservo.write(temperature + 50);

  switch(stage) {
    case 1:
      reportTemperature(1);
      break;
    case 2:
      reportTemperature(0);
      updatePoints();
      break;
    case 3:
      myservo.write(180);
      warn();
      restart();
      myservo.write(0);
      break;
  }
}
