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

// TODO FEATURES:
// testing- add bluetooth functionality
//    conductCommandAction
// make all names/cmds scroll if too long
// testing- simulate all buttons / IO
// find space for IR dump
// testing- toggleProfileSelection

// UNRESOLVED POTENTIAL ISSUES:
// CurrentState.availableProfiles - THIS IS NOT UPDATED PROPERLY AND WHEN NECESSARY
//    - workaround by calculating the global cursor position
// why does selection cursor go through the entire vector while
// scrolling profiles and not for commands?
// maybe add a safety lock if there are 0 profiles? 
// currently it could cause issues with PageAmt (should default to 1)
// irSend is technically always started at boot and never closed, not sure if that would cause issues
//    BLE starts only when its selected and closes when not selected, so should save power and performance and stuff


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
// 
// DEBUG INPUTS
// CurrentState


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
void onScroll(bool up); // rotate buffer accordingly - cycle back to first page if exceeds total - do safely
void toggleProfileSelection();
void conductCommandAction(command* cmd);
void startIRDump();
void startBLEDevice(); // check if object exists, create
void disconnectBLEDevice();
void clearCmds();
void clearProfileList();
void loadProfileList(std::vector<DeviceProfile*> profiles, int page=0);
void reloadProfileList();
void profileListSetup();

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  // initialize all modules
  screenSetup();
  Serial.println("Screen initialized");

  int bondCount = esp_ble_get_bond_device_num();



  // setup IR profile as the current & load profile
  // CurrentState.currentProfile = IRProfiles[0];
  // CurrentState.mode = CurrentState.currentProfile->mode;
  // allProfileSetup();
  loadProfile(IRProfiles[0]);
  irSendSetup();
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
            // // set cursor and page to 0
            // CurrentState.selectionCursor = 0;
            // CurrentState.currentPage = 0;
            setupBT();
          } else {
            CurrentState.mode = transmissionModes::IR;
            disconnectBT();
          }
          profileListSetup();

        } else{
          // Conduct the connected command
          // conductCommandAction(CurrentState.currentProfile->getCommand(i))
          conductCommandAction(CurrentState.availableCommands[i]);
        }
      }
    }


    // DEBUG STUFF
    // Print the current state
    if (data.equalsIgnoreCase("CurrentState")){
      Serial.println("Current State:");
      Serial.print("Mode: ");
      Serial.println((int)CurrentState.mode);
      Serial.print("Current Profile: ");
      Serial.println(CurrentState.currentProfile->name);
      Serial.print("Current Page: ");
      Serial.println(CurrentState.currentPage);
      Serial.print("Last Page: ");
      Serial.println(CurrentState.lastPage);
      Serial.print("Is Selecting Profile: ");
      Serial.println(CurrentState.selectingProfile);
      Serial.print("Selection Cursor: ");
      Serial.println(CurrentState.selectionCursor);
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


int profileAmtOnPage(int currentPage){
  int len = profileAmt();

  // return len/4 if currentPage == lastPage
  // return CurrentState.lastPage == currentPage ? len%4 : 4;
  return (CurrentState.lastPage == currentPage) ?(len - ((pageAmt()-1)*4)) : 4;
}

void reloadProfileList(){
  clearProfileList();
  if (CurrentState.mode == transmissionModes::IR) {
    loadProfileList(IRProfiles,CurrentState.currentPage);
  } else if (CurrentState.mode == transmissionModes::BT) {
    loadProfileList(BTProfiles, CurrentState.currentPage);
  }
}

// TODO this is temp 
void onScroll(bool up) {
  Serial.println("Scrolling");
  if (CurrentState.selectingProfile) { // scrolling profiles

    // update the cursor first, if it goes beyond 0/4 entries, increment the currentPage
    if (up) {
      CurrentState.selectionCursor--;
      if (CurrentState.selectionCursor < 0) {
        CurrentState.currentPage = (CurrentState.currentPage - 1)%pageAmt();
        CurrentState.selectionCursor = profileAmtOnPage(CurrentState.currentPage) - 1;
        // set profiles into CurrentState.availableProfiles
        reloadProfileList();
      }
      // CurrentState.selectionCursor = (int(CurrentState.selectionCursor)-1)%profileAmt();
    } else { // scroll down
      CurrentState.selectionCursor++;
      // check if it is on the last page w/ %
      if ((CurrentState.selectionCursor >= profileAmtOnPage(CurrentState.currentPage))) {
        // go to next page (or wrap around)
        CurrentState.currentPage = (CurrentState.currentPage + 1)%pageAmt();
        CurrentState.selectionCursor = 0;
        reloadProfileList();
      }

      
      // CurrentState.selectionCursor = (CurrentState.selectionCursor+1)%profileAmt();
    }



  } else { // scrolling commands
    // check if upcoming page goes above or below page bounds
    if (up) {
      // CurrentState.currentPage = (int(CurrentState.currentPage) - 1)%pageAmt();
      CurrentState.currentPage = (CurrentState.currentPage==0) ? (pageAmt()-1) : (CurrentState.currentPage - 1);
    } else {
      // CurrentState.currentPage = (CurrentState.currentPage + 1)%pageAmt();
      CurrentState.currentPage = (CurrentState.currentPage==pageAmt()-1) ? 0 : (CurrentState.currentPage + 1);
    }
    loadCmds();
  }
  
}


// page amount for COMMANDS and PROFILES
// starts at 1
int pageAmt() {
  int len = 0;
  if (CurrentState.selectingProfile){
    len = profileAmt();
    return (len+3)/4;
  } else{
    len = CurrentState.currentProfile->getCommandCount();
    return (len+7)/8;
  }
}

int profileAmt(){
  int len = 0;
  if (CurrentState.mode == transmissionModes::IR) {
    len = IRProfiles.size();
  } else if (CurrentState.mode == transmissionModes::BT) {
    len = BTProfiles.size();
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

void clearProfileList(){
  for (int i = 0; i < 4; i++) {
    CurrentState.availableProfiles[i] = nullptr;
  }
}

void loadProfileList(std::vector<DeviceProfile*> profiles, int page) {
  // load into availableProfiles
  Serial.print("profile count: ");
  Serial.print(profiles.size());
  
  for (int i = (page*4); i < (profileAmtOnPage(page)+(page*4)); i++) {
    CurrentState.availableProfiles[i] = profiles[i];
  }
}



void loadProfile(DeviceProfile* newProfile) {
  CurrentState.currentProfile = newProfile;
  CurrentState.mode = CurrentState.currentProfile->mode;
  CurrentState.selectionCursor = 0;

  // calculate number of pages
  CurrentState.lastPage = pageAmt()-1;

  CurrentState.currentPage = 0;

  // put available commands in the buffer
  loadCmds();
}


void profileListSetup(){
  CurrentState.selectionCursor = 0;
  CurrentState.currentPage = 0;
  CurrentState.lastPage = pageAmt()-1;
  clearCmds();
  reloadProfileList();
}

// switch state of selecting profile, // check if a profile has been selected, if so then load
void toggleProfileSelection() {
  CurrentState.selectingProfile = !CurrentState.selectingProfile;
  CurrentState.currentPage = 0;
  if (CurrentState.selectingProfile) {
    profileListSetup();

  } else {
    clearProfileList();
    // load profile of active element
    // calculate the selected profile - selectionCursor is relative to page
    int cursorPlaceGlobal = CurrentState.selectionCursor + (CurrentState.currentPage*4);
    if (CurrentState.mode == transmissionModes::IR) {
      // CurrentState.availableProfiles - this 
      // loadProfile(CurrentState.availableProfiles[CurrentState.selectionCursor]);
      loadProfile(IRProfiles[cursorPlaceGlobal]);
    } else if (CurrentState.mode == transmissionModes::BT) {
      loadProfile(BTProfiles[cursorPlaceGlobal]);
    }
  }
  // check if a profile has been selected, if so then load
  return;
}

// I think this is somewhat safe if still in profile selection or something
// BUT that should never happen and I haven't tested so might be very broken
void conductCommandAction(command* cmd) {
  if (CurrentState.currentProfile->mode == transmissionModes::IR) {
    // get the protocol, irSend handles variant data
    // interpret profile as IrDeviceProfile
    IrDeviceProfile* irProfile = static_cast<IrDeviceProfile*>(CurrentState.currentProfile);
    IRCommand* irCmd = static_cast<IRCommand*>(cmd);
    handleIRCommand(irProfile->protocol, *irCmd);
  } else if (CurrentState.currentProfile->mode == transmissionModes::BT) {
    BTCommand* btCmd = static_cast<BTCommand*>(cmd);
    handleBTCommand(btCmd);
  }
}