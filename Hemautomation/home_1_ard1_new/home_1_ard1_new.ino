#include <VirtualWire.h>

//#include <EasyTransferVirtualWire.h>
#include <OneWire.h>

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

int ROOM_ID = 1;
int ARDUINO_ID = 1;
int dallasTempSensorId = 1;
int vw_tx_pin = 3;
int vw_data_speed = 2000;




typedef struct {		
  int           roomId; 		//node ID (1xx, 2xx, 3xx);  1xx = basement, 2xx = main floor, 3xx = outside
  int		arduinoId;		//sensor ID (2, 3, 4, 5)
  int           sensorId;
  unsigned long data1_millisecs; 		//uptime in ms
  int           data2;   	        //sensor data?
  int           data2_factor;
  int		data3;		//battery condition?
  int           data3_factor;
} Message;
Message message;



char msg[6];

void setup() {
  Serial.begin(9600);	
  message.roomId = ROOM_ID;
  message.arduinoId = ARDUINO_ID;
  initDallasTemp();  
  initVirtualWire();
}



void loop() {
  sendDallasTemp();

delay(10000);

}

void sendDallasTemp(){
  message.sensorId = dallasTempSensorId;
  // Temperature  
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  message.data1_millisecs = millis();
  message.data2 = (temp*100);
  message.data2_factor = 100;
  message.data3 = 0;

  Serial.println(message.roomId);
  Serial.println(message.arduinoId);
  Serial.println(message.sensorId);
  Serial.println(message.data1_millisecs);
  Serial.println(message.data2);

  vw_send((uint8_t *)&message, sizeof(message));
  vw_wait_tx();
  
}  


void initDallasTemp(){
  sensors.begin();
}  

void initVirtualWire(){
  vw_setup(vw_data_speed);
  vw_set_tx_pin(vw_tx_pin);
}  
