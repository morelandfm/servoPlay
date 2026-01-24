#include <SCServo.h>

SMS_STS st;

/*
Latest Test:
In good working order
*/

/*
Future Steps:
*/

void display1(void) {
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
}

void display2(void) {
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
}

void display3(void) {
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}

void display4(void) {
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
}

void display5(void) {
  digitalWrite(2, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}

void display6(void) {
  digitalWrite(2, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
}

void display0(void) {
  digitalWrite(2, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(1, HIGH);
}

void displayServo(int x) {
  if (x == 1) {
    clearDisplay();
    display1();
  }
  if (x == 2) {
    clearDisplay();
    display2();
  }
  if (x == 3) {
    clearDisplay();
    display3();
  }
  if (x == 4) {
    clearDisplay();
    display4();
  }
  if (x == 5) {
    clearDisplay();
    display5();
  }
  if (x == 6) {
    clearDisplay();
    display6();
  }
}

//Function for resetting the position of each of the servos
void reset(void){
    st.WritePosEx(1, 2070, 500, 25);
    st.WritePosEx(2, 1500, 500, 125);
    st.WritePosEx(3, 2500, 500, 125);
    st.WritePosEx(4, 2000, 500, 125);
    st.WritePosEx(5, 2050, 500, 25);
    st.WritePosEx(6, 2200, 500, 25);
    delay(1000);
}

//Function for angle jump
void angleJump(void){
    st.WritePosEx(1, 2070, 500, 25);
    st.WritePosEx(2, 1500, 500, 25);
    st.WritePosEx(3, 2000, 500, 25);
    st.WritePosEx(4, 1500, 500, 25);
    st.WritePosEx(5, 2050, 500, 25);
    st.WritePosEx(6, 2200, 500, 25);
    delay(1000);
}
//Function for stopping a servo
int stop(int x) {
  int cPos = st.ReadPos(x); 
  //Changed acc and vel values to zero
  st.WritePosEx(x, cPos, 0, 0);
  return cPos;
}

//Function for moving the servo
void moveServo(int x, int y, int velocity){
  if(y > 900){
    st.WritePosEx(x, 4000, velocity, 25);
  }
  else if(y < 150){
    st.WritePosEx(x, 0, velocity, 25);
  }

  //Drastically changed acc and vel values
  //For the future could write if statements here for varries acc and vel servo values
  //The idea here is that as long as the joystick is held in one direction or the other the servo will move in that direction until the joystick goes back to the dead zone
}

void clearDisplay(void) {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}
//Seven segment display pinout for lighting
//a=2(top),b=3(topRight),c=4(botRight),d=5(bot),e=6(botLeft),f=7(topLeft),g=8(mid)


int xPin = A0;
int yPin = A1;
int buttonPin = 9;
int xVal;
int yVal;
int buttonState;
int servoId = 1;
int long buttonPressStartTime = 0;
bool held = false;
bool timing = false;
bool bTiming = false;
bool triggered = false;
bool movedServo = false;
int Pos;
int velocity[] = {1000, 1000, 800, 650, 650, 300};
int mapPos;
int curPos;


byte ID[6];
s16 Position[6];
u16 Speed[6];
byte ACC[6];

void setup() {
  Serial.begin(115200);
  Serial1.begin(1000000, SERIAL_8N1);
  st.pSerial = &Serial1;
  //Sick way to do the same thing that was done in the commented
  //out code from below
  for (int i = 2; i <= 8; i++)
    pinMode(i, OUTPUT);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  //Setting the initial position of all the servos to relative middle
  //This is being conducted in the setup portion of the code, keep delay
  st.WritePosEx(1, 2070, 500, 25);
  st.WritePosEx(2, 1500, 500, 25);
  st.WritePosEx(3, 2500, 500, 25);
  st.WritePosEx(4, 2000, 500, 25);
  st.WritePosEx(5, 2050, 500, 25);
  st.WritePosEx(6, 2200, 500, 25);
  delay(1000);
}

void loop() {
  xVal = analogRead(xPin);
  yVal = analogRead(yPin);
  buttonState = digitalRead(buttonPin);
  Serial.print("X: ");
  Serial.print(xVal);
  Serial.print(" | Y: ");
  Serial.print(yVal);
  Serial.print(" | Pos: ");
  Serial.print(curPos);
  Serial.print(" | Held: ");
  Serial.print(held ? "true" : "false");
  Serial.print(" | ServoId: ");
  Serial.print(servoId);
  Serial.print(" | ButtonState: ");
  Serial.println(buttonState);
  displayServo(servoId);

  //Three if statements for using the button to reset or anglejump the servos
  //Buttonstate is 1 for not pressed, 0 for pressed
  /*if(buttonState == 0 && !bTiming){
    buttonPressStartTime = millis();
    bTiming = true;
  }
  if(millis() - buttonPressStartTime >= 250 && bTiming){
    //reset();
  }*/
  if(buttonState == 0){
  reset();
  bTiming = false;
  }

  /*
  //Added in the event that the joystick returns to the center before the
  //required amount of time to change the servo
  if (xVal >= 150 && xVal <= 950) {
    held = false;
    timing = false;
    triggered = false;
  }
  else if (!timing) {
    buttonPressStartTime = millis();
    timing = true;
  }

  if(millis() - buttonPressStartTime >= 350 && timing){
    held = true;

    if (!triggered) {
      triggered = true;
      if (xVal < 150) {
        servoId--;
        if (servoId == 0)
          servoId = 6;
      } 
      else if (xVal > 950) {
          servoId++;
          if (servoId > 6)
            servoId = 1;
      }
    }
  }*/
  
  if ((xVal < 150 || xVal > 950) && !timing) {
    buttonPressStartTime = millis();
    timing = true;
  }

  if(millis() - buttonPressStartTime >= 350 && timing){
    held = true;
  }

  //Added in the event that the joystick returns to the center before the
  //required amount of time to change the servo
  if (xVal >= 150 && xVal <= 950) {
    held = false;
    timing = false;
    triggered = false;
  }
  
  if (xVal < 150 && held && !triggered) {
    servoId = servoId - 1;
    triggered = true;
  } else {
    if (xVal > 950 && held && !triggered) {
      servoId = servoId + 1;
      triggered = true;
    }
  }
  if (servoId == 7) {
    servoId = 1;
  } else {
    if (servoId == 0) {
      servoId = 6;
    }
  }
  //Mapping 0:4000 from the servo to 0:1048 on the joystick y axis
  mapPos = (yVal, 0, 4000, 0, 1048);
  //Getting the current position of the chosen servo for printing purposes
  //May need to be moved
  curPos = st.ReadPos(servoId);
  if (yVal > 900 || yVal < 150){
    if (movedServo == false){

    movedServo = true;
    moveServo(servoId, yVal, velocity[servoId - 1]);
    }
  } 
  else if (movedServo)
  {
    movedServo = false;
    stop(servoId);
  }
}