#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <VirtualWire.h>

// Update these with values suitable for your network.
//byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte mac[]    = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
char* state  = "ON";

IPAddress ip(192, 168, 1, 202);
IPAddress server(192, 168, 1, 121);
IPAddress dns_server(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
 
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
 
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
 
void setup()
{
  Serial.begin(57600);
 
  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(callback);
 
  if (Ethernet.begin(mac) == 0){
      Serial.println("Failed to configure Ethernet using DHCP");
      return;
  }
//  Ethernet.begin(mac, ip, dns_server, gateway, subnet);
  // Allow the hardware to sort itself out
  delay(1500);
  vw_setup(2000);
  vw_rx_start();
  vw_set_rx_pin(10);
}
 
void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
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

