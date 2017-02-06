#include "setup.h"
#include "tools.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#include <Wire.h>
#include <OneWire.h>
#include <U8x8lib.h>
#include <U8g2lib.h>
#include <RH_ASK.h>

// The display
Display(LCD);

// DS18b20
OneWire ds_out(TEMP_PIN_OUT);  // on pin 2 (a 4.7K resistor is necessary)
OneWire ds_in(TEMP_PIN_IN);


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
    float temperature_out = get_temperature(ds_out);
    float temperature_in = get_temperature(ds_in);
    float uv_index = getUvIndex(REF_3V3, UVOUT, temperature_out);
    LCD.firstPage();
    do {
        LCD.setFont(u8g2_font_unifont_t_cyrillic);
        LCD.setCursor(15, 20);
        LCD.print(F("Темп <-: "));
        LCD.print(temperature_in);
        LCD.setCursor(15, 35);
        LCD.print(F("Темп ->: "));
        LCD.print(temperature_out);
        LCD.setCursor(15, 50);
        LCD.print(F("УВ Инд: "));
        LCD.print(uv_index);

        LCD.drawRFrame(0,0,128,64,7);
    } while ( LCD.nextPage() );
    delay(100);
}

