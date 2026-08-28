#include "configs.hpp"

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

#include <functional>

// TODO FEATURES:
// testing- add bluetooth functionality
//    conductCommandAction
// make all names/cmds scroll if too long
// testing- simulate all buttons / IO
// find space for IR dump
// testing- toggleProfileSelection

// maybe make all the buttons into a struct and use a vector of them
// maybe make the top of the remote top heavy with screen, IR transceiver, ESP, encoder
//     and bottom with all the buttons

// UNRESOLVED POTENTIAL ISSUES:
// CurrentState.availableProfiles - it is now updated when it is supposed to
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

// debounce timeout for shift register (and encoder button)
int InputRegisterTimeout = 0;
int screenUpdateTimeout = 0;

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
void enterBT(); // check if object exists, create
void exitBT();
void clearCmds();
void clearProfileList();
void loadProfileList(const std::vector<DeviceProfile*>& profiles, int page=0);
void reloadProfileList();
void profileListSetup();
void printByte(byte byteVar) {
  for (int i=7; i >= 0; i--) {
    Serial.print(bitRead(byteVar, i));
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  // initialize all modules
  screenSetup();
  Serial.println("Screen initialized");


  encoderSetup();
  registerSetup(); // why was this not here before ??????????
  


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



void enterBT() {
  Serial.println("starting BT");
  setupBT();
}

void exitBT() {
  Serial.println("exiting BT");
  disconnectBT();
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
      if (isInDumpMode) {
        isInDumpMode = false;
      }
    }
    // simulate each of the 8 buttons and their actions
    // in the format of btn<index>
    // ex btn1
    for (int i = 0; i < 8; i++) {
      if (data.equalsIgnoreCase("btn"+String(i))) {
        if (isInDumpMode) {
          isInDumpMode = false;
        }

        
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
      Serial.print("Encoder bitmask: ");
      Serial.println(readEncoderSignal());
      Serial.print("Button bitmask: ");
      Serial.println(scanRegister());
    }
  }
}

void checkInputActions(){
  // check if timeout has passed
  if (!((millis() - InputRegisterTimeout) > DEBOUNCE_DELAY)) {
    return;
  } 

  // get encoder signal (scroll for every 3~ rotations)
  byte encoderSignal = readEncoderSignal();
  if (encoderSignal){
    if (encoderSignal & ENCODER_SCROLLUP) {
      onScroll(true);
      // InputRegisterTimeout = millis();
    } 
    if (encoderSignal & ENCODER_SCROLLDOWN) {
      onScroll(false);
      // InputRegisterTimeout = millis();
    } 
    if (encoderSignal & ENCODER_BTN) {
      toggleProfileSelection();
      InputRegisterTimeout = millis();
    }
  }


  // get data of buttons
  byte btnData = scanRegister();
  
  #if USE_SHIFT_REG
    for (int i = 0; i < 8; i++) {
      if (btnData & (1 << i)) {
        InputRegisterTimeout = millis();
        if (CurrentState.selectingProfile) {
          if (CurrentState.mode == transmissionModes::IR) {
              CurrentState.mode = transmissionModes::BT;
              setupBT();
            } else {
              CurrentState.mode = transmissionModes::IR;
              disconnectBT();
            }
            profileListSetup();

        } else { // conduct a command
          Serial.print("button pressed: ");
          Serial.println(i);
          conductCommandAction(CurrentState.availableCommands[i]);
        }
      }
    }
  #else
    if (btnData) { // in this case it is either 0 or 1
      InputRegisterTimeout = millis();
      if (CurrentState.selectingProfile) {
        if (CurrentState.mode == transmissionModes::IR) {
            CurrentState.mode = transmissionModes::BT;
            setupBT();
          } else {
            CurrentState.mode = transmissionModes::IR;
            disconnectBT();
          }
          profileListSetup();

      } else { // conduct a command
        Serial.print("button pressed: ");
        Serial.println(CurrentState.selectionCursor);
        conductCommandAction(CurrentState.availableCommands[CurrentState.selectionCursor]);
      }
    }

  #endif
  
}

void loop() {
  

  // DEBUGGING INPUTS
  checkInputActions();

  // static uint8_t last = 255;
  // uint8_t state = 
  //   (digitalRead(outputA) << 1) | 
  //   digitalRead(outputB);

  // if (state != last) {
  //   Serial.println(state, BIN);
  //   last = state;
  // }

  // // debug print state of encoder and buttons
  // byte encoderSignal = readEncoderSignal();
  // byte btnData = scanRegister();
  // if (encoderSignal || btnData) {
  //   Serial.print("Encoder: ");
  //   // Serial.println(encoderSignal, BIN);
  //   printByte(encoderSignal);
  //   Serial.print("Buttons: ");
  //   // Serial.println(btnData, BIN);
  //   printByte(btnData);
  //   delay(1000);
  // }


  if ((millis() - screenUpdateTimeout) > 50) { // 1 / .05 = 20 fps -- doing 16 would be ~60 fps
    checkSerialCmd();
    updateScreen(&CurrentState);
    screenUpdateTimeout = millis();
  }

  if (isInDumpMode) {
    IRRECLoop();
  }



  delay(50);
}


int profileAmtOnPage(int currentPage){
  int len = profileAmt();

  // return len/4 if currentPage == lastPage
  // return CurrentState.lastPage == currentPage ? len%4 : 4;
  return (CurrentState.lastPage == currentPage) ?(len - ((pageAmt()-1)*4)) : 4;
}

int cmdAmtOnPage(int currentPage) {
  int len = CurrentState.currentProfile->getCommandCount();
  return (CurrentState.lastPage == currentPage) ? (len - ((pageAmt()-1)*8)) : 8;
}

int itemAmtOnPage(int currentPage) {
  if (CurrentState.selectingProfile) {
    return profileAmtOnPage(currentPage);
  }

  return cmdAmtOnPage(currentPage);
}

// func is either loadCmds or reloadProfileList
void updateScrollingOnPage(bool up, const std::function<void(void)> &func) {
  if (up) {
    CurrentState.selectionCursor--;
    if (CurrentState.selectionCursor < 0) {
      // CurrentState.currentPage = (CurrentState.currentPage - 1)%pageAmt();
      CurrentState.currentPage = (CurrentState.currentPage==0) ? (pageAmt()-1) : (CurrentState.currentPage - 1);
      CurrentState.selectionCursor = itemAmtOnPage(CurrentState.currentPage) - 1;
      // loadCmds();
      func();
    }
  } else { // scroll down
    CurrentState.selectionCursor++;
    if ((CurrentState.selectionCursor >= itemAmtOnPage(CurrentState.currentPage))) {
      // go to next page (or wrap around)
      // CurrentState.currentPage = (CurrentState.currentPage + 1)%pageAmt();
      CurrentState.currentPage = (CurrentState.currentPage==pageAmt()-1) ? 0 : (CurrentState.currentPage + 1);
      CurrentState.selectionCursor = 0;
      // loadCmds();
      func();
    }

  
  // CurrentState.selectionCursor = (CurrentState.selectionCursor+1)%profileAmt();
  }
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

    updateScrollingOnPage(up, reloadProfileList);
    // Serial.print(CurrentState.lastPage);

  } else { // scrolling commands

    #if USE_SHIFT_REG

      // check if upcoming page goes above or below page bounds
      if (up) {
        // CurrentState.currentPage = (int(CurrentState.currentPage) - 1)%pageAmt();
        CurrentState.currentPage = (CurrentState.currentPage==0) ? (pageAmt()-1) : (CurrentState.currentPage - 1);
      } else {
        // CurrentState.currentPage = (CurrentState.currentPage + 1)%pageAmt();
        CurrentState.currentPage = (CurrentState.currentPage==pageAmt()-1) ? 0 : (CurrentState.currentPage + 1);
      }
      loadCmds();
    #else
      // increase selectionCursor, wrap around that and current page if necessary
      updateScrollingOnPage(up, loadCmds);

    #endif
    
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

// void loadProfileList(std::vector<DeviceProfile*> profiles, int page) {
//   // load into availableProfiles
//   Serial.print("profile count: ");
//   Serial.print(profiles.size());
  
//   for (int i = (page*4); i < (profileAmtOnPage(page)+(page*4)); i++) {
//     CurrentState.availableProfiles[i] = profiles[i];
//   }
// }

// void loadProfileList(const std::vector<DeviceProfile*>& profiles, int page) {

//     Serial.print("profile count: ");
//     Serial.println(profiles.size());

//     int start = page * 4;
//     int count = profileAmtOnPage(page);

//     for (int i = 0; i < 4; i++) {

//         if (i < count) {
//             CurrentState.availableProfiles[i] = profiles[start + i];

//             Serial.print("Loaded profile ");
//             Serial.println(start + i);
//         } 
//         else {
//             CurrentState.availableProfiles[i] = nullptr;
//         }
//     }
// }
void loadProfileList(
    const std::vector<DeviceProfile*>& profiles,
    int page
) {
    for (int i = 0; i < 4; i++) {

        int index = page * 4 + i;

        if (index < profiles.size()) {
            CurrentState.availableProfiles[i] = profiles[index];

            Serial.print("Loaded profile in list: ");
            Serial.print(index);
            Serial.print(", ");
            Serial.println(profiles[index]->name);
        } else {
            CurrentState.availableProfiles[i] = nullptr;
        }
    }
}


void loadProfile(DeviceProfile* newProfile) {
  if (CurrentState.mode == transmissionModes::BT && newProfile->mode != transmissionModes::BT) {
    disconnectBT();
  }

  if (CurrentState.mode != transmissionModes::BT && newProfile->mode == transmissionModes::BT) {
    setupBT();
  }

  Serial.print("Loading profile ");
  Serial.println(newProfile->name);

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
  CurrentState.currentPage = 0;
  CurrentState.selectionCursor = 0;
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