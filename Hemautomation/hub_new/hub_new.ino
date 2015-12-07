#include <VirtualWire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };



IPAddress server(192, 168, 1, 34);
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

float temp;
boolean firstTime = true; 
int i;
unsigned long timeLast = 0; 
unsigned long timeDiff = 0; 
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


void initVirtualWire(){
   vw_set_tx_pin(3);
   vw_set_rx_pin(4);
   vw_setup(2000);  
   vw_rx_start();
   
   
  }
void setup(){
   Serial.begin(9600);
   mqttClient.setServer(server, 1883);
   mqttClient.setCallback(callback);
   initVirtualWire();
   if (Ethernet.begin(mac) == 0){
      Serial.println("Failed to configure Ethernet using DHCP");
      return;
   }
   delay(1500);
}

void loop(){
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  
  uint8_t buf[VW_MAX_MESSAGE_LEN];
//  uint8_t buflen ;
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (firstTime == true){
    Serial.print("buflen1:");
    Serial.println(buflen);
    firstTime = false;    
  }  
  if( vw_get_message(buf, &buflen) ) {
    Serial.print("buflen:");
    Serial.println(buflen);
    if (buflen == sizeof(msg_t))
    {
      // The right amount of data was received
      memcpy( &msg, buf, sizeof(msg));       // copy the received data into the struct
      
      Serial.print("msg.deviceId: ");
      Serial.println(msg.deviceId);
      Serial.print("msg.msgId: ");
      Serial.println(msg.msgId);

      ack.deviceId = msg.deviceId;
      ack.msgId = msg.msgId; 
       Serial.print("ack.deviceId: ");
      Serial.println(ack.deviceId);
      Serial.print("ack.msgId: ");
      Serial.println(ack.msgId);
      Serial.print("sizeof ack ");
      Serial.println(sizeof(ack));
      vw_send((uint8_t *)&ack, sizeof(ack));
      
      
//      vw_send((uint8_t *)&message, sizeof(message));

      vw_wait_tx();
      
      timeDiff = msg.data1_millisecs - timeLast;
      Serial.print(F("identifier = "));
 //     Serial.println( msg.roomId);
 //     Serial.println( msg.arduinoId);
      Serial.println( msg.sensorId);
      Serial.print("Time diff: ");
      Serial.println( timeDiff);
      Serial.print( "msgId:");
      Serial.println( msg.msgId);
      Serial.println( msg.data2);
      Serial.println( msg.data2_factor);
      float factor = msg.data2_factor;
      if (factor == 0.0){
        factor = 1.0;
      }  
    
      float temp = msg.data2;
      temp = temp / factor;  
      Serial.print( "temp : ");
      Serial.println(temp);
      char temperature[10];  
      dtostrf(temp,4,3,temperature);
      //buildTopic();
//      mqttClient.publish("/home/1/ard1/p2/state",temperature);
//  

      
      String topicStr = buildTopic();
      int topicLength = topicStr.length()+1;
      char topic[topicLength];
      topicStr.toCharArray(topic, topicLength);
      mqttClient.publish(topic,temperature);
      timeLast = msg.data1_millisecs;
    }
     //delay(000);
  }  
  }



String buildTopic(){
 // "/home/1/ard1/p2/state"
// String topic = "/home/" + msg.roomId;
  String topic = "/home/";
  topic = topic + msg.roomId;
 topic = topic + "/ard";
 topic = topic + msg.deviceId;
 topic = topic + "/p";
 topic = topic + msg.sensorId;
 topic = topic + "/state";
 return topic;
}  



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic","hello world");
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



