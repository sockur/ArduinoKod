#include <VirtualWire.h>

//#include <EasyTransferVirtualWire.h>
#include <OneWire.h>

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
//EasyTransfer ET;

struct DATA_STRUCTURE{
 int int_data;
 char char_data;
};
DATA_STRUCTURE myData;

typedef struct {		
  int           nodeID; 		//node ID (1xx, 2xx, 3xx);  1xx = basement, 2xx = main floor, 3xx = outside
  int		deviceID;		//sensor ID (2, 3, 4, 5)
  unsigned long data1_millisecs; 		//uptime in ms
  int         data2_float;   	        //sensor data?
  int		data3_float;		//battery condition?
} Payload;
Payload message;



char msg[6];

void setup() {
  Serial.begin(9600);	
  sensors.begin();

  vw_setup(2000);

  vw_set_tx_pin(3);
  message.nodeID = 1;
  //ET.begin(details(myData), &Serial);
}

void loop() {

// Temperature  
message.deviceID = 1;
sensors.requestTemperatures();

float temp = sensors.getTempCByIndex(0);
message.data1_millisecs = millis();
message.data2_float = temp;
message.data3_float = 0;

Serial.print(temp);
char temperature[10];  
dtostrf(temp,4,3,temperature);


dtostrf(temp, 6, 2, msg);
//Serial.print(msg);
//Serial.println(temperature);

//radio.sendWithRetry(GATEWAYID, (const void*)(&theData), sizeof(theData));    
vw_send((uint8_t *)msg, strlen(msg));
vw_wait_tx();

delay(120000);

}
