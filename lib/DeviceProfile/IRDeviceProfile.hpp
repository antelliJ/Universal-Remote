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
// This is the worst piece of code that I feel like I've ever written but if it works then I'll take it
struct IRCommand : public command {
    // String name;
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