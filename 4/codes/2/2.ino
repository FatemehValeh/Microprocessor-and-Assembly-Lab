#include <Keypad.h>
#include <Servo.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'6', '5', '4', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
char key = 0;
byte rowPins[ROWS] = {31, 33, 35, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {29, 27, 25, 23}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo myServo;  // create servo object to control a servo

String input = "";
int val;    // variable to read the value from the analog pin

void setup() {
  myServo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  key = keypad.getKey();
  if (key == '#'){
    val = 90;
    input = "";
    myServo.write(val);
  }
  if (key == '*'){
    val = input.toInt();
    myServo.write(val);
    input = "";
  }
  if ( 48 <= key && key <= 57){
    input += key;
  }                       
}
