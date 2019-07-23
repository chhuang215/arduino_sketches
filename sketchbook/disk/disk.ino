const int MOTOR_PIN1 = 9;
const int MOTOR_PIN2 = 10;
const int OPEN_SW_PIN = 8;
const int CLOSE_SW_PIN = 7;

const int OPEN_CLOSE_PIN = 4;

const int OPENED = 3;
const int CLOSED = 2;
const int OPENING = 1;
const int CLOSING = 0;

int slotState = -1;

int activate = 0;

unsigned long previousMillis = 0;
void setup() {
  // put your setup code here, to run once:
//  Serial.begin(9600);
  pinMode(MOTOR_PIN1, OUTPUT) ;
  pinMode(MOTOR_PIN2, OUTPUT) ;

  pinMode(OPEN_SW_PIN,  INPUT_PULLUP);
  pinMode(CLOSE_SW_PIN, INPUT_PULLUP);
  
  pinMode(OPEN_CLOSE_PIN, INPUT_PULLUP);
}

void openSlot(){
  if (slotState == OPENING || slotState == OPENED) return;
  digitalWrite(MOTOR_PIN1, HIGH);
  digitalWrite(MOTOR_PIN2, LOW);
  slotState = OPENING;
//  Serial.print("opening\n");
}

void closeSlot(){
  if (slotState == CLOSING || slotState == CLOSED) return;
  
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, HIGH);
  slotState = CLOSING;  
//  Serial.print("closing\n");
}

void idleSlot (){
  if (slotState == OPENED || slotState == CLOSED) return;
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
//  Serial.print("idle\n");
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned currMillis = millis();

  int openedState = !digitalRead(OPEN_SW_PIN);
  int closedState = !digitalRead(CLOSE_SW_PIN);



  if (currMillis - previousMillis >= 300){
    
    if (digitalRead(OPEN_CLOSE_PIN) == LOW){
      if(slotState == OPENED){
        closeSlot();
      }
      else if (slotState == CLOSED){
        openSlot();
      }
      
      previousMillis = currMillis;
      
    }
    else{
      if (openedState){
        
        idleSlot();
        slotState = OPENED;
      }
    
      if (closedState){
        
        idleSlot();
        slotState = CLOSED;
      }
      
    }
  }

  
  

  if (!openedState && !closedState){
    if (slotState == OPENED){
      closeSlot();  
    }  

    if (slotState == CLOSED){
      openSlot();  
    }
  }
}
