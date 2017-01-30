#include "tools.h"

#include <Arduino.h>


const float vcc = 5.00; // Should be 5.0
// const float vcc = 5.0; // Should be 5.0

float getUvIndex(uint8_t vref_pin, uint8_t data_pin, int8_t temperature) {
    uint16_t vref = analogRead(vref_pin);
    Serial.print(F("VREF = "));
    Serial.print(vref);

    uint16_t uv = analogRead(data_pin);
    Serial.print(F(", UV = "));
    Serial.print(uv);


    float vref_v = vref * (vcc / 1023); // The real voltage on 3.3
    Serial.print(F(", vref_v = "));
    Serial.print(vref_v);

    float uv_v = uv * (vcc / 1023); // The real voltage on the UV sensor
    Serial.print(F(", uv_v = "));
    Serial.print(uv_v);

    // Default is 25 degree celsius curve
    float idx_min = 1.0;
    float idx_max = 2.8;

    if (temperature > 50) { // The 75 degree curve
        idx_min = 0.93;
        idx_max = 2.76;
    } else if (temperature < -15) {
        idx_min = 1.06;
        idx_max = 2.95;
    } else if (temperature < 5) {
        idx_min = 1.03;
        idx_max = 2.9;
    }

    float result = mapfloat(uv_v, idx_min, idx_max, 0.0, 15.0);

    Serial.print(F(", result = "));
    Serial.print(result);
    Serial.println();
    if (result < 0.0) {
        // result = 0.0;
        Serial.print(F("UV Error!"));
        Serial.println();
    }
    return result;
}

float get_temperature(OneWire& ds) {
    byte addr[8];
    byte data[12];

    ds.reset_search();
    delay(250);
    if (!ds.search(addr)) {
        Serial.println("No DS20b18 sensor found!");
        return 1.0/0.0;
    }
    if (OneWire::crc8(addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!");
        return 1.0/0.0;
    }

    byte type_s = 0;
    // the first ROM byte indicates which chip
    switch (addr[0]) {
    case 0x10:
        // Serial.println("  Chip = DS18S20");  // or old DS1820
        type_s = 1;
        break;
    case 0x28:
        // Serial.println("  Chip = DS18B20");
        break;
    case 0x22:
        // Serial.println("  Chip = DS1822");
        break;
    default:
        Serial.println("Device is not a DS18x20 family device.");
        return 1.0/0.0;
    }

    if (!ds.reset()) {
        Serial.println("DS20B18 dissapeared!");
        return 1.0/0.0;
    }
    ds.select(addr);
    ds.write(0x44);        // start conversion, use ds.write(0x44,1) with parasite power on at the end
    delay(800);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.

    if (!ds.reset()) {
        Serial.println("DS20B18 dissapeared!");
        return 1.0/0.0;
    }
    ds.select(addr);
    ds.write(0xBE);         // Read Scratchpad

    for (byte i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
    }

    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    } else {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
        // default is 12 bit resolution, 750 ms conversion time
    }
    Serial.print(F("Temp:"));
    Serial.println(float(raw) / 16);
    return float(raw) / 16.0;
}

