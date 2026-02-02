#include <Bluepad32.h>
#include <SCServo.h>

SMS_STS st;
/*
Latest Test:
Velocity values aren't changing correctly, could be an issue with the reset function setting values incorrectly
Right thumbstick works, still need to widen the value threshold so two servos aren't moving at the same time so frequently
Dpad is backwards with the symbols: triangle, square for servo2, circle and x for servo1
*/

/*
Next Test:
Make sure that the threshold changes are suitable for both thumbsticks
Try and check speed changes, might need to print some thnings for this one
Added trailing zeros for dpad controls, might have been contributing to the reference issues, if this doesn't work might need to relook at "controller" to double check they are being mapped correctly

*/

/*
Future Steps:
Switch symbols to dpad
Fix speed up and slow down
*/
//Below required
bool movedServo = false;
//Added for tracking current speed
int curSpeed = 600;
bool movedServo1 = false;
bool movedServo2 = false;
bool movedServo3 = false;
bool movedServo4 = false;
bool movedServo5 = false;
bool movedServo6 = false;

byte ID[6];
s16 Position[6];
u16 Speed[6];
byte ACC[6];

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
      }
    }

    if (!foundEmptySlot) {
      Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

    if (!foundController) {
      Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

// ========= SEE CONTROLLER VALUES IN SERIAL MONITOR ========= //

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
        "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d curSpeed: \n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->gyroX(),        // Gyro X
        ctl->gyroY(),        // Gyro Y
        ctl->gyroZ(),        // Gyro Z
        ctl->accelX(),       // Accelerometer X
        ctl->accelY(),       // Accelerometer Y
        ctl->accelZ(),        // Accelerometer Z
        curSpeed
    );
}

//Imported function from previous code
int stop(int x) {
  int cPos = st.ReadPos(x); 
  //Changed acc and vel values to zero
  st.WritePosEx(x, cPos, 0, 0);
  return cPos;
}

void reset(void){
    st.WritePosEx(1, 2070, curSpeed, 25);
    st.WritePosEx(2, 1500, curSpeed, 25);
    st.WritePosEx(3, 2500, curSpeed, 25);
    st.WritePosEx(4, 2000, curSpeed, 25);
    st.WritePosEx(5, 2050, curSpeed, 25);
    st.WritePosEx(6, 2200, curSpeed, 25);
    delay(1000);
}

//NOT SURE IF NECESSARY
void moveServo(int x, int y, int velocity){
  if(y > 900){
    st.WritePosEx(x, 4000, velocity, 25);
  }
  else if(y < 150){
    st.WritePosEx(x, 0, velocity, 25);
  }
}

// ========= GAME CONTROLLER ACTIONS SECTION ========= //

void processGamepad(ControllerPtr ctl) {
  // There are different ways to query whether a button is pressed.
  // By query each button individually:
  //  a(), b(), x(), y(), l1(), etc...
 
  //== PS4 X button = 0x0001 ==//
  if (ctl->buttons() == 0x0001) {
    // code for when X button is pushed
  }
  if (ctl->buttons() != 0x0001) {
    // code for when X button is released
  }

  //== PS4 Square button = 0x0004 ==//
  if (ctl->buttons() == 0x0004) {
    // code for when square button is pushed
  }
  if (ctl->buttons() != 0x0004) {
  // code for when square button is released
  }

  //== PS4 Triangle button = 0x0008 ==//
  if (ctl->buttons() == 0x0008) {
    // code for when triangle button is pushed
  }
  if (ctl->buttons() != 0x0008) {
    // code for when triangle button is released
  }

  //== PS4 Circle button = 0x0002 ==//
  if (ctl->buttons() == 0x0002) {
    // code for when circle button is pushed
  }
  if (ctl->buttons() != 0x0002) {
    // code for when circle button is released
  }

  //== PS4 Dpad UP and DOWN button = 0x01 ==//
  //Controlling function added
  //Double check that this is the correct direction for the controlling of the servos
  if (ctl->buttons() == 0x01) {
    st.WritePosEx(1, 0, curSpeed, 25);
    movedServo1 = true;
  } else if (ctl->buttons() == 0x02) {
    st.WritePosEx(1, 4000, curSpeed, 25);
    movedServo1 = true;
  } 
  if(movedServo1 && ctl->buttons() != 0x01 && ctl->buttons() != 0x02){
    stop(1);
    movedServo1 = false;
  }

  //== PS4 Dpad LEFT and RIGHT button = 0x08 ==//
  //Controlling function added
  if (ctl->buttons() == 0x08) {
    st.WritePosEx(2, 4000, curSpeed, 25);
    movedServo2 = true;
  } else if (ctl->buttons() == 0x04) {
    st.WritePosEx(2, 0, curSpeed, 25);
    movedServo2 = true;
  }  
  if(movedServo2 && ctl->buttons() != 0x08 && ctl->buttons() != 0x04){
    stop(2);
    movedServo2 = false;
  }

  //== PS4 R1 trigger button = 0x0020 ==//
  //Controlling function added
  if (ctl->buttons() == 0x0020) {
    if (curSpeed >= 200){
    curSpeed = curSpeed + 200;
    } else{curSpeed = curSpeed * 2;}
  }
  if (ctl->buttons() != 0x0020) {
    // code for when R1 button is released
  }

  //== PS4 R2 trigger button = 0x0080 ==//
  if (ctl->buttons() == 0x0080) {
    // code for when R2 button is pushed
  }
  if (ctl->buttons() != 0x0080) {
    // code for when R2 button is released
  }

  //== PS4 L1 trigger button = 0x0010 ==//
  //Controlling function added
  if (ctl->buttons() == 0x0010) {
    if(curSpeed != 0){
      if(curSpeed <= 200){
        curSpeed = curSpeed * 0.5;
      }else {curSpeed = curSpeed - 200;}
    }
  }
  if (ctl->buttons() != 0x0010) {
    // code for when L1 button is released
  }

  //== PS4 L2 trigger button = 0x0040 ==//
  //Controlling function added
  if (ctl->buttons() == 0x0040) {
    reset();
  }
  if (ctl->buttons() != 0x0040) {
    // code for when L2 button is released
  }

  //== LEFT JOYSTICK - UP ==//
  //Controlling function added
  if (ctl->axisY() <= -200) {
    st.WritePosEx(3, 4000, curSpeed, 25);
    movedServo3 = true;
    }

  //== LEFT JOYSTICK - DOWN ==//
  if (ctl->axisY() >= 200) {
    st.WritePosEx(3, 0, curSpeed, 25);
    movedServo3 = true;
  }

  //== LEFT JOYSTICK - LEFT ==//
  if (ctl->axisX() <= -200) {
    st.WritePosEx(4, 4000, curSpeed, 25);
    movedServo4 = true;
  }

  //== LEFT JOYSTICK - RIGHT ==//
  if (ctl->axisX() >= 200) {
    st.WritePosEx(4, 0, curSpeed, 25);
    movedServo4 = true;
  }

  //== LEFT JOYSTICK DEADZONE ==//
  if (ctl->axisY() > -150 && ctl->axisY() < 150 && ctl->axisX() > -150 && ctl->axisX() < 150) {
    if(movedServo3 || movedServo4){
    stop(3);
    stop(4);
    movedServo3 = false;
    movedServo4 = false;
    }
  }

  /*LEAVING IN FOR NOW BUT OLD CODE
  //== RIGHT JOYSTICK - X AXIS ==//
  if (ctl->axisRX()) {
    st.WritePosEx(5, 4000, curSpeed, 25);
  }

  //== RIGHT JOYSTICK - Y AXIS ==//
  if (ctl->axisRY()) {
  // code for when right joystick moves along y-axis
  }
  */
  
  //== RIGHT JOYSTICK - UP ==//
  //Controlling function added
  if (ctl->axisRY() <= -200) {
    st.WritePosEx(5, 4000, curSpeed, 25);
    movedServo5 = true;
    }

  //== RIGHT JOYSTICK - DOWN ==//
  if (ctl->axisRY() >= 200) {
    st.WritePosEx(5, 0, curSpeed, 25);
    movedServo5 = true;
  }

  //== RIGHT JOYSTICK - LEFT ==//
  if (ctl->axisRX() <= -200) {
    st.WritePosEx(6, 4000, curSpeed, 25);
    movedServo6 = true;
  }

  //== RIGHT JOYSTICK - RIGHT ==//
  if (ctl->axisRX() >= 200) {
    st.WritePosEx(6, 0, curSpeed, 25);
    movedServo6 = true;
  }

  //== RIGHT JOYSTICK DEADZONE ==//
  if (ctl->axisRY() > -150 && ctl->axisRY() < 150 && ctl->axisRX() > -150 && ctl->axisRX() < 150) {
    if(movedServo5 || movedServo6){
    stop(5);
    stop(6);
    movedServo5 = false;
    movedServo6 = false;
    }
  }
  
  dumpGamepad(ctl);
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
         processGamepad(myController);
      }
      else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {
  Serial.begin(115200);
  //Setting up serial communication with serial bus
  Serial2.begin(1000000, SERIAL_8N1);
  st.pSerial = &Serial2;
  
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
  // This call fetches all the controllers' data.
  // Call this function in your main loop.
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();

    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

    // vTaskDelay(1);
  //See what breaks
  //delay(150);
}
