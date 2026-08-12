#include <Arduino.h>
#include "readEncoder.hpp"
#include "readRegister.hpp"
#include "drawScreen.hpp"
#include "IRDeviceProfile.hpp"

#include "DeviceProfile.hpp"

#include "structs.hpp"
#include "state.hpp"

// #include "IRProfilesList.hpp"
// #include "BTProfilesList.hpp"
// I don't really know if this is ever used?
#include "AllProfiles.hpp"

#include "irSend.hpp"
#include "btControls.hpp"

// TODO:
// add bluetooth functionality
//    conductCommandAction
// make all names/cmds scroll if too long
// simulate all buttons / IO
// find space for IR dump
// toggleProfileSelection


// ALL SIMULATED INPUTS:
// scrollup
// scrolldown
// toggle
// btn0
// btn1
// btn2
// btn3
// btn4
// btn5
// btn6
// btn7



// uint8_t curProfilePage = 0;
// DeviceProfile* currentProfile = nullptr;

// command availableCommands[8];

state CurrentState;

// if only there was some special type of file where all these headers could live
// too bad that doesn't exist
void loadProfile(DeviceProfile* newProfile); // set currentProfile safely
void loadCmds();
int pageAmt();
int profileAmt();
void onProfileSelected(); // put into buffer
void onScroll(bool up); // rotate buffer accordingly - cycle back to first page is exceeds total - do safely
void toggleProfileSelection();
void conductCommandAction(command* cmd);
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
  allProfileSetup();
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
    } else if (data.equalsIgnoreCase("toggle")) {
      toggleProfileSelection();
    }
    // simulate each of the 8 buttons and their actions
    // in the format of btn<index>
    // ex btn1
    for (int i = 0; i < 8; i++) {
      if (data.equalsIgnoreCase("btn"+String(i))) {
        if (CurrentState.selectingProfile) {
          // toggle between BT and IR profiles
          // while smart, we want logic to occur when switching to BT, and to turn it off for IR
          // CurrentState.mode = (CurrentState.mode == transmissionModes::IR) ? transmissionModes::BT : transmissionModes::IR;
          if (CurrentState.mode == transmissionModes::IR) {
            CurrentState.mode = transmissionModes::BT;
            setupBT();
          } else {
            CurrentState.mode = transmissionModes::IR;
            disconnectBT();
          }

        } else{
          // Conduct the connected command
          // conductCommandAction(CurrentState.currentProfile->getCommand(i))
          conductCommandAction(CurrentState.availableCommands[i]);
        }
      }
    }
  }
}

void checkInputActions(){
  // get encoder signal (scroll for every 3~ rotations)

  // get data of buttons
  byte data = scanRegister();

  if (CurrentState.selectingProfile) {

  } else {
    
  }
}

void loop() {
  checkSerialCmd();

  // CURRENT PIN LAYOUT INTERFERES WITH SCREEN I2C
  // BRING BACK LATER
  // checkInputActions();
  
  updateScreen(&CurrentState);

  delay(250);
}




// TODO this is temp 
void onScroll(bool up) {
  Serial.println("Scrolling");
  if (CurrentState.selectingProfile) { // scrolling profiles
    if (up) {
      CurrentState.selectionCursor = (int(CurrentState.selectionCursor)-1)%profileAmt();
    } else {
      CurrentState.selectionCursor = (CurrentState.selectionCursor+1)%profileAmt();
    }
  } else { // scrolling commands
    // check if upcoming page goes above or below page bounds
    if (up) {
      CurrentState.currentPage = (int(CurrentState.currentPage) - 1)%pageAmt();
    } else {
      CurrentState.currentPage = (CurrentState.currentPage + 1)%pageAmt();
    }
    loadCmds();
  }
  
}


// starts at 1
int pageAmt() {
  int len = CurrentState.currentProfile->getCommandCount();
  return (len+7)/8;
}

int profileAmt(){
  int len = 0;
  if (CurrentState.mode == transmissionModes::IR) {
    len = IRProfiles.size();
  } else if (CurrentState.mode == transmissionModes::BT) {
    // len = BTProfiles.size();
  }
  return len;
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

void clearCmds(){
  // set all to nullptr
  for (int i = 0; i < 8; i++) {
    CurrentState.availableCommands[i] = nullptr;
  }
}

void loadProfileList(std::vector<DeviceProfile*> profiles) {
  // load into availableProfiles
  Serial.print("profile count: ");
  Serial.print(profiles.size());
  for (int i = 0; i < profiles.size(); i++) {
    CurrentState.availableProfiles[i] = profiles[i];
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

// switch state of selecting profile, // check if a profile has been selected, if so then load
void toggleProfileSelection() {
  CurrentState.selectingProfile = !CurrentState.selectingProfile;
  CurrentState.currentPage = 0;
  if (CurrentState.selectingProfile) {
    CurrentState.selectionCursor = 0;
    CurrentState.currentPage = 0;
    CurrentState.lastPage = profileAmt()-1;
    clearCmds();

    if (CurrentState.mode == transmissionModes::IR) {
      loadProfileList(IRProfiles);
    } else if (CurrentState.mode == transmissionModes::BT) {
      loadProfileList(BTProfiles);
    }


  } else {
    // load profile of active element
    if (CurrentState.mode == transmissionModes::IR) {
      // loadProfile(IRProfiles[CurrentState.selectionCursor]);
    } else if (CurrentState.mode == transmissionModes::BT) {
      // loadProfile(BTProfiles[CurrentState.selectionCursor]);
    }
  }
  // check if a profile has been selected, if so then load
  return;
}

void conductCommandAction(command* cmd) {
  if (CurrentState.currentProfile->mode == transmissionModes::IR) {
    // get the protocol, irSend handles variant data
    // interpret profile as IrDeviceProfile
    IrDeviceProfile* irProfile = static_cast<IrDeviceProfile*>(CurrentState.currentProfile);
    IRCommand* irCmd = static_cast<IRCommand*>(cmd);
    handleIRCommand(irProfile->protocol, *irCmd);
  } else if (CurrentState.currentProfile->mode == transmissionModes::BT) {

  }
}