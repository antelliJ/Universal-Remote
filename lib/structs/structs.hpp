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
    // std::variant<uint64_t, uint16_t*, std::vector<char>> data;
    command(String name) : name(name) {}
    command(){}
    virtual ~command() = default;
    const String& getName() const {return name;};
    // T data;
};



// Pointer to special devices and classes (IR receiver and transmitter, screen, etc)
struct devicePointers {
    IRrecv* irReceiver = nullptr;
    IRsend* irTransmitter = nullptr;
    Adafruit_SSD1306* screen = nullptr;
};