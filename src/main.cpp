#include <Arduino.h>
#include "encoder.hpp"
#include "button.hpp"
#include "Keyboard.h"

#define BUTTON_1  3
#define ENCODER_A 7
#define ENCODER_B 8

// SET IT TO 1 for INTERRUPT MODE 0 to NON INTERRUPT MODE
#define INTTERUPT_MODE 1

// Decleare object pointers in source file scope.
Encoder *enc_1;
Button *bt_1;

/* For encoder there are three functions. One is for connect it to a hardware interrupt callback 
 * The two other is called by the Object itself depending on we are turning the encoder right or left.
 * Im using an encoder what has two signals / mechanical ticks, so modifie it if yours is different.
 */
void encoder1_callback(void);
void encoder1_foward(void);
void encoder1_backward(void);

/* For button there are three functions. One is for connect it to a hardware change interrupt callback.
 * The other two is called by the Object itself depending on we are pressing or releasing the button.
 */
void button1_callback(void);
void button1_pressed(void);
void button1_released(void);

void setup()
{
  Serial.begin(9600);
  
  #if INTTERUPT_MODE
    enc_1 = new Encoder(ENCODER_A, ENCODER_B, 5, encoder1_callback, encoder1_foward, encoder1_backward);
    bt_1 = new Button(BUTTON_1, BUTTON_TYPE_STEADY_HIGH, 20, button1_callback, button1_pressed, button1_released);
  #else
    enc_1 = new Encoder(ENCODER_A, ENCODER_B, 5);
    bt_1 = new Button(BUTTON_1, BUTTON_TYPE_STEADY_HIGH, 5);
  #endif

}

void loop()
{ 
  #if INTTERUPT_MODE == 0
  
  // ENCODER NON INTERRUPT MODE
  uint8_t enc_1_status = enc_1->measure();
  if(enc_1_status == ENCODER_FOWARD) {
    Serial.println("E.N.I: -->");
    //Keyboard.write(0xD7);
  } else if (enc_1_status == ENCODER_BACKWARD) {
    Serial.println("E.N.I: <--");
    //Keyboard.write(0xD8);
  }  
  
  // BUTTON NON INTTERUPT MODE
  int8_t bt_1_status = bt_1->measure();
  if(bt_1_status == BUTTON_PRESSED) {
    Serial.println("B.N.I: Pressed");
    //Keyboard.press('C');
  } else if (bt_1_status == BUTTON_RELEASED) {
    Serial.println("B.N.I: Released");
    //Keyboard.release('C');
  }

  #endif
  delay(1);
}



#if INTTERUPT_MODE
// Callback function for encoder 1 just call the handler of the created object, nothing else.
// It will decide inside which function pointer to call.
void encoder1_callback(void)
{
  enc_1->callbackHandler();
}

void encoder1_foward(void)
{
  Serial.println("E.I: -->");
  //Keyboard.write(0xD7);
}

void encoder1_backward(void)
{
  Serial.println("E.I: <--");
  //Keyboard.write(0xD8);
}

// Callback function for buton 1 just call the handler of the created object, nothing else.
// It will decide inside which function pointer to call.
void button1_callback(void)
{
  bt_1->callbackHandler();
}
void button1_pressed(void)
{
  Serial.println("B.I: Pressed");
}
void button1_released(void)
{
  Serial.println("B.I: Released");
}
#endif