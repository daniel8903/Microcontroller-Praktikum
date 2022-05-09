#include "button.hpp"

Button::Button(volatile uint8_t* pinID_ptr, uint8_t pinID) {
    this->pinID_ptr = pinID_ptr;
    this->pinID     = pinID;
}

bool Button::pressed() {
    bool pressed = !(*(this->pinID_ptr) & (1 << this->pinID));
    if (pressed) {
        _delay_ms(30);
        while (!( *(this->pinID_ptr) & (1 << this->pinID))) {}  
        this->pressCounter++;      
        return true;
    } else return false;
}

void Button::blinkLED(uint8_t times) {
    for (uint8_t i = 0; i < times; ++i) {
        PORTB |= (1 << PB5); 
        _delay_ms(400);
        PORTB &= ~(1 << PB5);
        _delay_ms(400);
    }
}

bool Button::isOdd() {
    if (this->pressCounter % 2) return true;
    else return false;
}
