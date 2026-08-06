#include <Arduino.h>
#include "readEncoder.hpp"
#include "readRegister.hpp"
#include "IRDeviceProfile.hpp"

#include "DeviceProfile.hpp"

uint8_t curProfilePage = 0;
DeviceProfile* currentProfile = nullptr;

void setup() {
  Serial.begin(115200);

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
