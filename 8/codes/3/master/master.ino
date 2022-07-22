#include <SPI.h>
#include <math.h>

#define SS1 22
#define SS2 23

#define R1 100

int lastTemperature = -100;
long lastLightIntensity = -100;

int temperature;
long lightIntensity;

void setup (void) {
   Serial.begin(9600); 
   Serial.println("master:");
   pinMode(SS1, OUTPUT);
   digitalWrite(SS1, HIGH);
   pinMode(SS2, OUTPUT);
   digitalWrite(SS2, HIGH);
   SPI.begin ();
   SPI.setClockDivider(SPI_CLOCK_DIV8);

   pinMode(A0, INPUT);
   pinMode(A1, INPUT);
}

void loop (void) {

  temperature = analogRead(A0) / 308.0 * 150;
  lightIntensity = R1 * (1023.0 / analogRead(A1) - 1);
  
  if (abs(lastLightIntensity - lightIntensity) >= 1) {
    digitalWrite(SS1, LOW);

    int l = snprintf(NULL, 0, "%d", lightIntensity);
    const char *str = malloc((l + 1) * sizeof(char));
    snprintf(str, l + 1, "%d", lightIntensity);

    for (int i = 0; i < strlen(str); ++i)
      SPI.transfer(str[i]);
    SPI.transfer('\r');
    Serial.print("Master: L: ");
    Serial.println(str);
    free(str);

    digitalWrite(SS1, HIGH);
    lastLightIntensity = lightIntensity;
  }
  
  if (abs(lastTemperature - temperature) >= 1) {
    digitalWrite(SS2, LOW);

    int l = snprintf(NULL, 0, "%d", temperature);
    const char *str = malloc((l + 1) * sizeof(char));
    snprintf(str, l + 1, "%d", temperature);

    for (int i = 0; i < strlen(str); ++i)
      SPI.transfer(str[i]);
    SPI.transfer('\r');
    Serial.print("Master: T: ");
    Serial.println(str);
    free(str);
    
    digitalWrite(SS2, HIGH);
    lastTemperature = temperature;
  }   
}
