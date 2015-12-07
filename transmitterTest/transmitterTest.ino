// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@open.com.au)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
#undef int
#undef abs
#undef double
#undef float
#undef round
int msgid = 0; 

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

    // Initialise the IO and ISR
    //vw_set_ptt_inverted(true); // Required for DR3100

    vw_set_tx_pin(3);
    vw_set_rx_pin(4);
    vw_setup(2000);	 // Bits per sec
    vw_rx_start();
}

void loop()
{
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
    
    uint8_t buf2[VW_MAX_MESSAGE_LEN];
    uint8_t buflen2 = VW_MAX_MESSAGE_LEN;
    
    // Wait 100 ms for an ack
    vw_wait_rx_max(100);
    if (vw_get_message(buf2, &buflen2)) // Non-blocking
    {
            Serial.println("Gor msg");
    }  
    if (buflen2 == sizeof(ack_t)){
          memcpy( &ack, buf2, sizeof(ack));    
         
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
    delay(2000);
    
}
