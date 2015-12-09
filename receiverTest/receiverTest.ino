// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@open.com.au)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
#undef int
#undef abs
#undef double
#undef float
#undef round

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
long randNumber;
long rand2;
int loopNo; 

void setup()
{
    Serial.begin(9600);	// Debugging only
    Serial.println("setup");

    // Initialise the IO and ISR
    //vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_tx_pin(5);
    vw_set_rx_pin(6);
    vw_setup(2000);	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
    randomSeed(analogRead(0));
    randNumber = random(10);
    rand2 = random(10);
    
  Serial.println(randNumber);
}

void loop()
{  
    loopNo++;
    
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
        if (buflen == sizeof(msg_t))
        {
        memcpy( &msg, buf, sizeof(msg));
	int i;

        digitalWrite(13, true); // Flash a light to show received good message
	// Message with a good checksum received, dump it.
	Serial.print("Got: ");
	Serial.print("msg.deviceId:");
        Serial.print(msg.deviceId);
        Serial.print("msg.msgId:");
        Serial.print(msg.msgId);
	Serial.println("");
        digitalWrite(13, false);
        randNumber = random(10);
        ack.deviceId = msg.deviceId;
        if (randNumber >5){
          ack.msgId = msg.msgId+1;
        }  else {
          ack.msgId = msg.msgId;
        }  
        
        rand2 = random(10);
        Serial.print("random:");
        Serial.println(rand2);
        if (rand2 > 3){
          vw_send((uint8_t *)&ack, sizeof(ack));
          vw_wait_tx(); // Wait until the whole message is gone
          
        }  
        
        }
    }
}
