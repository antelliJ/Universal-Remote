#include <BleKeyboard.h>
#include <vector>
#include "BTDeviceProfile.hpp"

BleKeyboard bleKeyboard("Universal Remote");

void setupBT(){
    bleKeyboard.begin();
}

void disconnectBT(){
    bleKeyboard.end();
}

void send_bt_key(uint8_t key){
    bleKeyboard.write(key);
}

// I should keep it as const since they're not being updated
void send_bt_key(const MediaKey& key){
    MediaKeyReport mediaKeyReport = {
        key.data[0],
        key.data[1]
    };
    bleKeyboard.write(mediaKeyReport);
}

void send_bt_keys_simultaneously(const std::vector<uint8_t>& keys){
    for (int i = 0; i < keys.size(); i++){
        bleKeyboard.press(keys[i]);
    }
    bleKeyboard.releaseAll();
}

void send_bt_keys_order(const std::vector<uint8_t>& keys) {
    for (int i=0; i < keys.size(); i++) {
        send_bt_key(keys[i]);
    }
}

void reconnectBluetooth() {
    Serial.println("Restarting BLE device...");

    disconnectBT();
    delay(200);
    setupBT();

    Serial.println("Restarted BLE device");
}

void handleBTCommand(BTCommand* cmd){
    //double check that its not nullptr
    if (cmd == nullptr) {
        return;
    }

    // this is kinda a bandage solution but it should be fine? I only want 1 setting
    if (cmd->type == BTCommandType::SETTING) {
        //handle settings
        if (cmd->name == "reconnect") {
            reconnectBluetooth();
        }
        return;
    }

    if (cmd->type == BTCommandType::MEDIA) { // is type MediaKey
        MediaKey data = std::get<MediaKey>(cmd->data);
        send_bt_key(data);

    } else { // is BTCommandType::KEYBOARD
        // check if its a single key
        if (std::holds_alternative<uint8_t>(cmd->data)) {
            send_bt_key(std::get<uint8_t>(cmd->data));
        }
        else {
            // its a vector
            std::vector<uint8_t> data = std::get<std::vector<uint8_t>>(cmd->data);
            if (cmd->sync_keypress) {
                send_bt_keys_simultaneously(data);
            } else {
                send_bt_keys_order(data);
            }
        }
    }
}