#include <Adafruit_NeoPixel.h>
#include <NewPing.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif
#define trigPin  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define echoPin     13  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define PIN 6
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);
NewPing sonar(trigPin, echoPin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
long duration, cm, inches;
 
void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
//  strip.begin();
//  strip.show(); // Initialize all pixels to 'off'
  //randomSeed(analogRead(A0));   
  
}

void loop() {
delay(50);
   unsigned int uS = sonar.ping_cm();
   if (uS > 0 && uS <= MAX_DISTANCE ){
     Serial.print(uS);
   Serial.println("cm");  
     } else {
          Serial.println("fail ...........");  
       }
   

  

  
//  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).

 
  
  
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 100); // Red
  //colorWipe(strip.Color(0, 255, 0), 10); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
 //theaterChase(strip.Color(127, 127, 127), 50); // White
// theaterChase(strip.Color(127, 0, 0), 50); // Red
//theaterChase(strip.Color(0, 0, 127), 50); // Blue

  //rainbow(20);
//rainbowCycle(20);
  //theaterChaseRainbow(50);
  
//    delay(500);
}



