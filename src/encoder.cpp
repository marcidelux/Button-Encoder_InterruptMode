#include "encoder.hpp"

Encoder::Encoder(uint8_t pin_A, uint8_t pin_B, uint8_t flicer)
{
    _pin_A      = pin_A;
    _pin_B      = pin_B;
    _flicer     = flicer;
    pinMode(_pin_A, INPUT);
    pinMode(_pin_B, INPUT);
    _prev_state = this->state();
}

Encoder::Encoder(uint8_t pin_A, uint8_t pin_B, uint8_t flicer, void (*riseCallBack)(void), void(*funcRight)(void), void(*funcLeft)())
{
    _pin_A      = pin_A;
    _pin_B      = pin_B;
    _funcRight  = funcRight;
    _funcLeft   = funcLeft;
    _flicer     = flicer;
    _twoCntr    = 0;
    pinMode(_pin_A, INPUT);
    pinMode(_pin_B, INPUT);
    attachInterrupt(digitalPinToInterrupt(_pin_A), riseCallBack, RISING);
}

Encoder::~Encoder()
{
}

uint8_t Encoder::state()
{
    uint8_t state = 0;
    state |= digitalRead(_pin_A) << 1;
    state |= digitalRead(_pin_B);
    return state;
}

uint8_t Encoder::measure()
{
    uint8_t state = this->state();
    uint8_t retVal = ENCODER_NO_CHANGE;

    if(state != _prev_state) {
        if(state == POS_STEADY) {
            if(_prev_state == POS_LEFT) {
                retVal =  ENCODER_FOWARD;
            } else if(_prev_state == POS_RIGHT) {
                retVal = ENCODER_BACKWARD;
            }
        }
        _prev_state = state;
    }

    return retVal;
}

void Encoder::callbackHandler(void)
{
    uint8_t cntr = 0;
    for(uint8_t i = 0; i < _flicer; i ++) {
        cntr += digitalRead(_pin_A);
    }
    if(cntr < _flicer)
        return;

    if(digitalRead(_pin_B)) {
        if(_twoCntr) {
            _twoCntr = 0;
            this->_funcLeft();
        } else {
            _twoCntr++;
        }
    } else {
        if(_twoCntr) {
            _twoCntr = 0;
            this->_funcRight();
        } else {
            _twoCntr++;
        }
    }
}