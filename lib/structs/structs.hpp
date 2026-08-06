#pragma once
#include <variant>
#include <vector>

#include <IRsend.h>
#include <IRrecv.h>
#include <Adafruit_SSD1306.h>


enum class transmissionModes {
    IR,
    BT
};


// I don't think this is how I should do this
// template <typename T>
struct command {
    String name;
    std::variant<uint64_t, uint16_t*, std::vector<char>> data;
    // T data;
};

// struct for the state, contains the current state of the device including IR/BT mode, current profile, current and last page of profile, etc
struct state {
    transmissionModes mode = transmissionModes::IR;
    int currentProfile = 0;
    int currentPage = 0;
    bool selectingProfile = false;
};

// Pointer to special devices and classes (IR receiver and transmitter, screen, etc)
struct devicePointers {
    IRrecv* irReceiver = nullptr;
    IRsend* irTransmitter = nullptr;
    Adafruit_SSD1306* screen = nullptr;
};