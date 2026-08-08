#include <vector>
#include <Arduino.h>
#include "IRDeviceProfile.hpp"


// String name, IRProtocol protocol, std::vector<IRCommand> commands
std::vector<IrDeviceProfile*> IRProfiles = {
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


    


    new IrDeviceProfile("tv", IRProtocol::RAW, { //commands
        {"pwr", std::vector<uint16_t>
            {232, 1860,  228, 806,  184, 854,  180, 854,  180, 882,  136, 900,  136, 1922,  230, 1860,  230, 806,  182, 1906,  230, 808,  180, 856,  180, 882,  138, 1924,  232, 804,  184, 46302,  182, 1906,  228, 808,  184, 854,  178, 856,  184, 882,  136, 1926,  224, 810,  180, 856,  180, 1908,  226, 810,  186, 1902,  228, 1864,  180, 1904,  228, 812,  180, 1908,  226, 44156,  222, 1862,  228, 838,  136, 900,  132, 902,  134, 874,  182, 884,  132, 1930,  222, 1892,  138, 870,  182, 1934,  182, 828,  180, 856,  180, 856,  176, 1912,  180, 856,  180}
        },
    }),

    // new IrDeviceProfile(),
    // new IrDeviceProfile(),
    // new IrDeviceProfile()
};