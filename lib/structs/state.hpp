#pragma once

#include "structs.hpp"

#include "DeviceProfile.hpp"


// struct for the state, contains the current state of the device including IR/BT mode, current profile, current and last page of profile, etc
struct state {
    transmissionModes mode = transmissionModes::IR;
    // int currentProfile = 0;
    DeviceProfile* currentProfile = nullptr;
    uint8_t currentPage = 0;
    uint8_t lastPage = 0;
    bool selectingProfile = false;
    command* availableCommands[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    DeviceProfile* availableProfiles[4] = {nullptr, nullptr, nullptr, nullptr};
    int selectionCursor = 0; // from 0-(len-1) of cmds/profiles
};

