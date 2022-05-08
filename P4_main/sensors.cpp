#include "sensors.hpp"

float Sensors::offset() {
    return calculateOffset();
}

float Sensors::calculateOffset() {
    this->rightValue = this->adc.read(RIGHT_SENSOR);
    this->leftValue  = this->adc.read(LEFT_SENSOR);
    int16_t error = this->rightValue - this->leftValue;
    return conversionConstant * error; // offset
}

uint16_t Sensors::potentiometerReading() {
    return this->adc.read(POTENTIOMETER);
}

uint16_t Sensors::leftReading() {
    return this->leftValue;
}

uint16_t Sensors::rightReading() {
    return this->rightValue;
}

void Sensors::setConversionConstant(float newVal) {
    this->conversionConstant = newVal;
}
