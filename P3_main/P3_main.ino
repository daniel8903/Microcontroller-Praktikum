#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <string.h>

#define POTENTIOMETER 1

void initADC();
uint16_t readADC(uint8_t channel);
void initTimer0();
ISR(TIMER0_OVF_vect);
void initTimer1();

bool blinkLED = false;
uint8_t t0_overflowCounter = 0;

int main() {
    DDRD |= (1 << PD3); // set Pin PD3 with LED as OUTPUT
    DDRB |= (1 << PB1); // set Pin OC1A as OUTPUT
  
    initADC();
    initTimer1();
    initTimer0();

    uint16_t adc_input = 0;

    while(true) {
        adc_input = readADC(POTENTIOMETER);
        OCR1A = 1000 + ((adc_input * 3800.0)/1023.0); // set pulse width between 0,5 ms (0°) and 
                                                     // 2,4 ms (180°) according to adc_input
                                                    // https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1
        if(adc_input >= 512) {
            if(blinkLED == false) {
                TCCR0B |= (1 << CS02) | (1 << CS00); // start timer 0 with prescale 1:1024
                t0_overflowCounter = 0; // reset overflowCounter
                PORTD |= (1 << PD3); // turn LED on, to instantly start blinking 
                blinkLED = true;
            }
        } else {
            TCCR0B = 0x00; // stops timer 0
            PORTD &= ~(1 << PD3); // turn LED off
            blinkLED = false;
        }
    }
    return 0;
}

void initADC() {
    ADMUX |= (1 << REFS0); // set 5V as REF voltage
    // prescale division factor 128, ADC clock == 128 kHz, sampling rate 9.6 kHz
    ADCSRA |= ( (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) );
    ADCSRA |= (1 << ADEN); // activate single conversion
    ADCSRA |= (1 << ADSC); // first ADC run to warm up
    // clean ADC
    do {} while ( ADCSRA & (1 << ADSC) );
    uint16_t garbage = ADC;
}

void initTimer0() {
    // TCCR0B |= (1 << CS02) | (1 << CS00); // prescale 1:1024, happens in main
    TIMSK0 |= (1 << TOIE0);  // interrupt on overflow, ≈ every 20ms
    sei(); // enable interrupts
}

void initTimer1(){ 
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
    TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11); // prescaler = 8
    ICR1 = 40000; // Pulse Cycle: 20ms (50Hz)
}

uint16_t readADC(uint8_t channel) {
    const uint8_t kMuxMask = ( (1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) );
    ADMUX &= ~kMuxMask; // clear MUX
    ADMUX |= channel; // set input channel
    ADCSRA |= (1 << ADSC); // start conversion
    // read ADC
    do {} while ( ADCSRA & (1 << ADSC) );
    return ADC;
}

ISR(TIMER0_OVF_vect) {
    t0_overflowCounter++;
    if(t0_overflowCounter == 25) { // ≈ every 500ms
        PORTD ^= (1 << PD3); // Toggle the LED
        t0_overflowCounter = 0;
    }
}
