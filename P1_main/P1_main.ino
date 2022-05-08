#define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

//************************ Arrays contain different steps ********************************
const uint8_t ARRAY_SIZE_D = 9;
const uint8_t ARRAY_SIZE_B = 7;

const uint8_t stepsPortD_const[ARRAY_SIZE_D] = { 0b00000100, 0b00001100, 0b00011100, 
                                                 0b00111100, 0b01110100, 0b11100100, 
                                                 0b11000100, 0b10000100, 0b00000100 };
const uint8_t stepsPortB_const[ARRAY_SIZE_B] = { 0b00011000, 0b00011001, 0b00011011, 
                                                 0b00011111, 0b00011110, 0b00011100,
                                                 0b00011000 };
uint8_t stepsPortB[ARRAY_SIZE_B];
uint8_t stepsPortD[ARRAY_SIZE_D];
//*****************************************************************************************     
                          
const uint8_t SWITCH1 = 1 << PB3; // Button to start Sequence
const uint8_t SWITCH2 = 1 << PB4; // Button to start Sequence with smaller delay
const uint8_t SWITCH3 = 1 << PD2; // Button to interrupt and disable first and last LED

uint8_t interruptCounter = 0;

ISR(INT0_vect);
void knightRiderSequence(uint8_t repetitions, uint16_t delayInMS);
void my_delay_ms(uint16_t ms);

int main() {
  DDRD = 0b11111000; // set output pins
  // DDRD |= (1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);
  DDRB = 0b00000111; // set output pins
  // DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
  
  PORTB |= (SWITCH1|SWITCH2); // activate pull-up 
  PORTD |= SWITCH3; // activate pull-up 

  EIMSK |= _BV(INT0);  //enable int0
  EICRA |= _BV(ISC01); //interrupt on falling edge
  sei(); // enable interrupts

  for(uint8_t i = 0; i < ARRAY_SIZE_D; ++i) { // make  copy of const arrays
    stepsPortD[i] = stepsPortD_const[i];
    if(i < ARRAY_SIZE_B)
      stepsPortB[i] = stepsPortB_const[i];
  }

  while(true) {
    if( !(PINB & SWITCH1) ) { // if button 1 pressed
      knightRiderSequence(3, 500);  // start sequence with 500ms delay (repeat 3 times)
    } else if(!(PINB & SWITCH2)) { // if button 2 pressed
      knightRiderSequence(3, 200); // start with 200ms delay 
    } 
  }
  return 0;
} 

ISR(INT0_vect) 
{ 
 _delay_ms(5); // confirm button press
 if( (PIND & SWITCH3) == 0 ) {
  ++interruptCounter; // increment counter 
  if(interruptCounter%2 == 1) { // if counter odd
    for(uint8_t i = 0; i < ARRAY_SIZE_D; ++i) { 
        // disable first and last LED
      stepsPortD[i] = stepsPortD_const[i] & 0b11110111;
      if(i < ARRAY_SIZE_B) 
        stepsPortB[i] = stepsPortB_const[i] & 0b11111011;
      }
    } else {
      for(uint8_t i = 0; i < ARRAY_SIZE_D; ++i) { // if counter even
        // enable first and last LED
      stepsPortD[i] = stepsPortD_const[i];
      if(i < ARRAY_SIZE_B) 
        stepsPortB[i] = stepsPortB_const[i];
      }
    }
  } 
}

void knightRiderSequence(uint8_t repetitions, uint16_t delayInMS) {
  for(;repetitions > 0; --repetitions)
  {
    uint8_t countD = 0;
    uint8_t countB = 0;
    uint8_t i = 0;
    for(; i < 12; i++) { // 11 repetitions (11 different steps)
        // Sequence forward, traverse arrays from start to end
      if(i >= 5 && i < 9) {           // on 6th,7th,8th repetition LEDs from both ports(arrays) needed
        PORTB = stepsPortB[countB++];
        PORTD = stepsPortD[countD++];
      } else if(i >= 9) {
        PORTB = stepsPortB[countB++]; // for last 3 repetitions only LEDs from PORTB needed
      } else {
        PORTD = stepsPortD[countD++]; // for the first 5 repetitions only LEDs from PORTD are needed
      }
      my_delay_ms(delayInMS); // delay between every step
    }
    for(i=11;i >= 0; i--) { // 11 steps 
      // Sequence backwards, traverse arrays from end to start
      if(i >= 9) {
        PORTB = stepsPortB[--countB];
      } else if(i >= 5 && i < 9) {
        PORTB = stepsPortB[--countB];
        PORTD = stepsPortD[--countD];
      } else {
        PORTD = stepsPortD[--countD];
      }     
      my_delay_ms(delayInMS); // delay between every step
    }
  }
}

void my_delay_ms(uint16_t ms)
{
  while (0 < ms)
  {  
    _delay_ms(1);
    --ms;
  }
}
