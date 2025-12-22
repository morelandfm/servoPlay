#include <SCServo.h>

SMS_STS st;

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
int servoId;
int Pos;
int startPos = 1500;
int counter = 1;
int long buttonPressStartTime = 0;
bool held = false;

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
  Pos = st.ReadPos(servoId);
  //Setting the initial position of all the servos to relative middle
  Serial.print("X: ");
  Serial.print(xVal);
  Serial.print(" | Y: ");
  Serial.print(yVal);
  Serial.print(" | Pos: ");
  Serial.print(Pos);
  Serial.print(" | ServoId: ");
  Serial.println(servoId);
  displayServo(servoId);
  if(counter == 1){
  //Setting the initial position of all the servos to relative middle
    st.WritePosEx(1, 2070, 500, 25);
    st.WritePosEx(2, 1500, 500, 125);
    st.WritePosEx(3, 2500, 500, 125);
    st.WritePosEx(4, 2000, 500, 125);
    st.WritePosEx(5, 2050, 500, 25);
    st.WritePosEx(6, 2200, 500, 25);
    delay(100);
  }

//New stuff for trying to use button held to change the servo and debouncing
  if (xVal < 150 || xVal > 950 && !held) {
    buttonPressStartTime = millis();
  }
  if(millis() - buttonPressStartTime >= 350){
    held = true;
  }
  if (xVal < 150 && held) {
    servoId = servoId - 1;
    held = false;
  } else {
    if (xVal > 900 && held) {
      servoId = servoId + 1;
      held = false;
    }
  }
  if (servoId > 6) {
    servoId = 1;
  } else {
    if (servoId == 0) {
      servoId = 6;
    }
  }
  //Setting individual parameters for each servo
  if (yVal < 150 && servoId == 1) {
    st.WritePosEx(servoId, Pos - 10, 500, 25);
  } else if (yVal > 900 && servoId == 1) {
    st.WritePosEx(servoId, Pos + 10, 500, 25);
  } else if (yVal < 150 && servoId == 5) {
    st.WritePosEx(servoId, Pos - 10, 500, 25);
  } else if (yVal > 900 && servoId == 5) {
    st.WritePosEx(servoId, Pos + 10, 500, 25);
  } else if (yVal < 150 && servoId == 6) {
    st.WritePosEx(servoId, Pos - 10, 500, 25);
  } else if (yVal > 900 && servoId == 6) {
    st.WritePosEx(servoId, Pos + 10, 500, 25);
  } else if (yVal < 150 && servoId == 2) {
    st.WritePosEx(servoId, Pos - 10, 500, 125);
  } else if (yVal > 900 && servoId == 2) {
    st.WritePosEx(servoId, Pos + 10, 500, 125);
  } else if (yVal < 150 && servoId == 3) {
    st.WritePosEx(servoId, Pos - 10, 500, 125);
  } else if (yVal > 900 && servoId == 3) {
    st.WritePosEx(servoId, Pos + 10, 500, 125);
  } else if (yVal < 150 && servoId == 4) {
    st.WritePosEx(servoId, Pos - 10, 500, 125);
  } else if (yVal > 900 && servoId == 4) {
    st.WritePosEx(servoId, Pos + 10, 500, 125);
  }  
  counter = 2;
}
