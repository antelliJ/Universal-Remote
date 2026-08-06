#include <Arduino.h>
#include "readEncoder.hpp"
#include "readRegister.hpp"
#include "IRDeviceProfile.hpp"

#include "DeviceProfile.hpp"

#include "structs.hpp"

uint8_t curProfilePage = 0;
DeviceProfile* currentProfile = nullptr;

command availableCommands[8];

state CurrentState; 

void loadProfile(); // set currentProfile safely
void onProfileSelected(); // put into buffer
void onScroll(bool up); // rotate buffer accordingly - cycle back to first page is exceeds total - do safely
void toggleProfileSelection();
void conductCommandAction();
void startIRDump();
void startBLEDevice(); // check if object exists, create
void disconnectBLEDevice();

void setup() {
  Serial.begin(115200);

  // initialize all modules



  // setup IR profile as the current


  // load profile
}





void checkSerialCmd(){
  if (Serial.available()){
    String data = Serial.readStringUntil('\n');
    data.trim();
    }
}

void checkInputActions(){
  // get encoder signal

  // get data of buttons
  byte data = scanRegister();
}

void loop() {
  checkSerialCmd();

  checkInputActions();
  delay(250);
}
