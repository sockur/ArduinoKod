#include <VirtualWire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
//byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte mac[]    = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
char* state  = "ON";

IPAddress ip(192, 168, 1, 202);
IPAddress server(192, 168, 1, 121);
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

float temp;

int i;

typedef struct {		
  int           roomId; 		//node ID (1xx, 2xx, 3xx);  1xx = basement, 2xx = main floor, 3xx = outside
  int		arduinoId;		//sensor ID (2, 3, 4, 5)
  int           sensorId;
  unsigned long data1_millisecs; 		//uptime in ms
  int           data2;   	        //sensor data?
  int           data2_factor;
  int		data3;		//battery condition?
  int           data3_factor;
} MessageStruct;
MessageStruct msg;



void setup(){
   Serial.begin(9600);
   vw_setup(2000);  
   vw_rx_start();
   vw_set_tx_pin(2);
   vw_set_rx_pin(3);
   mqttClient.setServer(server, 1883);
   mqttClient.setCallback(callback);
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
  uint8_t buflen ;

  if( vw_get_message(buf, &buflen) ) {
    Serial.println( (char*) buf);
    if (buflen == sizeof(MessageStruct))
    {
      // The right amount of data was received
      memcpy( &msg, buf, sizeof(msg));       // copy the received data into the struct

      Serial.print(F("identifier = "));
      Serial.println( msg.roomId);
      Serial.println( msg.arduinoId);
      Serial.println( msg.sensorId);
      Serial.println( msg.data2);
      Serial.println( msg.data2_factor);
    }
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
    mqttClient.publish("/home/1/ard1/p2/state",temperature);
     // delay(60000);
  }  
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



