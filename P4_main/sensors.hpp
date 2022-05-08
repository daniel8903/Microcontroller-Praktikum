#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include "adc.hpp"

#define LEFT_SENSOR PC1  // A1
#define RIGHT_SENSOR PC0 // A0
#define POTENTIOMETER 7

class Sensors {
private:
    MyADC adc;
    float conversionConstant = 0.002; // for conversion from error value to offset
    uint16_t leftValue = 0;
    uint16_t rightValue = 0;

    float calculateOffset(void);
public:
    Sensors(void) {}
    ~Sensors(void) {}

    float offset(void);
    uint16_t potentiometerReading(void);
    uint16_t leftReading(void);
    uint16_t rightReading(void);
    void setConversionConstant(float newVal);
};

#endif
