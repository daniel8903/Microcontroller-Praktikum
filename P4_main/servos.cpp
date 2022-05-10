#include "servos.hpp"

Servos::Servos() {
   initTimer1();
}

void Servos::followLine(float offset, uint16_t leftSensorReading, uint16_t rightSensorReading) 
{
    if(offset > 1) { 
        setSpeedM1( 0 ); 
        setSpeedM2( 2 * this->referenceSpeed );  
    } else if(offset < -1) {
        setSpeedM1( 2 * this->referenceSpeed ); 
        setSpeedM2( 0 );
    } else {
        setSpeedM1( (1 - offset) * this->referenceSpeed ); 
        setSpeedM2( (1 + offset) * this->referenceSpeed );
    }

    if(bothSeeWhite(leftSensorReading, rightSensorReading)) {
        driveBackwards();
    } else {
        driveForward(); 
    }
}

bool Servos::bothSeeWhite(uint16_t leftSensorReading, uint16_t rightSensorReading) {
    return leftSensorReading < TRESHOLD && rightSensorReading < TRESHOLD;
}

void Servos::setMode(uint16_t speed, uint8_t delay) {
    this->referenceSpeed  = speed;
    this->backwards_delay = delay;
}

void Servos::setSpeedM1(uint16_t speed) {
    // change duty cycle
    OCR1A = speed;
}
void Servos::setSpeedM2(uint16_t speed) {
    // change duty cycle
    OCR1B = speed;
}
void Servos::driveBackwards() {
    PORTB &= ~(1 << PB0); // backward right
    PORTB &= ~(1 << PB3); // backward left
    my_delay_ms(this->backwards_delay);
}
void Servos::driveForward() {
    PORTB |= (1 << PB0); // forward right
    PORTB |= (1 << PB3); // forward left 
}

void Servos::stop() {
    setSpeedM1(0);
    setSpeedM2(0);
}

void Servos::my_delay_ms(uint8_t ms) {
    while(0 < ms) {
        _delay_ms(1);
        --ms;
    }
}

void Servos::initTimer1() { 
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); // Fast PWM 14, Top = ICR1
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); // prescaler = 8  
  ICR1 = (1 << 14);   // ICR1 = 2^14 = 16384; Pule-width: ca. 122Hz
} 
