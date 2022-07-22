#include <Servo.h>

Servo myservo; 

int val;    

void setup() {
  myservo.attach(9);  
  Serial.begin(9600);
}

void loop() {
  Serial.println("Potentiometer:");
  val = analogRead(A0);            
  val = map(val, 0, 1023, 0, 180);     
  Serial.println(val);
  myservo.write(val);                  
  delay(300);                          
}
