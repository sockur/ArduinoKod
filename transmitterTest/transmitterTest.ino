#include <VirtualWire.h>
#include <OneWire.h>

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

int ROOM_ID = 1;
int ARDUINO_ID = 1;
int dallasTempSensorId = 1;
int vw_data_speed = 2000;
int waitTimeAck = 5000;
int delayTime = 300;
int msgid = 0; 
int loopNo = 0; 
//

typedef struct {		
  int           roomId; 		//node ID (1xx, 2xx, 3xx);  1xx = basement, 2xx = main floor, 3xx = outside
  int		deviceId;		//sensor ID (2, 3, 4, 5)
  int           sensorId;
  int           msgId;
  unsigned long data1_millisecs; 		//uptime in ms
  int           data2;   	        //sensor data?
  int           data2_factor;
  int		data3;		//battery condition?
  int           data3_factor;
} msg_t;
msg_t msg;


// Acknowledge message type
typedef struct {
  int deviceId;
  int msgId;
} ack_t; 
ack_t ack;

void setup()
{
    Serial.begin(9600);	  // Debugging only
    Serial.println("setup");
    initVirtualWire();
    initDallasTemp();
}


void initDallasTemp(){
  sensors.begin();
}  


void initVirtualWire(){
  Serial.println("Init VirtualWire");
  vw_set_tx_pin(5);
  vw_set_rx_pin(6);
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();
}  


void loop()
{    
    loopNo++;
    Serial.print("loopNo:");
    Serial.println(loopNo);
    sendMsg();
    delay(2000);
    
}

void sendMsg(){
  int attempt = 0;
    do{
      attempt++; 
      msgid++;
      msg.deviceId = 1;
      msg.msgId = msgid;
      digitalWrite(13, true); // Flash a light to show transmitting
      vw_send((uint8_t *)&msg, sizeof(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      digitalWrite(13, false);
    
      uint8_t buf[VW_MAX_MESSAGE_LEN];
      uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    // Wait 100 ms for an ack
    vw_wait_rx_max(100);
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
            Serial.println("Got msg");
    }  
    if (buflen == sizeof(ack_t)){
          memcpy( &ack, buf, sizeof(ack));    
         
         if ((ack.msgId == msg.msgId) && (ack.deviceId == msg.deviceId)) {
          Serial.print("Attempt no: "); 
          Serial.println(attempt);
          Serial.print("ack.deviceId:");
          Serial.println(ack.deviceId);
          
          Serial.print("msg.msgId:");
          Serial.println(msg.msgId);
          Serial.print("ack.msgId:");
          Serial.println(ack.msgId);
             
           
         }  

    }     
    delay(200);
    } while ((ack.msgId != msg.msgId) || (ack.deviceId != msg.deviceId));
  
}  


void sendDallasTemp(){
  
  msg.sensorId = dallasTempSensorId;
  // Temperature  
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  msg.data1_millisecs = millis();
  msg.data2 = (temp*100);
  msg.data2_factor = 100;
  msg.data3 = 0;

  Serial.println(msg.roomId);
  Serial.println(msg.deviceId);
  Serial.println(msg.sensorId);
  Serial.println(msg.msgId);
  Serial.println(msg.data1_millisecs);
  Serial.println(msg.data2);
  sendMsg();
  
 // vw_wait_rx_max(300);
 // uint8_t statys = vw_get_message(uint8_t* buf, uint8_t* len);
  
}  
