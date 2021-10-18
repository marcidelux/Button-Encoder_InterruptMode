#include <Arduino.h>

#define BUTTON_STEADY   0
#define BUTTON_PRESSED  1
#define BUTTON_RELEASED 2
#define BUTTON_FLICKER  3

#define BUTTON_TYPE_STEADY_LOW   0
#define BUTTON_TYPE_STEADY_HIGH  1

class Button
{
private:
    uint8_t _prev_state;
    uint8_t _steady_type;
    uint8_t _pin;
    uint8_t _flicer;
    void (*_funcPresss)(void);
    void (*_funcReleased)(void);
public:
    Button(uint8_t pin, uint8_t steady_type, uint8_t flicer);
    Button(uint8_t pin, uint8_t steady_type, uint8_t flicer,
           void (*changeCallBack)(void),
           void (*funcPressed)(void),
           void (*funcReleased)(void));
    ~Button();
    uint8_t measure();
    void callbackHandler();
};