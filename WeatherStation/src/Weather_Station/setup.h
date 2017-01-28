#ifndef SETUP_H
#define SETUP_H
#include <stdint.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

// The display
const uint8_t DISPLAY_CLK  = 13;
const uint8_t DISPLAY_DATA = 11;
const uint8_t DISPLAY_CS   = 10;
const uint8_t DISPLAY_DC   =  9;
const uint8_t DISPLAY_RST  =  8;
const uint8_t DISPLAY_D0   = 13;
const uint8_t DISPLAY_D1   = 12;

#define WITH_GRAPHICS
#ifdef WITH_GRAPHICS

#define Display(name)\
U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI name(U8G2_R0, DISPLAY_CLK, DISPLAY_DATA, DISPLAY_CS, DISPLAY_DC, DISPLAY_RST)

#else // WITH_GRAPHICS

#define Display(name)\
U8X8_SSD1306_128X64_NONAME_4W_SW_SPI name(DISPLAY_CLK, DISPLAY_DATA, DISPLAY_CS, DISPLAY_DC, DISPLAY_RST)

#endif // WITH_GRAPHICS



// DS18b20
const uint8_t TEMP_PIN     =  2;

// UV
const uint8_t UVOUT = A0;

// 3.3 reference voltage
const uint8_t REF_3V3 = A1;



#endif // SETUP_H

