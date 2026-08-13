#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Wire.h>
#include "IRDeviceProfile.hpp"

const uint16_t kIrLed = 25; // 14
IRsend irsend(kIrLed);


void irSendSetup() {
  // put your setup code here, to run once:
  irsend.begin();
}



void sendRawSignal(uint16_t* data, uint16_t hz=38) {
  irsend.sendRaw(data, sizeof(data) / sizeof(data[0]), hz);
  delay(30);
}

void sendNECdata(uint64_t data) {
    // IRTransmitter->sendNEC(data);
    irsend.sendNEC(data);
}

// I think the code is the same but I haven't tried yet so I'll just keep it like so until testing
void sendSONYdata(uint64_t data) {
    // IRTransmitter->sendSONY(data);
    irsend.sendSony(data);
}





void handleIRCommand(IRProtocol protocol, IRCommand command){
    String signal = "";
    Serial.print("Sending Signal: ");
    switch (protocol) {
        case IRProtocol::NEC:
            // break;
            sendNECdata(std::get<uint64_t>(command.data));
            signal = String(std::get<uint64_t>(command.data), HEX);
            Serial.println(signal);
            
        case IRProtocol::RAW:
            // break;
            if (std::holds_alternative<std::vector<uint16_t>>(command.data)) {
                
                std::vector<uint16_t> data = std::get<std::vector<uint16_t>>(command.data);
                uint16_t* ptr = data.data();
                sendRawSignal(ptr);
            }
        case IRProtocol::SONY:
            sendSONYdata(std::get<uint64_t>(command.data));
        default: // unknown protocol
            break;
    }
};