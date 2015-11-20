#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;
char StringReceived[22]; 

void setup()
{
    Serial.begin(9600);	// Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    uint8_t buf[50];
   // char buf[50];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      //driver.printBuffer("Got:", buf, buflen);
      digitalWrite(13, true);
      
    for (int i = 0; i < buflen; i++)
 {            
          // Fill Sensor1CharMsg Char array with corresponding 
          // chars from buffer.   
          StringReceived[i] = char(buf[i]);
 }
  Serial.println(StringReceived);
//  sscanf(StringReceived, "%d,%d,%d,%d,%d,%d",&Sensor1Data, &Sensor2Data,&SensorData3,&SensorData4); // Converts a string to an array
      
    }
}
