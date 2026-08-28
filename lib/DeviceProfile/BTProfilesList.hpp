#include <vector>
#include "BTDeviceProfile.hpp"
#include <BleKeyboard.h>

// special keys probably useful
const uint8_t K_L_CTRL = 0x80;
const uint8_t K_L_SHIFT = 0x81;
const uint8_t K_L_ALT = 0x82;
const uint8_t K_RETURN = 0xB0;
const uint8_t K_UP = 0xDA;
const uint8_t K_DOWN = 0xD9;
const uint8_t K_LEFT = 0xD8;
const uint8_t K_RIGHT = 0xD7;
const uint8_t K_F1 = 0xC2;
const uint8_t K_F2 = 0xC3;
const uint8_t K_F3 = 0xC4;
const uint8_t K_F4 = 0xC5;
const uint8_t K_F5 = 0xC6;
const uint8_t K_F6 = 0xC7;
const uint8_t K_F7 = 0xC8;
const uint8_t K_F8 = 0xC9;
const uint8_t K_F9 = 0xCA;
const uint8_t K_F10 = 0xCB;
const uint8_t K_F11 = 0xCC;
const uint8_t K_F12 = 0xCD;
const uint8_t K_F13 = 0xF0;
const uint8_t K_F14 = 0xF1;
const uint8_t K_F15 = 0xF2;
const uint8_t K_F16 = 0xF3;
const uint8_t K_F17 = 0xF4;
const uint8_t K_F18 = 0xF5;
const uint8_t K_F19 = 0xF6;
const uint8_t K_F20 = 0xF7;
const uint8_t K_F21 = 0xF8;
const uint8_t K_F22 = 0xF9;
const uint8_t K_F23 = 0xFA;
const uint8_t K_F24 = 0xFB;


const MediaKey MEDIAKEY_NEXT = {{1,0}};
const MediaKey MEDIAKEY_PREVIOUS = {{2,0}};

const MediaKey MEDIAKEY_PLAY_PAUSE = {{8,0}};
const MediaKey MEDIAKEY_VOLUME_UP = {{32,0}};
const MediaKey MEDIAKEY_VOLUME_DOWN = {{64,0}};
const MediaKey MEDIAKEY_MUTE={{16,0}};

std::vector<DeviceProfile*> BTProfiles = {
    new BTDeviceProfile(
        "media", 
        std::vector<BTCommand>{// commands
        {"play", MEDIAKEY_PLAY_PAUSE}, //KEY_MEDIA_PLAY_PAUSE
        {"vol up", MEDIAKEY_VOLUME_UP}, // KEY_MEDIA_VOLUME_UP
        {"vol down", MEDIAKEY_VOLUME_DOWN}, // KEY_MEDIA_VOLUME_DOWN
    }
    ),

    new BTDeviceProfile(
        "shortcuts",
        std::vector<BTCommand>{
            {"new tab", std::vector<uint8_t>{K_L_CTRL, (uint8_t)'t'}},
            {"a", (uint8_t)'a'},
            {"abc", std::vector<uint8_t>{(uint8_t)'a', (uint8_t)'b', (uint8_t)'c'}, false},
        }
    ),

    // I could make this as its own enum type but that seems a bit overkill for this example
    new BTDeviceProfile(
        "Settings", {
            {"Reconnect", 0}
        }
    ),
    new BTDeviceProfile(
        "TikTok", { 
            // up down, play
            {"up", K_UP},
            {"down", K_DOWN},
            {"play", MEDIAKEY_PLAY_PAUSE},
        }
    )
};