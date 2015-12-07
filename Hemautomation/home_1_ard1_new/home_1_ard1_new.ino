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
int vw_data_speed = 2000;
int msgId = 0;
int waitTimeAck = 5000;
int delayTime = 300;

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
msg_t message;

// Acknowledge message type
typedef struct {
  int deviceId;
  int msgId;
} ack_t; 
ack_t ack;


void initVirtualWire(){
  Serial.println("Init VirtualWire");
 vw_set_tx_pin(3);
  vw_set_rx_pin(4);
  vw_setup(2000);
  vw_rx_start();
 
    
    
}  

void setup() {
  Serial.begin(9600);	
  message.roomId = ROOM_ID;
  message.deviceId = ARDUINO_ID;
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
  Serial.println(message.deviceId);
  Serial.println(message.sensorId);
  Serial.println(message.msgId);
  Serial.println(message.data1_millisecs);
  Serial.println(message.data2);

//  vw_send((uint8_t *)&message, sizeof(message));
//  vw_wait_tx();
  
   send();
  
 // vw_wait_rx_max(300);
 // uint8_t statys = vw_get_message(uint8_t* buf, uint8_t* len);
  
}  

void send(){
  // Send message and receive acknowledgement

  uint8_t len; 
  
  do {
    msgId ++;
  message.msgId = msgId;
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
  uint8_t buf[VW_MAX_MESSAGE_LEN];
    vw_send((uint8_t *)&message, sizeof(message));
    vw_wait_tx();
    
    
    //vw_wait_rx_max(waitTimeAck);
//vw_wait_rx();
   //Serial.print("buf ");
   // Serial.println( (char*) buf);

    //vw_get_message(buf, &buflen);
    
    uint8_t buf2[VW_MAX_MESSAGE_LEN];
    uint8_t buflen2 = VW_MAX_MESSAGE_LEN;

    vw_wait_rx();
    if (vw_get_message(buf2, &buflen2)) // Non-blocking
    {
      Serial.println("got messsssssage");
      Serial.print("buflen2 length ");
      Serial.println(buflen2);
      
        if (buflen2 == sizeof(ack_t)){
          memcpy( &ack, buf2, sizeof(ack));
          Serial.print("ack.deviceId:");
          Serial.println(ack.deviceId);
          Serial.print("ack.msgId:");
          Serial.println(ack.msgId);
        }
    } else {
       Serial.println("Got no message");
    }  
   
//    vw_get_message(buf, &buflen);
//    memcpy( &ack, buf, sizeof(ack)); 
    //memcpy( &msg, buf, sizeof(msg));
          
    
    
    //len = rx.recv(&ack, sizeof(ack), 64);
    //}
    delay(300);
  } while ((ack.msgId != message.msgId) || (ack.deviceId != message.deviceId));
    
}



void initDallasTemp(){
  sensors.begin();
}  


