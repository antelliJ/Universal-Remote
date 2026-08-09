#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>


#include "structs.hpp"
#include "state.hpp"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// use font 1
// size 1 is like a 5x7 font (6x8 w/ spacing)
// size 2 is like a 10x16 font
#define FONT_SIZE 1

// use pins 4 and 5 -- I think these are safe for the c3 board
#define SDA_PIN 14
#define SCL_PIN 13

const uint8_t ADDRESS = 0x3C; // 0x3C for 128x64, may be 0x3D?




Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void drawScreen();
void cursorAlignText(int curEntry);

int charHeight(int size) {
    if (size == 1) {
        return 8;
    } else {return 16;}
};

int charWidth(int size) {
    if (size == 1) {
        return 6;
    } else {return 10;}
};


void screenSetup(){
    Wire.begin(SDA_PIN, SCL_PIN);

    // initialize the OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, ADDRESS)) {
        Serial.println("SSD1306 allocation failed");
        for (;;)
            ;
    }
    display.clearDisplay();
    display.setTextWrap(false); // should make it so things don't overlap

    drawScreen();
    delay(1000);
}

// draw a title at the center top of the screen
void drawTitle(String title) {
    int16_t x1,y1; // don't rlly care abt these
    uint16_t w, h;
    display.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 0);
    display.print(title);
}

void updateScreen(state* currentState, bool drawSlow=false) {
    display.setTextSize(FONT_SIZE);
    display.setTextColor(WHITE);
    display.clearDisplay();

    if (currentState->selectingProfile) { // for profile selection

    } else { // profile chosen, for command selection
        // name of profile at top center of screen
        String name = currentState->currentProfile->name;
        drawTitle(name);
        if (drawSlow){
            display.display();
            delay(1000); // TEMP delay each step
        }
    
        Serial.println("now drawing commands");
        // commands ordered throughout
        for (int i = 0; i < 8; i++) {
            if (currentState->availableCommands[i]) {
                cursorAlignText(i);
                String cmdName = currentState->availableCommands[i]->getName();
                display.print(cmdName.substring(0, 10)); //0-10 chars
                // Calculte name instead
                // NEED TO DO CHECK TO SEE IF COMMAND EXISTS
                // String name = currentState->currentProfile->getCommands()[i + currentState->currentPage*8].getName();
                if (drawSlow){
                    display.display();
                    delay(1000); // TEMP delay each step
                }
            }
            // command curCmd = *currentState.availableCommands[i];
        }

        Serial.println("now drawing arrows");
        // arrows displayed if necessary on right
        if (currentState->currentPage > 0) { // draw < arrow
            // display.setCursor(SCREEN_WIDTH-2*charWidth(FONT_SIZE), 16);
            display.setCursor(0, SCREEN_HEIGHT-2*charHeight(FONT_SIZE));
            display.print("<");
            if (drawSlow){
                display.display();
                delay(1000); // TEMP delay each step
            }
        }
        if (currentState->currentPage < currentState->lastPage) { // draw > arrow
            // display.setCursor(SCREEN_WIDTH-2*charWidth(FONT_SIZE), 32);
            display.setCursor(SCREEN_WIDTH-2*charWidth(FONT_SIZE), SCREEN_HEIGHT-2*charHeight(FONT_SIZE));
            display.print(">");
            if (drawSlow){
                display.display();
                delay(1000); // TEMP delay each step
            }
        }

        Serial.println("now drawing selection box");
        // draw selection box
        cursorAlignText(currentState->selectionCursor % 8);
        
        display.drawRect(display.getCursorX(), display.getCursorY(), SCREEN_WIDTH/2, charHeight(FONT_SIZE), WHITE);
    }
    Serial.println("now displaying screen");
    display.display();
}


void drawScreen() {
    display.setTextSize(FONT_SIZE);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Hello, world!");
    display.display();
}

// align the text of however many entries there are dynamically, 2 per line
void cursorAlignText(int curEntry) {
    int potentialEntries = SCREEN_HEIGHT/(charHeight(FONT_SIZE));
    // int curRow = curEntry/(potentialEntries/2); // this is just completely wrong? why did I do that
    int curRow = (curEntry/2)+1;
    int curCol = (curEntry)%2;

    int x = curCol * SCREEN_WIDTH/2;
    int y = curRow*(charHeight(FONT_SIZE)+3);
    display.setCursor(x, y);
}

void drawCommands(command commands[], int numCommands) {
    display.setTextSize(FONT_SIZE);
    display.setTextColor(WHITE);
    for (int i=0; i<numCommands; i++) {
        cursorAlignText(i);
        display.print(commands[i].name);
    }

    display.display();
}