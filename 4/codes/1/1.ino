#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int i;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (i = 180; i>90 ; i--){
    myservo.write(i);
    delay(15);
  }
  for (i=90 ; i < 180 ; i++){
    myservo.write(i);
    delay(15);
  }
 
}
