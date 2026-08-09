#pragma once

#include <Arduino.h>
#include <vector>
#include "structs.hpp"


// template <typename T = uint64_t>
class DeviceProfile {
    // std::vector<command> commands;
    public:
        int id = 0;
        String name = "Default";
        transmissionModes mode = transmissionModes::IR;
        virtual void handleBtnPress(int btnNum){};
        DeviceProfile(String name, transmissionModes mode) : name(name), mode(mode) {};
        virtual ~DeviceProfile() = default;

        // removed command vector so each profile will deal with
        // it on its own
        virtual int getCommandCount() = 0;
        // virtual std::vector<String> getCommandNames(){
        //     std::vector<String> names;
        //     for (int i = 0; i < commands.size(); i++) {
        //         names.push_back(commands[i].name);
        //     }
        //     return names;
        // };

        // vector needs the specific type but it breaks so I'll make a method
        // just to return command at an index
        // virtual std::vector<command>& getCommands(){return commands;};

        virtual command* getCommand(int index)=0;
};

