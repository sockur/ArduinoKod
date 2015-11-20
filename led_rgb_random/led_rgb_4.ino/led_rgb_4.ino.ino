//Program by Samuel. Enjoy!

//If you are using a common anode RGB led, remove the slashes at the next line (the comment)
#define COMMON_ANODE

//Defining pinouts
int redPin = 11; //The pin that red is connected through
int bluePin = 10; //The pin that blue is connected through
int greenPin = 9; //The pin that green is connected through
int red, green, blue;

void setup() {

  //Setting these pins as outputs
  pinMode(redPin, OUTPUT); //Setting red as output
  pinMode(greenPin, OUTPUT); //Setting green as output
  pinMode(bluePin, OUTPUT); //Setting blue as output

}

void loop() {

  red = random(0,255);
  green = random(0,255);
  blue = random(0,255);
  for (int red = 0; red < 255; red++){
      setColor(red, 0, 0); //Red color  
      delay(10);
    }
  for (int green = 0; green < 255; green++){
      setColor(0, green, 0); 
      delay(10);
    }
  for (int blue = 0; blue < 255; blue++){
      setColor(0, 0, blue); //Red color  
      delay(10);
    }

}

void setColor(int redValue, int greenValue, int blueValue) { //Defining function setColor, taking three arguments

#ifdef COMMON_ANODE //Checking if this is defined
  redValue = 255 - redValue;
  greenValue = 255 - greenValue;
  blueValue = 255 - blueValue;
#endif //Ending check

  analogWrite(redPin, redValue); //Setting value red
  analogWrite(greenPin, greenValue); //Setting value green
  analogWrite(bluePin, blueValue); //Setting value blue

}
