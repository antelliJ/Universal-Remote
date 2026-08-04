#include <Arduino.h>
#include "readEncoder.hpp"
#include "readRegister.hpp"



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
  byte data = scanInputs();
}

void loop() {
  checkSerialCmd();

  checkInputActions();
  delay(250);
}
