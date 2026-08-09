#include <Arduino.h>
#include "readEncoder.hpp"
#include "readRegister.hpp"
#include "drawScreen.hpp"
#include "IRDeviceProfile.hpp"

#include "DeviceProfile.hpp"

#include "structs.hpp"
#include "state.hpp"

#include "IRProfilesList.hpp"

// TODO:
// add bluetooth functionality
// make all names/cmds scroll if too long



// uint8_t curProfilePage = 0;
// DeviceProfile* currentProfile = nullptr;

// command availableCommands[8];

state CurrentState;

void loadProfile(DeviceProfile* newProfile); // set currentProfile safely
void loadCmds();
void onProfileSelected(); // put into buffer
void onScroll(bool up); // rotate buffer accordingly - cycle back to first page is exceeds total - do safely
void toggleProfileSelection();
void conductCommandAction();
void startIRDump();
void startBLEDevice(); // check if object exists, create
void disconnectBLEDevice();

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  // initialize all modules
  screenSetup();
  Serial.println("Screen initialized");



  // setup IR profile as the current & load profile
  // CurrentState.currentProfile = IRProfiles[0];
  // CurrentState.mode = CurrentState.currentProfile->mode;
  loadProfile(IRProfiles[0]);
  Serial.println("Profile loaded");
  Serial.println(CurrentState.currentProfile->name);



  updateScreen(&CurrentState, false);
  Serial.println("Updated screen in SETUP");
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
  Serial.println("Scanned serial commands");

  // CURRENT PIN LAYOUT INTERFERES WITH SCREEN I2C
  // BRING BACK LATER
  // checkInputActions();
  
  updateScreen(&CurrentState);
  Serial.println("Updated screen");

  delay(250);
}




// TODO this is temp 
void onScroll(bool up) {
  Serial.println("Scrolling");
  if (up) {
    CurrentState.selectionCursor = (CurrentState.selectionCursor+1)%CurrentState.currentProfile->getCommandCount();
  } else {
    CurrentState.selectionCursor = (CurrentState.selectionCursor-1)%CurrentState.currentProfile->getCommandCount();
  }
  
  // check if cursor goes above or below page bounds
  if (CurrentState.selectionCursor < (CurrentState.currentPage*8)) {
    CurrentState.currentPage = CurrentState.currentPage-1;
    loadCmds();
  } else if (CurrentState.selectionCursor >= ((CurrentState.currentPage+1)*8)) {
    CurrentState.currentPage = CurrentState.currentPage+1;
    loadCmds();
  }
}


// starts at 1
int pageAmt() {
  int len = CurrentState.currentProfile->getCommandCount();
  return (len+7)/8;
}

// loads currentProfile commands into 8 size cmd buffer
void loadCmds() {
  // for (int i = (CurrentState.currentPage*8); i++; i < (CurrentState.currentPage*8)+8) {
  //   CurrentState.availableCommands[]
  // }
  Serial.print("Command count: ");
  Serial.println(CurrentState.currentProfile->getCommandCount());
  for (int i = 0; i < 8; i++) {

    int index = i + (CurrentState.currentPage*8);
    if (index < CurrentState.currentProfile->getCommandCount()) {
      // CurrentState.availableCommands[i] = &CurrentState.currentProfile->getCommands()[index];
      CurrentState.availableCommands[i] = CurrentState.currentProfile->getCommand(index);
      Serial.print("Loaded command ");
      Serial.println((CurrentState.currentPage*8)+i);
    } else {
      Serial.println("Iterating commands out of bounds");
      // same page but out of bounds
      CurrentState.availableCommands[i] = nullptr;
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

