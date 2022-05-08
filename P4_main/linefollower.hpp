#ifndef LINEFOLLOWER_H_
#define LINEFOLLOWER_H_

#include <avr/io.h>
#include "servos.hpp"
#include "sensors.hpp"
#include "button.hpp"

class Linefollower {
private: 
    Servos servos;
    Sensors sensors;
    Button buttonPD2;
    
    void setup(void);
    void changeMode(void);
public:
    Linefollower(void);
    ~Linefollower(void) {}

    void run(void); 
};

#endif
