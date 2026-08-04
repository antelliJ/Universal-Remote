/*     Arduino Rotary Encoder Tutorial
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
 #include <Arduino.h>

 #define outputA 34
 #define outputB 35
 
 #define encoderBtn 32

 int counter = 0; 
 int aState;
 int aLastState;  

 void encoderSetup(){ 
   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);

   pinMode(encoderBtn, INPUT_PULLUP);
   
   Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA);   
 } 

 void readEncoderSignal() { 
   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
       scrollUp();
     } else {
       counter --;
       scrollDown();
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state

   if (digitalRead(encoderBtn) == LOW){
     Serial.println("Encoder Button Pressed");
     encoderButtonPressed();
   }
 }


 void scrollUp(){

 }

 void scrollDown(){
  
 }

 void encoderButtonPressed(){

 }
