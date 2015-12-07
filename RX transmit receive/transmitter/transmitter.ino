    //simple Tx on pin D12
    //Written By : Mohannad Rawashdeh
    // 3:00pm , 13/6/2013
    //http://www.genotronex.com/
    //..................................
    #include <VirtualWire.h>
    String *controller;
    int counter=0;
    
    
    void setup() {
      pinMode(13,OUTPUT);
      vw_set_ptt_inverted(true); //
      vw_set_tx_pin(2);
      vw_setup(4000);// speed of data transfer Kbps
      Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
    }
    
    void loop(){
      counter++;
      controller="1"  ;
      controller.concat(counter) ;
      vw_send((uint8_t *)controller, strlen(controller));
      vw_wait_tx(); // Wait until the whole message is gone
      digitalWrite(13,1);
//      Serial.print(string.concat("On",counter));
      delay(2000);
      controller="0"  ;
      vw_send((uint8_t *)controller, strlen(controller));
      vw_wait_tx(); // Wait until the whole message is gone
      digitalWrite(13,0);
            Serial.print("Off ");
      delay(2000);
    
    }
