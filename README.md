# ESP 32 Universal Remote
https://github.com/antelliJ/Universal-Remote

A project aimed at replicating IR signals for media communication and the ability to act as a bluetooth keyboard, with extensible elements available.

This project is built with an ESP32-C3 as the main heart and brains of the system

![Hero shot of remote in crappy wooden shell](<assets/BetterRemoteBuild.png>)


[![Watch the video of all features](https://youtu.be/WP4MpGtz5AQ)](https://youtu.be/WP4MpGtz5AQ)



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
- [x] Optional Shift register (74HC165) for multiple button input (most efficient)
- [x] Single button input for testing or if register is unavailable
- [x] Serial interface for testing
- [x] Extensible nature of devices profiles and other communication

Potential features for the future:
- [] Internal IR signal recording
    - Currently requires serial monitoring and recompilation of the source to add your own signals.
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

On startup the display says hello world before loading the first profile.

### Schematic

The rotary encoder, OLED display, IR transmitter and receiver are connected in the same way, whether or not a shift register is used, and the wiring is as seen in the schematic.

#### With Shift Register

![Schematic showing connections from ESP32-C3 to the shift register and all other components](<assets/schematic v1-0.png>)

The recommended configuration uses a PISO (parallel-in, serial-out) shift register to get more inputs than what the microcontroller supports.

The actual connection uses
| Signal            |     | ESP32-C3 GPIO |
| ------            | --- | ------------- |
| Clock             |     | GPIO 5        |
| (parallel) Load In|     | GPIO 6        |
| Serial Out        |     | GPIO 7        |

Button inputs are connected to the input pins of the shift register, where each pin should have a respective pullup / pulldown resistor (I intended on using a pulldown resistor)

#### With Single Input
![Schematic showing connections from ESP32-C3 to a single button on GPIO 7, with the same connections to all other components](<assets/schematic v1-1.png>)

For testing or if you don't have a PISO shift register, the code allows for a single button input to be connected to GPIO 7 (and is pulled properly - I used the internal pullup resistors here. So my button is connected to GPIO 7 and GND)

This option uses the rotary encoder to select the command while the single button actually executes that chosen command. I found this system of traversal to be much more clunky that the other option.

### Reading IR Signals
The IRremoteESP8266 does the heavy lifting in reading IR signals from the receiver. 

Protocols that the library recognizes can be easily stored with just a little addition in the project to make room. If it isn't recognized, raw timings can be used, where they are stored as a vector of `uint16_t` duration pulses of the IR light in microseconds


<br>

To be able to read what is received, first connect the board to the PC and open the serial monitor (at a baud rate of 115200)

Enter the IR dump / read mode by entering the settings in the IR profiles list, and executing the "IR Dump" command

Whatever signals are received will be printed in a human readable signal hex code (if the protocol is recognized), otherwise a list of the timings is printed

### Adding IR Protocols 
These can then be stored as an IRCommand as follows (Perhaps in IRProfilesList.hpp)
```
//example for raw timings:
{
    "power",
    std::vector<uint16_t>{
        232, 1860,  228, 806,  184, 854,  
        180, 854,  180, 882,  136, 900,
        ...
    }
}

//example for recognized protocol
{"vol up", 0x40A2}
```

These commands that contain the data can be used in the definitions for the profiles

```
new IrDeviceProfile(
    "tv",
    IRProtocol::SHARP, // ::RAW, ::NEC, etc. 
    {
        ... // commands go here
    }
)

```

### Creating bluetooth commands

These commands are represented with `BTCommand`. Commands may contain a single key or a collection of multiple keys. There is an argument if there are multiple keys whether they should be pressed consecutively or simultaneously.

Special media keys can also be used

```

    new BTDeviceProfile(
        "shortcuts",
        std::vector<BTCommand>{
            {"new tab", std::vector<uint8_t>{K_L_CTRL, (uint8_t)'t'}, true}, // true is optional since its the default, setting whether keypress is synced
            {"a", (uint8_t)'a'},
            {"vol up", MEDIAKEY_VOLUME_UP}, // KEY_MEDIA_VOLUME_UP
        }
    ),
```


### Utilizing a Shift Register (Recommended)
This project works with a PISO shift register (74HC165 - I got it working with a 74LS165) connected to GPIO pins 5,6, and 7, as seen in the schematic.

Enable this feature in the compiler flags, or through the `config.hpp` file (set USE_SHIFT_REG to 1)
    `#define USE_SHIFT_REG 1`

Ensure the pinout is correct as per `readEncoder.hpp` (Written in the schematic section)

The state of the button is returned as a byte, where the rest of the program can process what these states mean


### Using a single button for input (Simpler wiring)
but if unavailable the rotary encoder and a single button connected to GPIO 7 (and an internal pullup resistor) is able to be used

Disable the shift register functionality in the compiler flags, or through the `config.hpp` file (set USE_SHIFT_REG to 0) 
    `#define USE_SHIFT_REG 0`

the input is then configured as `pinMode(7, INPUT_PULLUP);`, as the button is connected to GPIO 7 and GND. This means the button is active when LOW.

The rotary encoder selects the active command, and the button executes the active command, and is more cumbersome to use than with the shift register.

### Serial API

Tools are available for debugging and injecting certain command data and simulated inputs

Communication is at <b>115200 baud</b>

#### Simulated Button Presses
| Command               |     | Simulated Action                  |
| ------                | --- | -------------                     |
| scrollup              |     | Rotary Encoder Scroll up          |
| scrolldown            |     | Rotary Encoder Scroll down        |
| toggle                |     | Rotary Encoder Scroll button press|
| btn{0-7} -> ex. btn5  |     | Shift Register Button Press       |


#### Debug Prints
| Command               |     | Output                            |
| ------                | --- | -------------                     |
| CurrentState          |     | Prints the values of the CurrentState Variable, used throughout the program           |

The project also outputs some information at startup and throughout operation for how processes are going, such as what IR messages are being transmitted, the amount of commands loaded, etc.


### Flashing Precompiled firmware
Precompiled firmware is available in the
[GitHub Releases](https://github.com/antelliJ/Universal-Remote/releases).

There are 2 configs provided:
| Firmware               |     | Input Method                  |
| ------                | --- | -------------                     |
| UniversalRemote-shiftreg-merged.bin              |     | 74HC165/74LS165 shift register          |
| UniversalRemote-single-merged.bin              |     | Single Button Input on GPIO 7          |

### Flashing with esp tool
Download the desired `.bin` file release
Connect your ESP32-C3 to your computer with USB
Put the microcontroller into uploading mode (if necessary)

Install and use `esptool`: (replace COMx with the COM serial port of your ESP32-C3)

(Make sure you have python installed)

```bash
pip install esptool

python -m esptool --chip esp32c3 --port COMx erase_flash

python -m esptool --chip esp32c3 --port COMx write_flash 0x0 UniversalRemote-shift-register.bin

OR FOR THE SINGLE BUTTON

python -m esptool --chip esp32c3 --port COMx write_flash 0x0 UniversalRemote-single-button.bin
```


### Instructions with building if using PlatformIO
Clone the repository

Build the project with either of the respective environments:

```
pio run -e esp32-c3-shiftreg

pio run -e esp32-c3-singlebtn
```

</br></br>
If you want to build and upload in one command:
```
pio run -e esp32-c3-shiftreg -t upload

pio run -e esp32-c3-singlebtn -t upload
```


#### To make the merged files yourself:
Clone the repository

Build the files for the respective platform you're using with platformio

##### FOR esp32-c3-shiftreg

`pio pkg exec -p tool-esptoolpy -- esptool.py --chip esp32c3 merge_bin -o merged.bin --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 .pio/build/esp32-c3-shiftreg/bootloader.bin 0x8000 .pio/build/esp32-c3-shiftreg/partitions.bin 0x10000 .pio/build/esp32-c3-shiftreg/firmware.bin`

##### FOR esp32-c3-singlebtn

`pio pkg exec -p tool-esptoolpy -- esptool.py --chip esp32c3 merge_bin -o merged.bin --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 .pio/build/esp32-c3-singlebtn/bootloader.bin 0x8000 .pio/build/esp32-c3-singlebtn/partitions.bin 0x10000 .pio/build/esp32-c3-singlebtn/firmware.bin`

^ This command was generated by ChatGPT, but it did produce a working file without errors so it seems to work?


This creates a merged.bin file that can be used to upload to the microcontroller (using the steps used previously for the release builds)

### AI Usage Declaration
- Code autocomplete
- AI summary of unfamiliar c++ concepts
- Terminal commands specific to PlatformIO
- Debugging issues with libraries