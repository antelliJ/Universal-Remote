#include <Arduino.h>


/*
Taken from https://dronebotworkshop.com/shift-registers/
74LS165 shift register


ABANDON SHIP - the LS uses 5v logic, which is not good for a 3.3v board. I'll just test inputs directly from the pins

*/

// int LOAD = 22; // LOAD pin
// int CLOCK = 23; // CLOCK pin
// int DATA = 21; // DATA pin
// int CLOCK_ENABLE = 19; // CLOCK_INHIBIT pin

// void registerSetup(){

//     pinMode(LOAD, OUTPUT);
//     pinMode(CLOCK, OUTPUT);
//     pinMode(DATA, INPUT);
//     pinMode(CLOCK_ENABLE, OUTPUT);
// }

const uint16_t inputPins[8] = {14, 12, 13, 15, 27, 26, 25, 33};

void setupInputs(){
    for (int i = 0; i < 8; i++){
        pinMode(inputPins[i], INPUT_PULLUP);
    }
}


byte scanInputs(){

    byte data = 0;
    for (int i = 0; i < 8; i++){
        data |= digitalRead(inputPins[i]) << i;
    }
    return data;
}