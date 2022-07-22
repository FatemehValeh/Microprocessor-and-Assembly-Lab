#include <SPI.h>
#define SS1 22
void setup (void) {
   Serial.begin(9600); 
   Serial.println("master:");
   pinMode(SS1, OUTPUT);
   digitalWrite(SS1, HIGH);
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop (void) {
   char c;
   const char *p = "Valeh-9831121\r";
   digitalWrite(SS1, LOW); 
   // send test string
   for ( ; c = *p; p++) {
      SPI.transfer (c);
      Serial.print(c);
   }
   digitalWrite(SS1, HIGH);
   delay(1000);
}
