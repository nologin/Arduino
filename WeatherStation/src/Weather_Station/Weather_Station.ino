#include "setup.h"
#include "tools.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#include <Wire.h>
#include <OneWire.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

// The display
Display(LCD);

// DS18b20
OneWire ds(TEMP_PIN);  // on pin 2 (a 4.7K resistor is necessary)



void setup() {
    Serial.begin(115200);
    Serial.print(F("Initializing pins...\n"));

    // Initialize pinMode for UV Sensor
    // pinMode(UVOUT, INPUT); // Input is the default state of all pins
    // pinMode(REF_3V3, INPUT); // Input is the default state of all pins

    Serial.print(F("Initializing display...\n"));
    LCD.begin();
    LCD.enableUTF8Print();
    // LCD.setPowerSave(0);
    // LCD.setFont(u8x8_font_chroma48medium8_r);
    LCD.firstPage();
    do {
        LCD.drawRFrame(0,0,128,64,7);
        LCD.setFont(u8g2_font_unifont_t_cyrillic);
        LCD.setCursor(10, 32);
        LCD.print("Инициализиране");

    } while ( LCD.nextPage() );

    Serial.print(F("Initialization complete.\n"));
}



void loop() {
    float temperature = get_temperature(ds);
    float uv_index = getUvIndex(REF_3V3, UVOUT, temperature);
    /*
                LCD.setCursor(0, 0);
                LCD.print(F("Hello World!"));

                LCD.setCursor(0, 1);
                LCD.print(F("UV: "));
                float temperature = get_temperature(ds);
                LCD.print(getUvIndex(REF_3V3, UVOUT, temperature));
                LCD.print(F("   "));

                LCD.setCursor(0, 2);
                LCD.print(F("T: "));
                LCD.print(temperature);
                LCD.print(F("   "));
    */
    LCD.firstPage();
    do {
        LCD.setFont(u8g2_font_unifont_t_cyrillic);
        LCD.setCursor(15, 20);
        LCD.print(F("Темп:  "));
        LCD.print(temperature);
        LCD.setCursor(15, 35);
        LCD.print(F("УВ Инд: "));
        LCD.print(uv_index);
        if (temperature > 29.0) {
            LCD.setFont(u8g2_font_unifont_t_cyrillic);
            LCD.setCursor(15, 50);
            LCD.print(F("Жега!"));
        } else if (temperature < 0.00) {
            LCD.setFont(u8g2_font_unifont_t_cyrillic);
            LCD.setCursor(15, 50);
            LCD.print(F("Студ!"));
        } else if (temperature < 5.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(15, 50, 0x2603);
            LCD.drawGlyph(35, 50, 0x2603);
            LCD.drawGlyph(55, 50, 0x2603);
        } else if (temperature < 10.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(15, 50, 0x2603);
            LCD.drawGlyph(35, 50, 0x2603);
        } else if (temperature < 15.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(15, 50, 0x2603);
        } else if (temperature < 20.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(15, 50, 0x2600);
        }  else if (temperature < 25.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(15, 50, 0x2600);
            LCD.drawGlyph(25, 50, 0x2600);
        } else if (temperature < 29.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(15, 50, 0x2600);
            LCD.drawGlyph(25, 50, 0x2600);
            LCD.drawGlyph(35, 50, 0x2600);
        }

        if (uv_index > 5.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(80, 50, 0x2600);
            LCD.drawGlyph(90, 50, 0x2600);
            LCD.drawGlyph(100, 50, 0x2600);
        } else if (uv_index < 1.00) {
            LCD.setFont(u8g2_font_unifont_t_cyrillic);
            LCD.setCursor(80, 50);
            LCD.print(F("UV OK"));
        } else if (uv_index < 2.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(80, 50, 0x2600);
        } else if (uv_index < 5.00) {
            LCD.setFont(u8g2_font_unifont_t_symbols);
            LCD.drawGlyph(80, 50, 0x2600);
            LCD.drawGlyph(90, 50, 0x2600);
        }
        LCD.drawRFrame(0,0,128,64,7);
    } while ( LCD.nextPage() );
    //delay(100);
}

