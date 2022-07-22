#include <Tone.h>

#define BUZZPIN0 8
#define BUZZPIN1 9
#define BUZZPIN2 10

Tone tone0;
Tone tone1;
Tone tone2;

#define LED1 13 
#define LED2 12
#define LED3 11

#define DIVIDER 4
 
float scale = 0;

int notes[3][5] = {
  {NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1},
  {NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2},
  {NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3, NOTE_C3}
  };
int frets[3] = {0};
boolean nutSelected[3] = {false};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

   tone0.begin(BUZZPIN0);
   tone1.begin(BUZZPIN1);
   tone2.begin(BUZZPIN2);
   
   
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    char c = Serial.read();
    Serial.println(c);
    
    switch (c){
      case 'o':
        if(nutSelected[0]){
            nutSelected[0] = false;
            digitalWrite(LED1, LOW);
            tone0.stop();
          }
         else{
          nutSelected[0] = true;
          digitalWrite(LED1, HIGH);
          }
         break;

       case 'k':
        if(nutSelected[1]){
            nutSelected[1] = false;
            digitalWrite(LED2, LOW);
            tone1.stop();
          }
         else{
          nutSelected[1] = true;
          digitalWrite(LED2, HIGH);
          }
         break;

       case 'm':
        if(nutSelected[2]){
            nutSelected[2] = false;
            digitalWrite(LED3, LOW);
            tone2.stop();
          }
         else{
          nutSelected[2] = true;
          digitalWrite(LED3, HIGH);
          }
         break;
          
        
      case 'q':
        frets[0] = 0;
        break;
      case 'w':
        frets[0] = 1;
        break;
      case 'e':
        frets[0] = 2;
        break;
      case 'r':
        frets[0] = 3;
        break;
      case 't':
        frets[0] = 4;
        break;
      
      case 'a':
        frets[1] = 0;
        break;
      case 's':
        frets[1] = 1;
        break;
      case 'd':
        frets[1] = 2;
        break;
      case 'f':
        frets[1] = 3;
        break;
      case 'g':
        frets[1] = 4;
        break;
     
      case 'z':
        frets[2] = 0;
        break;
      case 'x':
        frets[2] = 1;
        break;
      case 'c':
        frets[2] = 2;
        break;
      case 'v':
        frets[2] = 3;
        break;
      case 'b':
        frets[2] = 4;
        break;
    }
  if(nutSelected[0]){
    scale = analogRead(A0) / 512.0;
    tone0.play((int) (notes[0][frets[0]] * scale * DIVIDER));  
    }

  if(nutSelected[1]){
    scale = analogRead(A1) / 512.0;
    tone1.play((int) (notes[1][frets[1]] * scale * DIVIDER));
    }
    
  if(nutSelected[2]){
    scale = analogRead(A2) / 512.0;
    tone2.play((int) (notes[2][frets[2]] * scale * DIVIDER));
    }
            
    }
}
