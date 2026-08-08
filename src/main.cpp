#include <Arduino.h>
#include "readEncoder.hpp"
#include "readRegister.hpp"
#include "drawScreen.hpp"
#include "IRDeviceProfile.hpp"

#include "DeviceProfile.hpp"

#include "structs.hpp"
#include "state.hpp"

#include "IRProfilesList.hpp"

// uint8_t curProfilePage = 0;
// DeviceProfile* currentProfile = nullptr;

// command availableCommands[8];

state CurrentState;

void loadProfile(DeviceProfile* newProfile); // set currentProfile safely
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
  screenSetup();



  // setup IR profile as the current & load profile
  // CurrentState.currentProfile = IRProfiles[0];
  // CurrentState.mode = CurrentState.currentProfile->mode;
  loadProfile(IRProfiles[0]);
}






void checkSerialCmd(){
  if (Serial.available()){
    String data = Serial.readStringUntil('\n');
    data.trim();
    if (data.equalsIgnoreCase("scrollup")) {
      onScroll(true);
    } else if (data.equalsIgnoreCase("scrolldown")) {
      onScroll(false);
    }
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




// TODO this is temp 
void onScroll(bool up) {
  Serial.println("Scrolling");
}


// starts at 1
int pageAmt() {
  int len = CurrentState.currentProfile->getCommandCount();
  return (len/8)+1;
}

// loads currentProfile commands into 8 size cmd buffer
void loadCmds() {
  // for (int i = (CurrentState.currentPage*8); i++; i < (CurrentState.currentPage*8)+8) {
  //   CurrentState.availableCommands[]
  // }

  for (int i = 0; i < 8; i++) {
    if (i + (CurrentState.currentPage*8) < CurrentState.currentProfile->getCommandCount()) {
      CurrentState.availableCommands[i] = CurrentState.currentProfile->getCommands()[(CurrentState.currentPage*8)+i];
    }
  }
}


void loadProfile(DeviceProfile* newProfile) {
  CurrentState.currentProfile = newProfile;
  CurrentState.mode = CurrentState.currentProfile->mode;

  // calculate number of pages
  CurrentState.lastPage = pageAmt()-1;

  CurrentState.currentPage = 0;

  // put available commands in the buffer
  loadCmds();
}

