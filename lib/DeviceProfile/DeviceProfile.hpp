#pragma once

#include <Arduino.h>
#include <vector>
#include "structs.hpp"


// template <typename T = uint64_t>
class DeviceProfile {
    std::vector<command> commands;
    public:
        int id = 0;
        String name = "Default";
        transmissionModes mode = transmissionModes::IR;
        virtual void handleBtnPress(int btnNum){};
        virtual ~DeviceProfile() = default;

        // don't think these need to be virutal, but will keep it incase something is needed
        virtual int getCommandCount(){return commands.size();};
        virtual std::vector<String> getCommandNames(){
            std::vector<String> names;
            for (int i = 0; i < commands.size(); i++) {
                names.push_back(commands[i].name);
            }
            return names;
        };
};

