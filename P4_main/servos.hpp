#ifndef SERVOS_H_
#define SERVOS_H_

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define TRESHOLD 300

class Servos {
private:
    uint16_t referenceSpeed = 3300; // reference value between 0 and 8000
    uint8_t backwards_delay = 30; // 30ms by default
    
    void initTimer1(void); 
    void setSpeedM1(uint16_t speed);
    void setSpeedM2(uint16_t speed);
    void driveBackwards(uint8_t backwards_delay);
    void driveForward(void);
    void my_delay_ms(uint8_t ms);
    bool bothSeeWhite(uint16_t leftSensorReading, uint16_t rightSensorReading);
public:
    Servos(void);
    void stop(void);
    void followLine(float offset, uint16_t leftSensorReading, uint16_t rightSensorReading);
    void setMode(uint16_t speed=3300, uint8_t delay=30);
};

#endif
