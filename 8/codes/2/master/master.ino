#include <SPI.h>
#define SS1 22
#define SS2 23
void setup (void) {
   Serial.begin(9600); 
   Serial.println("master:");
   pinMode(SS1, OUTPUT);
   digitalWrite(SS1, HIGH);
   pinMode(SS2, OUTPUT);
   digitalWrite(SS2, HIGH);
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop (void) {
   char c, d;
   const char * myName = "Amir\r";
   digitalWrite(SS2, LOW); 
   for ( ; c = *myName; myName++) {
      SPI.transfer (c);
      Serial.print(c);
   }
   digitalWrite(SS2, HIGH);
   delay(1000);
   
   const char * hello = "Hello Amir\r";
   digitalWrite(SS1, LOW);
    for ( ; d = *hello; hello++) {
      SPI.transfer (d);
      Serial.print(d);
   }
   digitalWrite(SS1, HIGH);
   delay(1000);
   
   
}
