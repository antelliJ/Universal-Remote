#pragma once
// include vectors
#include <vector>
#include <Arduino.h>
#include "structs.hpp"
#include "DeviceProfile.hpp"
// #include <IRremoteESP8266.h>
// #include <IRsend.h>
// #include "irSend.hpp" // I don't think the profile needs any of the sending logic, that can be done in main
#include <variant>

// command struct has name, and data
// This is the worst piece of code that I feel like I've ever written but if it works then I'll take it
struct IRCommand : public command {
    // String name;
    // uint64_t data;
    // std::variant<uint64_t, std::vector<int>> data; // compiles fine? - apparently bug with platformio + vscode intellisense?
    std::variant<
        uint64_t, 
        std::vector<uint16_t>
    > data;
    // seems like it doesn't compile fine
    IRCommand(
        const String& name, 
        std::variant<uint64_t, std::vector<uint16_t>> data
    ) : command(name), data(data) {}
    String getName(){return name;};
};


// enum for different IR protocols - will add more later as necessary
enum class IRProtocol {
    NEC,
    RAW,
    SONY,
    UNKNOWN
};


class IrDeviceProfile : public DeviceProfile {
    public:
    transmissionModes mode = transmissionModes::IR;
    String name = "IR Device";
    IRProtocol protocol = IRProtocol::NEC;
    std::vector<IRCommand> commands;
  
    void handleBtnPress(int btnNum){};
    int getCommandCount(){return commands.size();};
    std::vector<IRCommand>& getCommands()  {return commands;};
    command* getCommand(int index){return &commands[index];};


    IrDeviceProfile(
        String name, 
        IRProtocol protocol, 
        std::vector<IRCommand> commands
    ) 
        : DeviceProfile(name, transmissionModes::IR), 
        protocol(protocol), commands(std::move(commands)) {}
    // {
    //     this->name = name;
    //     this->protocol = protocol;
    //     this->commands = commands;
    // }

};