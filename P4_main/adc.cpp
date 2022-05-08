#include "adc.hpp"

MyADC::MyADC() {  
    initADC();
}

uint16_t MyADC::read(uint8_t channel) {
    const uint8_t kMuxMask = ( (1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) );
    ADMUX &= ~kMuxMask; // clear MUX
    ADMUX |= channel; // set input channel
    ADCSRA |= (1 << ADSC); // start conversion
    // read ADC
    do {} while ( ADCSRA & (1 << ADSC) );
    return ADC;
}

void MyADC::initADC()  {
    ADMUX |= (1 << REFS0);  // set 5V as REF voltage
    // prescale division factor 128, ADC clock == 128 kHz, sampling rate 9.6 kHz
    ADCSRA |= ( (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) );
    ADCSRA |= (1 << ADEN); // activate single conversion
    ADCSRA |= (1 << ADSC); // first ADC run to warm up
    // clean ADC
    do {} while ( ADCSRA & (1 << ADSC) );
    uint16_t garbage = ADC;
}
