#include <vector>
#include <Arduino.h>
#include "IRDeviceProfile.hpp"


// String name, IRProtocol protocol, std::vector<IRCommand> commands
std::vector<DeviceProfile*> IRProfiles = {
    new IrDeviceProfile("lights", IRProtocol::NEC, {// commands
        {"pwr", 0xFF02FD},
        {"purple", 0xFF7887},
        {"breathe", 0xFF12ED},
        {"sound thing", 0xFF32CD},
        {"gentle fade", 0xFF609F},
        {"faster fade", 0xFFE01F},
        {"strong sound", 0xFFD827},
        {"party ig", 0xFFC837},
        {"flash dark", 0xFFD02F},
        {"flash rgb", 0xFF20DF},
    }),


    


    new IrDeviceProfile("tv", IRProtocol::SHARP, { //commands
        // {"pwr", std::vector<uint16_t>
        //     {232, 1860,  228, 806,  184, 854,  180, 854,  180, 882,  136, 900,  136, 1922,  230, 1860,  230, 806,  182, 1906,  230, 808,  180, 856,  180, 882,  138, 1924,  232, 804,  184, 46302,  182, 1906,  228, 808,  184, 854,  178, 856,  184, 882,  136, 1926,  224, 810,  180, 856,  180, 1908,  226, 810,  186, 1902,  228, 1864,  180, 1904,  228, 812,  180, 1908,  226, 44156,  222, 1862,  228, 838,  136, 900,  132, 902,  134, 874,  182, 884,  132, 1930,  222, 1892,  138, 870,  182, 1934,  182, 828,  180, 856,  180, 856,  176, 1912,  180, 856,  180}
        // },
        {"pwr", 0x41A2}, 
        {"vol up", 0x40A2},
        {"vol down", 0x42A2},
        {"input", 0x4322},
        {"mute", 0x43A2},
        {"up", 0x43AA},
        {"down", 0x406A},
        {"left", 0x42BE},
        {"right", 0x41BE},
        {"ok", 0x43BE},
    }),

    new IrDeviceProfile("tv2", IRProtocol::RC5, { //commands
        {"pwr", 0xB8C},
        {"up", 0x3BD},
        {"down", 0xBBE},
        {"left", 0x3BF},
        {"right", 0xBAB},
        {"ok", 0x3AC},
        {"vol up", 0x392},
        {"vol down", 0xB93},
        {"mute", 0xBB0},
        {"home", 0x38D},
        {"back", 0xB8F},
        {"pause/play", 0x38B},
        {">>", 0xB90},
        {"<<", 0x396},
        {"keyboard", 0xB98},
        {"f1-r", 0x3B2},
        {"f2-g", 0xBB1},
        {"f3-y", 0x3B9},
        {"f4-b", 0xBBA},
        {"stop", 0x399},
    }),

    new IrDeviceProfile("Sony", IRProtocol::SONY,
    {
        {"pwr", 0x540A},
        {"vol up", 0x240A},
        {"vol down", 0x640A},
        {"function", 0x4B0B},
        {"up", 0x1E108},
        {"down", 0x9E108},
        {"left", 0x5E108},
        {"right", 0xDE108},
        {"mute", 0x140A},
        {"tv input", 0x4322},
        {"home", 0x700B},
        {"return", 0xBE108},
        {"options", 0xCE108},
        {"pop up menu", 0x58108},
        {"top menu", 0x98108},
        {"subtitle", 0x88108},
        {"sound mode", 0x690B},
        {"pause", 0x400B},
        {"play", 0x200B},
        {"fwd", 0x2C108},
        {"bkwd", 0xCC108},
        {"skip >", 0x8C108},
        {"skip <", 0xC108},
    }),

    new IrDeviceProfile("Settings", IRProtocol::SETTINGS,
    {
        {"IR Dump", 0x000000},
    })

    // new IrDeviceProfile(),
    // new IrDeviceProfile(),
    // new IrDeviceProfile()
};