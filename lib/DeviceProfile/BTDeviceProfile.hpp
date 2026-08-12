#include <Arduino.h>
#include <vector>
#include <variant>

#include "DeviceProfile.hpp"
#include "structs.hpp"

struct BTCommand : public command{
    // String name;
    std::vector<char> data; // same as uint8_t -- storing as a vector in case combos are needed
    bool sync = true; // whether to send the set of keys simultaneously

    BTCommand(const String& name, std::vector<char> data) : command(name), data(data){}
    // if only entering a single char
    BTCommand(const String& name, char data) : command(name), data({data}) {}

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