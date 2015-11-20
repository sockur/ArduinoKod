//Program by Samuel. Enjoy!

//If you are using a common anode RGB led, remove the slashes at the next line (the comment)
//#define COMMON_ANODE

//Defining pinouts
int redPin = 11; //The pin that red is connected through
int bluePin = 10; //The pin that blue is connected through
int greenPin = 9; //The pin that green is connected through

void setup() {

  //Setting these pins as outputs
  pinMode(redPin, OUTPUT); //Setting red as output
  pinMode(greenPin, OUTPUT); //Setting green as output
  pinMode(bluePin, OUTPUT); //Setting blue as output

}

void loop() {
  analogWrite(redPin, 255); //Setting value red
  
  delay(1000);
    analogWrite(greenPin, 0); //Setting value green  
  delay(1000);
  analogWrite(bluePin, 0); //Setting value blue  
  delay(1000);

}

void setColor(int redValue, int greenValue, int blueValue) { //Defining function setColor, taking three arguments

#ifdef COMMON_ANODE //Checking if this is defined
  red = 255 - redValue;
  green = 255 - greenValue;
  blue = 255 - blueValue;
#endif //Ending check

  



}
