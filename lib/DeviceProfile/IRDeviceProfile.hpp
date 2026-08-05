// include vectors
#include <vector>
#include <Arduino.h>
#include "structs.hpp"
#include "DeviceProfile.hpp"
// #include <IRremoteESP8266.h>
// #include <IRsend.h>
#include "irSend.hpp"
#include <variant>

// command struct has name, and data
struct IRCommand {
    String name;
    // uint64_t data;
    // std::variant<uint64_t, std::vector<int>> data; // compiles fine? - apparently bug with platformio + vscode intellisense?
    std::variant<uint64_t, uint16_t*> data;
    // seems like it doesn't compile fine
};


// enum for different IR protocols - will add more later as necessary
enum class IRProtocol {
    NEC,
    RAW,
    SONY,
    UNKNOWN
};


class IrDeviceProfile : public DeviceProfile {
    transmissionModes mode = transmissionModes::IR;
    String name = "IR Device";
    IRProtocol protocol = IRProtocol::NEC;
    std::vector<IRCommand> commands;
  
    public:
    void handleBtnPress(int btnNum){};
};



void handleCommand(IRProtocol protocol, IRCommand command){
    switch (protocol) {
        case IRProtocol::NEC:
            break;
            
        case IRProtocol::RAW:
            // break;
            if (std::holds_alternative<uint16_t*>(command.data)) {
                
                uint16_t* data = std::get<uint16_t*>(command.data);
                sendRawSignal(data);
            }
        case IRProtocol::SONY:
            break;
        default: // unknown protocol
            break;
    }
};


// void sendNECdata(IRsend* IRTransmitter, uint64_t data) {
//     IRTransmitter->sendNEC(data);
// }

// // I think the code is the same but I forgot
// void sendSONYdata(IRsend* IRTransmitter, uint64_t data) {
//     // IRTransmitter->sendSONY(data);
//     return;
// }


// void sendRAWdata(IRsend* IRTransmitter, uint64_t data) {
//     IRTransmitter->sendraw
// }
