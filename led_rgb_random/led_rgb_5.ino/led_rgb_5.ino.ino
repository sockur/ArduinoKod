//Program by Samuel. Enjoy!

//If you are using a common anode RGB led, remove the slashes at the next line (the comment)
#define COMMON_ANODE

//Defining pinouts
int redPin = 11; //The pin that red is connected through
int bluePin = 9; //The pin that blue is connected through
int greenPin = 10; //The pin that green is connected through
int red, green, blue;
int redDiff, greenDiff, blueDiff;

void setup() {

  //Setting these pins as outputs
  pinMode(redPin, OUTPUT); //Setting red as output
  pinMode(greenPin, OUTPUT); //Setting green as output
  pinMode(bluePin, OUTPUT); //Setting blue as output
  red = 0;
  green = 0;
  blue = 0;
  redDiff = 1;
  greenDiff = 2;
  blueDiff = 3;
}

void loop() {

  red = red + redDiff;
  green = green + greenDiff;
  blue = blue + blueDiff;
  if (red >= 255){
     red = 255;
     redDiff = redDiff *-1; 
  }
  if (red < 0){
      red = 0;
     redDiff = redDiff *-1; 
  }

  if (green >= 255){
     green = 255;
     greenDiff = greenDiff *-1; 
  }
  if (green < 0){
      green = 0;
     greenDiff = greenDiff *-1; 
  }

  if (blue >= 255){
     blue = 255;
     blueDiff = blueDiff *-1; 
  }
  if (blue < 0){
      blue = 0;
     blueDiff = blueDiff *-1; 
  }
 
 setColor(red,green,blue);
 delay(50);
/* setColor(0,0,0);
 delay(50); */
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
