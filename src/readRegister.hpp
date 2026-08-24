#include <Arduino.h>


/*
Taken from https://dronebotworkshop.com/shift-registers/
74LS165 shift register


ABANDON SHIP - the LS uses 5v logic, which is not good for a 3.3v board. 
    Or not? I tested it again with a different code so it actually works alright it seems.
    I think the clock inhibit is not a great idea

*/

const int LOAD = 6; // LOAD pin - 14
const int CLOCK = 5; // CLOCK pin - 13
const int DATA = 7; // DATA pin - 21


void registerSetup(){

    #if USE_SHIFT_REG


        pinMode(LOAD, OUTPUT);
        pinMode(CLOCK, OUTPUT);
        pinMode(DATA, INPUT);
        

        digitalWrite(CLOCK, LOW);
        digitalWrite(LOAD, HIGH);
    #else
        // debug single button mode
        pinMode(DATA, INPUT_PULLUP);
    #endif
}

// const uint16_t inputPins[8] = {14, 12, 13, 15, 27, 26, 25, 33};

// void setupInputs(){
//     for (int i = 0; i < 8; i++){
//         pinMode(inputPins[i], INPUT_PULLUP);
//     }
// }


// byte scanInputs(){

//     byte data = 0;
//     for (int i = 0; i < 8; i++){
//         data |= digitalRead(inputPins[i]) << i;
//     }
//     return data;
// }

byte scanRegister() {
    #if USE_SHIFT_REG

        byte data = 0;
        digitalWrite(LOAD, LOW);
        delayMicroseconds(5);
        digitalWrite(LOAD, HIGH);
        delayMicroseconds(5);
        
        data = shiftIn(DATA, CLOCK, MSBFIRST); // doesn't really matter if MSBFIRST or LSBFIRST since I can update functionality later
        
        return data;
    #else

    // single button mode
    return (digitalRead(DATA) == LOW) ? 0x01 : 0x00;

    #endif
}

