// include vectors
#include <vector>
#include <Arduino.h>
#include "structs.hpp"
#include "DeviceProfile.hpp"

// command struct has name, and data
struct IRCommand {
    String name;
    uint64_t data;
};


// enum for different IR protocols
enum class IRProtocol {
    NEC,
    RAW,
    SONY,
    UNKNOWN
};


class IrDeviceProfile : public DeviceProfile {
    transmissionModes mode = transmissionModes::IR;
    String name = "IR Device";
  
    public:
    void handleBtnPress(int btnNum){};
};
