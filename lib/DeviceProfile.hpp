#include <Arduino.h>
#include "structs.hpp"

class DeviceProfile {
    public:
        int id = 0;
        String name = "Default";
        transmissionModes mode = transmissionModes::IR;
        virtual void handleBtnPress(int btnNum){};
        virtual ~DeviceProfile() = default;
};