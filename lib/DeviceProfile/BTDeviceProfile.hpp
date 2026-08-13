#pragma once

#include <Arduino.h>
#include <vector>
#include <variant>

#include "DeviceProfile.hpp"
#include "structs.hpp"

enum class BTCommandType{
    KEYBOARD,
    MEDIA,
    SETTING
};

// typedef uint8_t MediaKey[2];
struct MediaKey{
    uint8_t data[2];
};

struct BTCommand : public command{
    // String name;
    // std::vector<uint8_t> data; // same as uint8_t -- storing as a vector in case combos are needed
    std::variant<
        uint8_t,
        std::vector<uint8_t>,
        MediaKey
    > data;
    BTCommandType type;
    bool sync_keypress = true; // whether to send the set of keys simultaneously

    BTCommand(
        const String& name, 
        std::vector<uint8_t> data,
        // BTCommandType type=BTCommandType::KEYBOARD,
        bool sync_keypress=true) 
        : command(name), data(std::move(data)), type(BTCommandType::KEYBOARD), sync_keypress(sync_keypress){}
    
        // if only entering a single char
    BTCommand(
        const String& name, uint8_t data
        // BTCommandType type=BTCommandType::KEYBOARD,
    ) : command(name), data(data), type(BTCommandType::KEYBOARD) {}
    
    BTCommand(
        const String& name, MediaKey data
        // BTCommandType type=BTCommandType::MEDIA
    ) : command(name), data(std::move(data)), type(BTCommandType::MEDIA) {}

    String getName(){return name;};

};

class BTDeviceProfile : public DeviceProfile{
    public:
    transmissionModes mode = transmissionModes::BT;
    String name = "BT Device";
    std::vector<BTCommand> commands;

    void handleBtnPress(int btnNum){};
    int getCommandCount(){return commands.size();};
    std::vector<BTCommand>& getCommands() {return commands;};
    command* getCommand(int index) {return &commands[index];};

    

    BTDeviceProfile(
        String name,
        std::vector<BTCommand> commands
    ) : DeviceProfile(name, transmissionModes::BT),
        commands(std::move(commands)) {}

    
};