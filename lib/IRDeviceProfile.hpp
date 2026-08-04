// include vectors
#include <vector>
#include <Arduino.h>
#include "structs.hpp"
#include "DeviceProfile.hpp"

// command struct has name, and data
struct IRCommand {
    String name;
    uint64_t data;
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
            break;
        case IRProtocol::SONY:
            break;
        case IRProtocol::UNKNOWN:
            break;
    }
};