#include <Arduino.h>


/*
Taken from https://dronebotworkshop.com/shift-registers/
74LS165 shift register


ABANDON SHIP - the LS uses 5v logic, which is not good for a 3.3v board. 
    Or not? I tested it again with a different code so it actually works alright it seems.
    I think the clock inhibit is not a great idea

*/

int LOAD = 14; // LOAD pin - 14
int CLOCK = 13; // CLOCK pin - 13
int DATA = 21; // DATA pin - 21


void registerSetup(){

    pinMode(LOAD, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(DATA, INPUT);
    

    digitalWrite(CLOCK, LOW);
    digitalWrite(LOAD, HIGH);
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
    byte data = 0;
    digitalWrite(LOAD, LOW);
    delayMicroseconds(5);
    digitalWrite(LOAD, HIGH);
    delayMicroseconds(5);
    
    data = shiftIn(DATA, CLOCK, MSBFIRST); // doesn't really matter if MSBFIRST or LSBFIRST since I can update functionality later
    
    return data;
}

