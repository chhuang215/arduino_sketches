const int MOTOR_PIN1 = 9;
const int MOTOR_PIN2 = 10;
const int OPEN_SW_PIN = 8;
const int CLOSE_SW_PIN = 7;

const int ON_OFF_PIN = 4;

const int OPENING = 1;
const int CLOSING = 0;

int slotState = -1;

int activate = 0;

unsigned long previousMillis = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_PIN1, OUTPUT) ;
  pinMode(MOTOR_PIN2, OUTPUT) ;

  pinMode(OPEN_SW_PIN,  INPUT_PULLUP);
  pinMode(CLOSE_SW_PIN, INPUT_PULLUP);
  
  pinMode(ON_OFF_PIN, INPUT_PULLUP);
}

void openSlot(){
  digitalWrite(MOTOR_PIN1, HIGH);
  digitalWrite(MOTOR_PIN2, LOW);
  slotState = OPENING;
}

void closeSlot(){
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, HIGH);
  slotState = CLOSING;
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned currMillis = millis();

  int openedState = !digitalRead(OPEN_SW_PIN);
  int closedState = !digitalRead(CLOSE_SW_PIN);

  if (currMillis - previousMillis >= 300){
    if (digitalRead(ON_OFF_PIN) == LOW){
        activate = activate ^ 1;
        previousMillis = currMillis;  
    }
  }
  

  if(activate){
    if(openedState && slotState == OPENING){
      // Close the slot if it's opened fully
      closeSlot();
    
    }
    else if (closedState && slotState == CLOSING){
      // Open the slot if it's closed fully
      openSlot();
      
    }
    else if (slotState == -1 && closedState){
      // Close the slot at start if the slot is half way opened/close
      openSlot();
    }
    else if (slotState == -1 && openedState){
      closeSlot();
    }
  }
  else{
    if (!closedState && slotState != CLOSING){
      closeSlot();
    }
    else if (closedState){
      digitalWrite(MOTOR_PIN1, LOW);
      digitalWrite(MOTOR_PIN2, LOW);
      slotState = -1;
    }
  }
}
