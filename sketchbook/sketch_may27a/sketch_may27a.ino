#include <Servo.h>
Servo myservo; // create servo object to control a servo
  // twelve servo objects can be created on most boards
int pos = 0;
void setup() {

  myservo.attach(9); // attaches the servo on pin 9 to the servo object
  myservo.write(pos);
}
void loop() {

  myservo.write(180); 
  delay(100);

  for(int i = 180; i > 0; i--){
    myservo.write(i);  
    delay(20);
  }

  delay(100);

  exit(0);
  
}
