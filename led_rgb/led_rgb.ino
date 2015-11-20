//Program by Samuel. Enjoy!

//If you are using a common anode RGB led, remove the slashes at the next line (the comment)
#define COMMON_ANODE

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

  setColor(255, 0, 0); //Red color
  delay(1000);
  setColor(0, 255, 0); //Green color
  delay(1000);
  setColor(0, 0, 255); //Blue color
  delay(1000);

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
