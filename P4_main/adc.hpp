#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>

class MyADC { // class name ADC is reserved in AVR
private:
    void initADC();
public:
    MyADC(void);
    ~MyADC(void) {}
    uint16_t read(uint8_t channel);
};

#endif
