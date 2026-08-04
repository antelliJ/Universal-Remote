#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// use font 1
// size 1 is like a 5x7 font (6x8 w/ spacing)
// size 2 is like a 10x16 font
#define FONT_SIZE 1

// use pins 4 and 5 -- I think these are safe for the c3 board
#define SDA_PIN 4
#define SCL_PIN 5

const uint8_t ADDRESS = 0x3C; // 0x3C for 128x64, may be 0x3D?



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void screenSetup(){
    Wire.begin(SDA_PIN, SCL_PIN);

    // initialize the OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, ADDRESS)) {
        Serial.println("SSD1306 allocation failed");
        for (;;)
            ;
    }
    display.clearDisplay();

    drawScreen();
    delay(1000);
}

void drawScreen() {
    display.setTextSize(FONT_SIZE);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Hello, world!");
    display.display();
}