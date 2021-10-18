#include "button.hpp"

Button::Button(uint8_t pin, uint8_t steady_type, uint8_t flicer)
{
    _pin = pin;
    _steady_type = steady_type;
    _flicer = flicer;
    pinMode(_pin, INPUT);
    _prev_state = digitalRead(_pin);
}

Button::Button(uint8_t pin, uint8_t steady_type, uint8_t flicer,
               void (*changeCallBack)(void),
               void (*funcPressed)(void),
               void (*funcReleased)(void))
{
    _pin = pin;
    _steady_type = steady_type;
    _flicer = flicer;
    _funcPresss = funcPressed;
    _funcReleased = funcReleased;
    pinMode(_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(_pin), changeCallBack, CHANGE);
}

void Button::callbackHandler()
{
    uint8_t pinVal = 0;
    for(uint8_t i = 0; i < _flicer; i ++) {
        pinVal += digitalRead(_pin);
    }

    // Pin is high
    if(pinVal == _flicer) {
        if(_steady_type == BUTTON_TYPE_STEADY_LOW) {
            this->_funcPresss();
        } else if(_steady_type == BUTTON_TYPE_STEADY_HIGH) {
            this->_funcReleased();
        }
    // Pin is low
    } else if (!pinVal) {
        if(_steady_type == BUTTON_TYPE_STEADY_LOW) {
            this->_funcReleased();
        } else if(_steady_type == BUTTON_TYPE_STEADY_HIGH) {
            this->_funcPresss();
        }
    // Flickering effect.
    } else {
        return; 
    }
}

// Use this function in Non interrupt mode.
uint8_t Button::measure()
{
    uint8_t retVal = BUTTON_STEADY;
    uint8_t state = 0;
    
    // Check if flicekrs or high or low.
    for(uint8_t i = 0; i < _flicer; i++) {
        state += digitalRead(_pin);
    }
    if (state == _flicer) {
        state = HIGH;
    } else if (!state) {
        state = LOW;
    } else {
        return BUTTON_FLICKER;
    }


    if (_prev_state != state) {
        // Normally low
        if (_steady_type == BUTTON_TYPE_STEADY_LOW) {
            if(_prev_state == LOW) {
                retVal = BUTTON_PRESSED;
            } else {
                retVal = BUTTON_RELEASED;
            }
        // Normally high
        } else if(_steady_type == BUTTON_TYPE_STEADY_HIGH){
            if(_prev_state == HIGH) {
                retVal = BUTTON_PRESSED;
            } else {
                retVal = BUTTON_RELEASED;
            }
        }

        _prev_state = state;
    }

    return retVal;
}

Button::~Button()
{
}
