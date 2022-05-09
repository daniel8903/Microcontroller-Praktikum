#include "Linefollower.hpp"

Linefollower::Linefollower() {
    setup();
}

void Linefollower::run() {
    while (true) {
        if (this->buttonPD2.pressed()) changeMode();
        this->servos.followLine(this->sensors.offset(), 
                                this->sensors.leftReading(), 
                                this->sensors.rightReading());
    }
}

void Linefollower::changeMode() {
    this->servos.stop(); // pause servos
    if(this->buttonPD2.isOdd()) {
        // fast mode
        this->buttonPD2.blinkLED(3);
        this->servos.setMode(8000, 10);
        this->sensors.setConversionConstant(0.00258); 
    } else {
        // default mode    
        this->buttonPD2.blinkLED(2);
        this->servos.setMode(3500, 30);
        this->sensors.setConversionConstant(0.002)
    }
}

void Linefollower::setup() {
    Button tmp(&PIND, PD2);
    this->buttonPD2 = tmp;
    PORTD |=  (  1 << PD2); // set pull-up resistor on PD2 for switch
    DDRB  |=  ( (1 << PB0) | (1 << PB1) );  // set pins for right wheel to output
    DDRB  |=  ( (1 << PB2) | (1 << PB3) );  // set pins for left wheel to output
    DDRC  &= ~( (1 << PC0) | (1 << PC1) );  // make sure PC0 and PC1 are inputs
}