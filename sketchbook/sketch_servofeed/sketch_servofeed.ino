#include <LiquidCrystal.h>
#include <Servo.h>
#include <dht11.h>

LiquidCrystal lcd(4, 5, 6, 7, 11, 12);
Servo motor;
dht11 DHT;

const int INTERVAL_BUTTON_BOUNCE = 150;
const int INTERVAL_FEED_BOUNCE = 245;
const int INTERVAL_SECOND = 1000;

const int PIN_DHT = 3;
const int PIN_MOTOR = 10;
const int PIN_BTN_RIGHT = A5;
const int PIN_SWITCH_DISPLAY = A4;
const int PIN_BTN_LEFT = A3;
const int PIN_AUTOFEED_INDICATOR_LED = 2;
const int DEFAULT_HR_UNTIL_FEED = 12;

const int DLastFeed = 0;
const int DTempHum = 1;
const int DAutoFeed = 2;

bool feeding = false;
bool autofeed = false;
bool buttonDownRight = false;
bool buttonDownLeft = false;
unsigned long previousMillisRight = 0;
unsigned long previousMillisLeft = 0;
unsigned long previousMillisDisplay = 0;
unsigned long previousTimeMillis = 0;
unsigned long feedMillis = 0;
unsigned int lastFeedHour = 8761;
unsigned int lastFeedMinute = 0;
unsigned int lastFeedSecond = 0;

unsigned int currentD = -1;

unsigned int hourUntilNextAuto = DEFAULT_HR_UNTIL_FEED;
unsigned int minutesAutoFeedPassed = 0;
unsigned int secondsPassed = 0;
unsigned int secondsAutoFeedPassed = 0;

void setup() {

  pinMode(PIN_AUTOFEED_INDICATOR_LED, OUTPUT);
  
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_SWITCH_DISPLAY, INPUT_PULLUP);
  pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
  
  motor.attach(PIN_MOTOR);  
  motor.write(0);
  
  lcd.begin(16,2);
  currentD = DTempHum;
  displayLCD();

}

void displayLCD(){

  switch (currentD){
    case DLastFeed:
      displayLastFeed();
      break;
    case DTempHum:
      displayTempHum();
      break;
    case DAutoFeed:
      displayAutofeed();
      break;
    default:
      displayTempHum();
  }
}

void displayLastFeed(){  
  lcd.setCursor(0, 0);
  
  lcd.print("Last feed");
  lcd.setCursor(0, 1);
  if (lastFeedHour > 8760){
     lcd.print("1+ Year AGO");
     return;
  }
  
  if(lastFeedHour < 10) lcd.print("0");
  lcd.print(lastFeedHour);
  lcd.print("h ");
  if(lastFeedMinute < 10) lcd.print("0");
  lcd.print(lastFeedMinute);
  lcd.print("m ");
  if(lastFeedSecond < 10) lcd.print("0");
  lcd.print(lastFeedSecond);
  lcd.print("s");

  lcd.setCursor(12, 1);
  lcd.print("AGO");
  
}

void displayAutofeed(){
  lcd.setCursor(0, 0);
  lcd.print("Autofeed ");
  if (autofeed){
    lcd.print("ON");
    lcd.setCursor(0, 1);
    lcd.print("Next:");
    int s = 3600 - secondsPassed;
    int h = hourUntilNextAuto - ceil(float(secondsPassed) / 3600.0);
    int m = (s / 60 % 60);
    if(h < 10) lcd.print("0");
    lcd.print(h);
    lcd.print("h");
    if(m < 10) lcd.print("0");
    lcd.print(m);
    lcd.print("m");
    int ss = s%60;
    if(ss < 10) lcd.print("0");
    lcd.print(ss);
  }
  else{
    lcd.print("OFF");
  }
}

void displayTempHum(){
  int tempC=DHT.temperature;
  int tempF=(tempC * 9) / 5 + 32;
  int hum=DHT.humidity;
  lcd.setCursor(0,0);
  lcd.print("Tem ");
  lcd.print(tempC);
  lcd.write(0xDF);
  lcd.print("C ");
  lcd.print(tempF);
  lcd.write(0xDF);
  lcd.print("F ");
  
  lcd.setCursor(0,1);
  lcd.print("Hum ");
  lcd.print(hum);
  lcd.print('%');
}

void feederOpen(){
  motor.write(55);      // Turn SG90 servo to open feeder
  feeding = true;
  lastFeedHour = 0;
  lastFeedMinute = 0;
  lastFeedSecond = 0;
  lcd.clear(); 
  displayLCD();
}

void feederClose(){
  motor.write(0);     // Close feed
  feeding = false;
}

void loop() {
  unsigned long currentMillis = millis();

  /*RIGHT BUTTON FN*/
  if(currentMillis - previousMillisRight >= INTERVAL_BUTTON_BOUNCE){
    previousMillisRight = currentMillis;
    if(digitalRead(PIN_BTN_RIGHT) == LOW){
      
      if (currentD == 0){
        feedMillis = currentMillis;
        feederOpen();  
      }
      
      else if(currentD == 2){
        if (!buttonDownRight){
          buttonDownRight = true;
          autofeed = !autofeed;
          
          digitalWrite(PIN_AUTOFEED_INDICATOR_LED, autofeed);
          
          lcd.clear();
          displayLCD();  
        }
      }
    }
    else{
      buttonDownRight = false;  
    }
  }

  /*LEFT BUTTON FN*/
  if(currentMillis - previousMillisLeft >= INTERVAL_BUTTON_BOUNCE){
    previousMillisLeft = currentMillis;
    if(digitalRead(PIN_BTN_LEFT) == LOW){
      if (currentD == 2 && autofeed){
         if (!buttonDownLeft){
            buttonDownLeft = true;
    
            hourUntilNextAuto--;
            secondsPassed = 0;
            if (hourUntilNextAuto <= 0){
              hourUntilNextAuto = DEFAULT_HR_UNTIL_FEED;
            }
            lcd.clear();
            displayLCD();  
            
          }
      }
      
    }
    else{
      buttonDownLeft = false;
    }    
  }

   /*CLOSE FEED*/
  if (feeding && (currentMillis - feedMillis >= INTERVAL_FEED_BOUNCE)){
    feederClose();
  }

  /*DISPLAY SWITCHING*/
  if(currentMillis - previousMillisDisplay >= INTERVAL_BUTTON_BOUNCE){
    previousMillisDisplay = currentMillis;
   
    if(digitalRead(PIN_SWITCH_DISPLAY) == LOW){
      lcd.clear();
      currentD = (currentD + 1) % 3;
      displayLCD();
    }
  }
  
  /*SECONDS COUNTER*/
  if(currentMillis - previousTimeMillis >= INTERVAL_SECOND){
    previousTimeMillis = currentMillis;
    
    
    lastFeedSecond ++;
    if (lastFeedSecond >= 60){
      lastFeedMinute ++;
      if (lastFeedMinute >= 60){
        lastFeedHour ++;
        lastFeedMinute = 0;
      }
      lastFeedSecond = 0;
    }

    if (autofeed) {
      secondsPassed ++;
      if(secondsPassed >= 3600){
        secondsPassed = 0;
        hourUntilNextAuto --;
        if (hourUntilNextAuto <= 0){
          
          feedMillis = millis();
          feederOpen();

          hourUntilNextAuto = DEFAULT_HR_UNTIL_FEED;
        }
      }
    }
    
    DHT.read(PIN_DHT);
    displayLCD();
  }
}
