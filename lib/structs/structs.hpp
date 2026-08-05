#pragma once

enum class transmissionModes {
    IR,
    BT
};

// struct for the state, contains the current state of the device including IR/BT mode, current profile, current and last page of profile, etc
struct state {
    transmissionModes mode = transmissionModes::IR;
    int currentProfile = 0;
    int currentPage = 0;
};