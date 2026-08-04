#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

void setupBT(){
    bleKeyboard.begin();
}

void deconnectBT(){
    bleKeyboard.end();
}

void send_bt_key(char key){
    bleKeyboard.write(key);
}