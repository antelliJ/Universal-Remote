#include <vector>
#include "BTDeviceProfile.hpp"
#include <BleKeyboard.h>


std::vector<DeviceProfile*> BTProfiles = {
    new BTDeviceProfile("media", {// commands
        {"play", {(char)8, (char)0}}, //KEY_MEDIA_PLAY_PAUSE
    }
)
};