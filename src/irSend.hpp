#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Wire.h>
#include "IRDeviceProfile.hpp"

#include "irDump.hpp"

const uint16_t kIrLed = 8; // 14
IRsend irsend(kIrLed);

bool isInDumpMode = false;


void irSendSetup() {
  // put your setup code here, to run once:
  irsend.begin();

  IRRECSetup();
}



// void sendRawSignal(uint16_t* data, uint16_t hz=38) {
//   irsend.sendRaw(data, sizeof(data) / sizeof(data[0]), hz);
//   delay(30);
// }

void sendRawSignal(const std::vector<uint16_t>& data, uint16_t hz=38) {
  irsend.sendRaw(data.data(), data.size(), hz);
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

void sendRC5data(uint64_t data) {
    irsend.sendRC5(data);
}




void handleIRCommand(IRProtocol protocol, IRCommand command){
    String signal = "";
    Serial.print("Sending Signal: ");
    switch (protocol) {
        case IRProtocol::SETTINGS:
            if (command.getName() == "IR Dump"){
                Serial.println("Dumping IR Code");
                // IRRECLoop();
                isInDumpMode = true;
            }
            break;
        case IRProtocol::NEC:
            signal = String(std::get<uint64_t>(command.data), HEX);
            sendNECdata(std::get<uint64_t>(command.data));
            Serial.println(signal);
            break;
            
        case IRProtocol::RAW:
            if (std::holds_alternative<std::vector<uint16_t>>(command.data)) {
                
                // std::vector<uint16_t> data = std::get<std::vector<uint16_t>>(command.data);
                // uint16_t* ptr = data.data();
                // sendRawSignal(ptr);

                const std::vector<uint16_t>& data = std::get<std::vector<uint16_t>>(command.data);
                sendRawSignal(data);

                for (uint16_t i = 0; i < data.size(); i++) {
                    Serial.print(data[i]);
                    Serial.print(" ");
                }
                Serial.println();
            }
            break;
        case IRProtocol::SONY:
            sendSONYdata(std::get<uint64_t>(command.data));
            Serial.println(std::get<uint64_t>(command.data));
            break;
        case IRProtocol::RC5:
            sendRC5data(std::get<uint64_t>(command.data));
            Serial.println(std::get<uint64_t>(command.data));
            break;
        default: // unknown protocol
            Serial.println("Unknown IR Protocol");
            break;
    }
};