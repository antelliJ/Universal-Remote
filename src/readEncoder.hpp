/*     Arduino Rotary Encoder Tutorial
*      
*  by Dejan Nedelkovski, www.HowToMechatronics.com


sorry Dejan but I'm stealing from https://www.build-electronic-circuits.com/arduino-rotary-encoder/
now
*  
*/
#include <Arduino.h>

#define outputA 1
#define outputB 0

#define encoderBtn 3



#define ENCODER_SCROLLUP 0x01
#define ENCODER_SCROLLDOWN 0x02
#define ENCODER_BTN 0x04


// int encoderTicks = 0; 
volatile int encoderTicks = 0;
int aState;
// int aLastState;  
volatile uint8_t lastEncoded = 0;
volatile uint8_t encoderEvents = 0;

void scrollUp();
void scrollDown();
void encoderButtonPressed();

void updateEncoder();

bool lastBtnState = HIGH;


void encoderSetup(){ 
  pinMode (outputA,INPUT_PULLUP);
  pinMode (outputB,INPUT_PULLUP);

  pinMode(encoderBtn, INPUT_PULLUP);
  
  lastEncoded = (digitalRead(outputA) << 1) | digitalRead(outputB); // Reads the initial state of the outputA and outputB

  attachInterrupt(
    digitalPinToInterrupt(outputA),
    updateEncoder,
    CHANGE
  );

  attachInterrupt(
    digitalPinToInterrupt(outputB),
    updateEncoder,
    CHANGE
  );
} 



void IRAM_ATTR updateEncoder(){

  uint8_t MSB = digitalRead(outputA); // MSB = most significant bit
  uint8_t LSB = digitalRead(outputB);

  uint8_t encoded = (MSB << 1) | LSB; // convert to 1 num
  uint8_t sum = (lastEncoded << 2) | encoded;

  if ( // clockwise?
    sum == 0b1101 ||
    sum == 0b0100 ||
    sum == 0b0010 ||
    sum == 0b1011 
  ) {
    encoderTicks++;
    // Serial.println("tick ++");
    // encoderEvents |= ENCODER_SCROLLUP;
  }

  if ( //ccw
    sum == 0b1110 ||
    sum == 0b0111 ||
    sum == 0b0001 ||
    sum == 0b1000
  ) {
    encoderTicks--;
    // Serial.println("tick --");
    // encoderEvents |= ENCODER_SCROLLDOWN;
  }

  lastEncoded = encoded;
}

byte readEncoderSignal() { 
  byte data = 0;

  int ticks;
  
  noInterrupts();

  // data = encoderEvents;
  // encoderEvents = 0;

  ticks = encoderTicks;
  if (encoderTicks >= TICKS_PER_SCROLL) {
    encoderTicks -= TICKS_PER_SCROLL;
    // data |= ENCODER_SCROLLUP;
    ticks = TICKS_PER_SCROLL;
  } else if (encoderTicks <= -TICKS_PER_SCROLL) {
    encoderTicks += TICKS_PER_SCROLL;
    ticks = -TICKS_PER_SCROLL;
  } else {
    ticks = 0;
  }

  interrupts();

  if (ticks > 0) {
    // scrollUp();
    data |= ENCODER_SCROLLUP;
    Serial.println("scrolling up");
  }else if (ticks < 0) {
    data |= ENCODER_SCROLLDOWN;
    Serial.println("scrolling down");
  }

  bool curBtnState = digitalRead(encoderBtn);

  if (lastBtnState == HIGH && curBtnState == LOW) {
    data |= ENCODER_BTN;
    encoderButtonPressed();
  }
  lastBtnState = curBtnState;

  return data;
}


void scrollUp(){

}

void scrollDown(){
 
}

void encoderButtonPressed(){

}
