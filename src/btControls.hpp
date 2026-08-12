#include <BleKeyboard.h>
#include <vector>

BleKeyboard bleKeyboard;

void setupBT(){
    bleKeyboard.begin();
}

void disconnectBT(){
    bleKeyboard.end();
}

void send_bt_key(char key){
    bleKeyboard.write(key);
}

void send_bt_keys_simultaneously(std::vector<char> keys){
    for (int i = 0; i < keys.size(); i++){
        bleKeyboard.press(keys[i]);
    }
    bleKeyboard.releaseAll();
}

void send_bt_keys_order(std::vector<char> keys) {
    for (int i=0; i < keys.size(); i++) {
        bleKeyboard.write(keys[i]);
    }
}