#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

class Button {
private:
    uint8_t pinID = 0;
    volatile uint8_t* pinID_ptr = nullptr;
    uint8_t pressCounter = 0;
public:
    Button(void) {}
    Button(volatile uint8_t* pinID_ptr, uint8_t pinID);

    void blinkLED(uint8_t times);
    bool pressed(void);
    bool isOdd(void);
};

#endif
