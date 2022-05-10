#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <string.h>

#define KONDENSATOR_LADESPANNUNG 1

void initUSART(uint32_t baudRate);
void transmitByte(uint8_t data);
void transmitString(const String& data);
void initADC();
uint16_t readADC(uint8_t channel);
void initTimer0();
ISR(TIMER0_COMPA_vect);

int main(void) {
    const uint32_t baudRate = 9600;
    initUSART(baudRate);
    initTimer0();
    initADC();

    while(true) {}
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

void initUSART(uint32_t baudRate) {
    UBRR0 = (F_CPU - (8 * baudRate))/(16 * baudRate); // set baud rate
    UCSR0B = (1 << RXEN0) | (1 << TXEN0); // enable Rx/Tx
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // set 8N1 protocoll
}

void transmitByte(uint8_t data) {
    // Wait for empty transmit buffer
    do{} while (!(UCSR0A & (1 << UDRE0)));
    // send byte
    UDR0 = data;
}

void transmitString(const String& data) {
    uint16_t i = 0;
    while (data[i] != '\0') {
        transmitByte(data[i]);
        i++;
    }
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

void initTimer0() {
    TCCR0A |= (1 << WGM01); // set CTC bit
    TCCR0B |= (1 << CS00)|(1 << CS01); // prescale 1:64
    OCR0A = 125; // total timer ticks, real time 0.5ms
    TIMSK0 |= (1 << OCIE0A);  // interrupt every 0.5ms
    // enable interrupts
    sei(); 
}

ISR(TIMER0_COMPA_vect) {
    static uint8_t extraTime = 0;
    extraTime++;
    if(extraTime == 100) { // every 50ms
        uint16_t adc_input = readADC(KONDENSATOR_LADESPANNUNG);
        transmitString(static_cast<String>(adc_input) + "\n");
        extraTime = 0;
    }
}
