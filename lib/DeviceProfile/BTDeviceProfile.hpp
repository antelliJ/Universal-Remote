#include <Arduino.h>
#include <vector>
#include <variant>

#include "DeviceProfile.hpp"
#include "structs.hpp"

struct BTCommand : public command{
    // String name;
    std::vector<char> data; // same as uint8_t -- storing as a vector in case combos are needed
    
    BTCommand(String name, std::vector<char> data){
        this->name = name;
        this->data = data;
    }
    // if only entering a single char
    BTCommand(String name, char data){
        this->name = name;
        // this->data.push_back(data);
        this->data = std::vector<char>{data};
    }

};

class BTDeviceProfile {
    public:
    transmissionModes mode = transmissionModes::BT;
    String name = "BT Device";
    std::vector<BTCommand> commands;

    public:
    void handleBtnPress(int btnNum){};

    String getName(){return name;};
};