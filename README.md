# ESP 32 Universal Remote
https://github.com/antelliJ/Universal-Remote

A project aimed at replicating IR signals for media communication and the ability to act as a bluetooth keyboard, with extensible elements available.

This project is built with an ESP32-C3

<strong> Libraries used: </strong>
- [IRremoteESP8266] (https://github.com/crankyoldgit/irremoteesp8266 "Github Project")
- [Adafruit SSD1306] (https://github.com/adafruit/adafruit_ssd1306) & [Adafruit GFX Library] (https://github.com/adafruit/Adafruit-GFX-Library)
- [ESP32 BLE Keyboard] (https://github.com/T-vK/ESP32-BLE-Keyboard "Github Project")


## Features
- [x] IR Transmission with common protocols like NEC, SONY, etc (any available through IRremoteESP8266)
- [x] Raw IR Transmission if using a protocol that hasn't been decoded
- [x] IR & BT Command and profile interface to store and switch between multiple devices and commands
- [x] BLE Bluetooth Keyboard functionality
    -   Compatible with major operating systems
- [x] OLED screen to view available profiles and commands
- [x] Rotary encoder for selection and navigation
- [x] Optional Shift register (74HS165) for multiple button input (most efficient)
- [x] Single button input for testing or if register is unavailable
- [x] Serial interface for testing
- [x] Extensible nature of devices profiles and other communication

- [] Internal IR signal recording
    - Currently requires serial monitoring.
- [] Profiles and Commands stored in a file system (littleFS) / external storage
    - All commands and profiles currently stored in 

    <pre>
    |--lib
    |  | 
    |  |--DeviceProfile
    |  |  |--IRProfilesList.hpp
    |  |  |--BTProfilesList.hpp
    </pre>
    - If implemented could potentially update order and inclusion of new commands wirelessly
 
## Usage
The remote is built around <b>device profiles</b>. These group commands of a device into a single folder of sorts. This can be a profile for a TV, for lights, and AC unit, and would have all the commands that can be sent to it.

The base `DeviceProfile` is used in:

- `IRDeviceProfile` > for IR devices
- `BTDeviceProfile` > Bluetooth HID devices


More may be added later (a more robust Settings profile, secret options, etc.)

Commands similarly are stored in the respective profiles and use the `command` base class

### Schematic

#### With Shift Register

The recommended configuration uses a PISO (parallel-in, serial-out) shift register to get more inputs than what the microcontroller supports.

#### With Single Input


### Reading IR Signals


### Adding IR Protocols 


### Creating bluetooth commands

### Utilizing a Shift Register (Recommended)
This project works with a PISO shift register (74HS165) connected to GPIO pins 5,6, and 7, as seen in the schematic.

Enable this feature in the compiler flags, or through the `config.hpp` file (set USE_SHIFT_REG to 1)


### Using a single button for input
but if unavailable the rotary encoder and a single button connected to GPIO 7 (and an internal pullup resistor) is able to be used

Disable the shift register functionality in the compiler flags, or through the `config.hpp` file (set USE_SHIFT_REG to 0)

### Serial API

#### Simulated Button Presses

#### Debug Prints