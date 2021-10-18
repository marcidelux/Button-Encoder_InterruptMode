#include <Arduino.h>

#define ENCODER_NO_CHANGE   0
#define ENCODER_FOWARD      1
#define ENCODER_BACKWARD    2

#define POS_STEADY          0b00000011
#define POS_RIGHT           0b00000001
#define POS_LEFT            0b00000010

class Encoder
{
private:
    /* data */
    uint8_t _prev_state;
    uint8_t _flicer;
    uint8_t _pin_A;
    uint8_t _pin_B;
    uint8_t _twoCntr;
    void (*_funcRight)(void);
    void (*_funcLeft)(void);
    uint8_t state();
public:
    Encoder(uint8_t pin_A, uint8_t pin_B, uint8_t flicer);
    Encoder(uint8_t pin_A, uint8_t pin_B, uint8_t flicer, void (*riseCallBack)(void), void(*funcRight)(void), void(*funcLeft)());
    ~Encoder();
    uint8_t measure();
    void callbackHandler();
};