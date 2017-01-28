#ifndef TOOLS_H
#define TOOLS_H

#include <stdint.h>
#include <OneWire.h>


//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
inline float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getUvIndex(uint8_t vref_pin, uint8_t data_pin, int8_t temperature);
float get_temperature(OneWire& ds);
#endif // TOOLS_H

