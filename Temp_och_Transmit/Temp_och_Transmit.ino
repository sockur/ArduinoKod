
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
 
RH_ASK driver;
int i = 0;
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
char msg[6]; 
char nodeName[] = "kitchen";

void setup() {
  Serial.begin(9600);	  // Debugging only
    if (!driver.init())
         Serial.println("init failed");

    // Start up the library
    sensors.begin();


}

void loop() {
// Transmit
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);
    char buffer [100]; 
    char strTemp[6];
    dtostrf(temperature, 4, 1, strTemp); 
    float test =1.2;    
    int len = sprintf (buffer, "%s;TEMP2;%s", nodeName, strTemp);  
    Serial.println(buffer);
    
    driver.send((uint8_t *)buffer, strlen(buffer));
    driver.waitPacketSent();
    delay(5000);
 // End transmit 
    


}





 char* getTemp(){

  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  char* tempChar; 
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tempFloat = sensors.getTempCByIndex(0);
  Serial.println(tempFloat,1);
  dtostrf(tempFloat, 4, 3, tempChar);
  return tempChar;
    
}
