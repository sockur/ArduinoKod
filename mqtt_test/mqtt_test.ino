float temp;
int tempPin = 0;

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h>
//#include <LCD.h>
//#include <LiquidCrystal_I2C.h>


// Update these with values suitable for your network.
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
byte server[] = { 192, 168, 1, 121 };
byte ip[]     = { 192, 168, 1, 202 };
char* state  = "ON";

EthernetClient ethClient;
PubSubClient mqttClient(server, 1883, callback, ethClient);

// callback function not used really since we are not subscribing to anything in this example
void callback(char* topic, byte* payload, unsigned int length) {
Serial.println("Callback");
//Serial.print("TopicSerial.println(topic);
//Serial.print("LengthSerial.println(length);
//Serial.print("PayloadSerial.write(payload,length);
Serial.println();
}

void setup()
{

  Serial.begin(9600);
//  Ethernet.begin(mac, ip);
Ethernet.begin(mac,ip);
 delay(1500);
 mqttClient.setServer(server, 1883);
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

void loop()
{
  if (state == "ON"){
    state = "OFF";
    } else {
      state = "ON";
      }
  mqttClient.loop();
  Serial.print(state);
  mqttClient.publish("/home/1/ard1/p1/com/state",state);
  delay(5000);
}
