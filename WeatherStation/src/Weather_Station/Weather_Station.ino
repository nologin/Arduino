#include "setup.h"
#include "tools.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#include <Wire.h>
#include <OneWire.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

//Time
#include <TimeLib.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

// The display
Display(LCD);

// DS18b20
OneWire ds(TEMP_PIN);  // on pin 2 (a 4.7K resistor is necessary)



void setup() {
    Serial.begin(115200);
    while (!Serial) ; // Needed for Leonardo only
     // pinMode(13, OUTPUT);
      setSyncProvider( requestSync);  //set function to call when sync required
      Serial.println("Waiting for sync message");

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
   if (Serial.available()) {
    processSyncMessage();
  } 
    if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
  }
  
    float temperature = get_temperature(ds);
    float uv_index = getUvIndex(REF_3V3, UVOUT, temperature);
   
/*        LCD.setCursor(0, 0);
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
        LCD.setCursor(10, 15);
        LCD.print(F("Темп:  "));
        LCD.print(temperature);
        LCD.setCursor(10, 30);
        LCD.print(F("УВ инд: "));
        LCD.print(uv_index);
        LCD.setCursor(10, 45);
        LCD.print(F("H: "));
        LCD.print(hour());
        LCD.print(F(":"));
        LCD.print(minute());
        LCD.print(F(":"));
        LCD.print(second());
        LCD.setCursor(10, 59);
        LCD.print(F("D: "));
        LCD.print(day());
        LCD.print(F("-"));
        LCD.print(month());
        LCD.print(F("-"));
        LCD.print(year());
        
/*        if (temperature > 29.0) {
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
        } */
        LCD.drawRFrame(0,0,128,64,7);
    } while ( LCD.nextPage() );
   //delay(100);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

    time_t requestSync() {
      Serial.write(TIME_REQUEST);  
      return 0; // the time will be sent later in response to serial mesg
    }
