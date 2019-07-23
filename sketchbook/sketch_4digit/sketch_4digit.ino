int ASeg = 10;
int BSeg = 13;
int CSeg = 4;
int DSeg = 6;
int ESeg = 7;
int FSeg = 11;
int GSeg = 2;

int a1 = 8;
int a2 = 12;
//int a3 = 12;
//int a4 = 2;
// set variable
long n = 0;
int x = 100;

int hrMinSwitch = 0;

int del = 30; // fine adjustment for clock

int hr, minute, sec;

int startLightHour = 7;
int endLightHour = 0;

const unsigned long ONE_SEC = 1000;

unsigned long startPumpMillis = 5000;
unsigned long stopPumpMillis = 0;
unsigned long previousMillis = -ONE_SEC;
unsigned long previousMillisButtonPress = 0;

void setup()
{
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
//  pinMode(a3, OUTPUT);
//  pinMode(a4, OUTPUT);
  pinMode(ASeg, OUTPUT);
  pinMode(BSeg, OUTPUT);
  pinMode(CSeg, OUTPUT);
  pinMode(DSeg, OUTPUT);
  pinMode(ESeg, OUTPUT);
  pinMode(FSeg, OUTPUT);
  pinMode(GSeg, OUTPUT);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  hr = 20;
  minute = 19;
  sec = 55;

  checkPump();
  checkLight();
//  updateHour();
//  updateMinute();
  setPwmFrequency(9, 1024);
  
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

void checkLight(){
  if((hr >= startLightHour || hr < endLightHour) && digitalRead(A0) == LOW){
    digitalWrite(A0, HIGH);
  }
  else if((hr < startLightHour && hr >= endLightHour) && digitalRead(A0) == HIGH){
    digitalWrite(A0, LOW);
  }
}

void checkPump(){
  if((minute >= 0 && minute < 5) && digitalRead(A1) == LOW){ 
    digitalWrite(A1, HIGH);
  }
  else if(!(minute >= 0 && minute < 5) && digitalRead(A1) == HIGH){
    digitalWrite(A1, LOW);
  }
}

void checkMotorAndFan(){
  if(minute >= 0 && minute < 10 && minute >= 30 && minute < 40)  {
    analogWrite(9, 127);    
    digitalWrite(A2, HIGH);
  }
  else{
    analogWrite(9, 0);
    digitalWrite(A2, LOW);
  }
}

void loop()
{
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillisButtonPress >= 250){
    previousMillisButtonPress = currentMillis;
    if(digitalRead(A3) == LOW){
      hrMinSwitch = 1;
      hr = ++hr % 24;  
      checkLight();
      checkPump();
      checkMotorAndFan();
      sec = 0;
    }
    if(digitalRead(A4) == LOW){
      hrMinSwitch = 5;
      minute = ++minute % 60;
      checkPump(); 
      checkMotorAndFan(); 
      if(minute == 0){
        hr = ++hr % 24;
        checkLight();
      }
      sec = 0;
    }
    if(digitalRead(A5) == LOW){
      hrMinSwitch = 5;
      minute = --minute % 60;  
      checkPump();
      checkMotorAndFan();
      if(minute < 0){
        hr = --hr % 24;
        checkLight();
        minute += 60;
      }
      sec = 0;
    }
  }

  // INC SECOND
  if(currentMillis - previousMillis >= ONE_SEC){
    previousMillis = currentMillis;
    sec = ++sec % 60;
    if(sec == 0){
      minute = ++minute % 60;
      checkPump();
      checkMotorAndFan();
      if(minute == 0){
        hr = ++hr % 24;
        checkLight();
     
      }
    }

    hrMinSwitch += 1 ;
    hrMinSwitch = hrMinSwitch % 8;

  }
  //if(currentMillis - previousMillisDigitUpdate >= 50){
   // previousMillisDigitUpdate = currentMillis;
  clearLEDS();
  if (hrMinSwitch >= 0 && hrMinSwitch < 1){
    pickDigit(1);  // Display position 1
    digitalWrite(ASeg, HIGH);
    digitalWrite(BSeg, LOW);
    digitalWrite(CSeg, LOW);
    digitalWrite(DSeg, HIGH);
    digitalWrite(ESeg, LOW);
    digitalWrite(FSeg, LOW);
    digitalWrite(GSeg, LOW);

  }
  else if(hrMinSwitch >= 1 && hrMinSwitch <= 3){
    
    pickDigit(1);  // Display position 1
    pickNumber(hr/10); // Display number
    delayMicroseconds(del); 
    clearLEDS();
    pickDigit(2);
    pickNumber(hr%10);
  

    delayMicroseconds(del); 
  }
  else if(hrMinSwitch >= 4 && hrMinSwitch < 5){
    pickDigit(1);  // Display position 1
    digitalWrite(ASeg, LOW);
    digitalWrite(BSeg, LOW);
    digitalWrite(CSeg, HIGH);
    digitalWrite(DSeg, HIGH);
    digitalWrite(ESeg, HIGH);
    digitalWrite(FSeg, LOW);
    digitalWrite(GSeg, HIGH);
  }
  else{
    pickDigit(1);  // Display position 1
    pickNumber(minute/10);
    delayMicroseconds(del); 
    clearLEDS();
    pickDigit(2);
    pickNumber(minute%10);
    delayMicroseconds(del); 
  }



//  pickDigit(1);  // Display position 1
//  pickNumber(sec/10); // Display number
//  delayMicroseconds(del); 
//  clearLEDS();
//  pickDigit(2);
//  pickNumber(sec%10);
//  delayMicroseconds(del);
    
//
//    clearLEDS();
//    pickDigit(3);
//    pickNumber(minute/10);
//    delayMicroseconds(del); 
//    clearLEDS();
//    pickDigit(4);
//    pickNumber(minute%10);
//    delayMicroseconds(del); 
  //}
}
// Select display position
void pickDigit(int x) 
{
  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
//  digitalWrite(a3, LOW);
//  digitalWrite(a4, LOW);

  switch(x)
  {
  case 1:
    digitalWrite(a1, HIGH);
    break;
  case 2:
    digitalWrite(a2, HIGH);
    break;
//  case 3:
//    digitalWrite(a3, HIGH);
//    break;
//  case 4:
//    digitalWrite(a4, HIGH);
//    break;  
  }
}
// select display number 
void pickNumber(int x) 
 {
   switch(x) {
    
   case 1: one(); break;
   case 2: two(); break;
   case 3: three(); break;
   case 4: four(); break;
   case 5: five(); break;
   case 6: six(); break;
   case 7: seven(); break;
   case 8: eight(); break;
   case 9: nine(); break;
   default: zero(); break;
 }
}

void clearLEDS()  // clear the screen
{
   digitalWrite(ASeg, HIGH);
   digitalWrite(BSeg, HIGH);
   digitalWrite(CSeg, HIGH);
   digitalWrite(DSeg, HIGH);
   digitalWrite(ESeg, HIGH);
   digitalWrite(FSeg, HIGH);
   digitalWrite(GSeg, HIGH);
}

//Display function ‘0-9’
void zero() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, LOW);
  digitalWrite(ESeg, LOW);
  digitalWrite(FSeg, LOW);
  digitalWrite(GSeg, HIGH);
}

void one() { 
  digitalWrite(ASeg, HIGH);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, HIGH);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, HIGH);
}

void two() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, HIGH);
  digitalWrite(DSeg, LOW);
  digitalWrite(ESeg, LOW);
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, LOW);
}

void three() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, LOW);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, LOW);
}
void four() {
  digitalWrite(ASeg, HIGH);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, HIGH);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, LOW);
  digitalWrite(GSeg, LOW);
}
void five() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, HIGH);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, LOW);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, LOW);
  digitalWrite(GSeg, LOW);
}
void six() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, HIGH);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, LOW);
  digitalWrite(ESeg, LOW);
  digitalWrite(FSeg, LOW);
  digitalWrite(GSeg, LOW);
}

void seven() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, HIGH);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, HIGH);
  digitalWrite(GSeg, HIGH);
}

void eight() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, LOW);
  digitalWrite(ESeg, LOW);
  digitalWrite(FSeg, LOW);
  digitalWrite(GSeg, LOW);
}

void nine() {
  digitalWrite(ASeg, LOW);
  digitalWrite(BSeg, LOW);
  digitalWrite(CSeg, LOW);
  digitalWrite(DSeg, LOW);
  digitalWrite(ESeg, HIGH);
  digitalWrite(FSeg, LOW);
  digitalWrite(GSeg, LOW);
}

