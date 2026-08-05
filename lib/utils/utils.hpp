#include <Arduino.h>

void debugPrintByte(byte byteVar) {
    for (int i=7; i>=0; i--) {
        Serial.print(bitRead(byteVar, i));
    }
    Serial.println();
}