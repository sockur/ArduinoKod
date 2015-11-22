#include <VirtualWire.h>
#include <OneWire.h>

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

char msg[6];

void setup() {
Serial.begin(9600);	
sensors.begin();

vw_setup(2000);

vw_set_tx_pin(3);

}

void loop() {

sensors.requestTemperatures();

float temp = sensors.getTempCByIndex(0);
Serial.print(temp);
char temperature[10];  
dtostrf(temp,4,3,temperature);


dtostrf(temp, 6, 2, msg);
//Serial.print(msg);
//Serial.println(temperature);
    
vw_send((uint8_t *)msg, strlen(msg));

vw_wait_tx();

delay(10000);

}
