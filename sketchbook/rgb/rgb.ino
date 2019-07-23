#define FADESPEED 7

const int REDPIN = 9;    // LED connected to digital pin 9
const int GREENPIN = 10;     // LED connected to digital pin 10
const int BLUEPIN = 11;    // LED connected to digital pin 11

void setup() {
  // set the digital pins as outputs
  pinMode(GREENPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
}

void loop() {

  for (int r = 10; r < 255; r++) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade from violet to red
  for (int b = 255; b > 10; b--) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from red to yellow
  for (int g = 10; g < 255; g++) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
  // fade from yellow to green
  for (int r = 255; r > 10; r--) { 
    analogWrite(REDPIN, r);
    delay(FADESPEED);
  } 
  // fade from green to teal
  for (int b = 10; b < 255; b++) { 
    analogWrite(BLUEPIN, b);
    delay(FADESPEED);
  } 
  // fade from teal to blue
  for (int g = 255; g > 10; g--) { 
    analogWrite(GREENPIN, g);
    delay(FADESPEED);
  } 
}

