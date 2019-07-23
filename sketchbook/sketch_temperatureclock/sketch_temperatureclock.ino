 /*
 * Author: SMRAZA KEEN
 * Date:2016/6/29
 * IDE V1.6.9
 * Email:TechnicSmraza@outlook.com
 * Function:
 */
// initialize the library with the numbers of the interface pins
#include <LiquidCrystal.h>
#include <dht11.h>
//LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
dht11 DHT;
const int DHT_PIN = 3;
const int LCD_BACKLIGHT_PIN = 9;
const int PUSH_BUTTON_PIN = 2;
const int INTERVAL_SECOND = 1000;
const int INTERVAL_UPDATE_TEMP = 4000;
const int INTERVAL_BUTTON_BOUNCE = 150;
unsigned long previousMillis[2] = {0, 0};
unsigned long previousTimeInMillies[1] = {0};  

bool buttonDown = false;

int backlightState = 1;

int hr, minute, sec;

void setup()                                                                                     
{
  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);

  pinMode(A5, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);

  hr = 2;
  minute = 50;
  sec = 0;
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);

  // Print a message to the LCD.
  displayTime();
  displayTemperatureLabels();
  displayTemperature();

  // Set lcd backlight on
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
}

void displayTemperatureLabels(){
  //lcd.setCursor(0,0);  //Display position
  
  lcd.setCursor(0,1);  //Display position
  lcd.print("Tem      Hum");

}

void displayTime(){  
  lcd.setCursor(4, 0);
  if(hr < 10) lcd.print("0");
  lcd.print(hr);
  lcd.print(":");
  if(minute < 10) lcd.print("0");
  lcd.print(minute);
  lcd.print(":");
  if(sec < 10) lcd.print("0");
  lcd.print(sec);
  
}

void displayTemperature(){
  DHT.read(DHT_PIN);
  int temp=DHT.temperature;
  int hum=DHT.humidity;
  lcd.setCursor(4,1);
  lcd.print(temp);
  lcd.write(0xDF);
  lcd.print("C");
  
  lcd.setCursor(13,1) ;
  lcd.print(hum);
  lcd.print('%');
}

void loop()
{
  unsigned long currentMillis = millis();
 
  // Button Trigger
  if(currentMillis - previousMillis[1] >= INTERVAL_BUTTON_BOUNCE){
    previousMillis[1] = currentMillis;
    if(digitalRead(PUSH_BUTTON_PIN) == LOW){
      if(buttonDown == false){  
        buttonDown = true;
        backlightState = backlightState ^ 1;
  
        lcd.clear();      
        if(backlightState == 0){ 
          lcd.setCursor(4,0);
          lcd.print("OFF");
          digitalWrite(LCD_BACKLIGHT_PIN, LOW);
        }
        else{
          displayTime();
          displayTemperatureLabels();
          displayTemperature();
          digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
        }
        
      }
      
    }
    else if(digitalRead(A5) == LOW){
      previousTimeInMillies[0] = currentMillis;
      hr = ++hr % 24;  
      sec = 0;
      displayTime();
    }
    else if(digitalRead(A4) == LOW){
      previousTimeInMillies[0] = currentMillis;
      minute = ++minute % 60;  
      if(minute == 0){
        hr = ++hr % 24;
      }
      sec = 0;
      displayTime();
      
    }
    else if(digitalRead(A3) == LOW){
      previousTimeInMillies[0] = currentMillis;
      minute = --minute % 60;  
      if(minute < 0){
        hr = --hr % 24;
        minute += 60;
      }
      sec = 0;
      displayTime();
      
    }
    else if(buttonDown){
      buttonDown = false;
    }
  }
 
  
  
  if(currentMillis - previousMillis[0] >= INTERVAL_UPDATE_TEMP && backlightState == 1){
    previousMillis[0] = currentMillis;
    displayTemperature();
    
  }

  if(currentMillis - previousTimeInMillies[0] >= INTERVAL_SECOND){
   previousTimeInMillies[0] = currentMillis;
   sec = ++sec % 60;
   if(sec == 0){
    minute = ++minute % 60;
    if(minute == 0){
      hr = ++hr % 24;
    }
   }
   if(backlightState == 1) displayTime();
  }
  
//  delay(1000);
}

