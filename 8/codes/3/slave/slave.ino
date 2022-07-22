#include <SPI.h>
char buff [50];
volatile byte indx;
volatile boolean process;

void setup (void) {
   Serial.begin (9600);
   Serial.println("slave:");
   pinMode(MISO, OUTPUT); 
   digitalWrite(MISO,LOW);
   SPCR |= _BV(SPE); 
   indx = 0; 
   process = false;
   SPI.attachInterrupt(); 
}
ISR (SPI_STC_vect){  
   byte c = SPDR;
   if (indx < sizeof buff) {
      buff [indx++] = c;
      if (c == '\r')
      process = true;
   }
}

void loop (void) {
   if (process) {
      process = false; 
      Serial.print (buff);
      indx= 0;
   }
}
