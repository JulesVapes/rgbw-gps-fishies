#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN 6
#define NUM_LEDS 60
#define BRIGHTNESS 255
#define SPEED 20

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println("INFO: Starting up.");
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(0, 0, 0, 255), SPEED); // White
  colorWipe(strip.Color(255, 0, 0), SPEED); // Red
  colorWipe(strip.Color(255, 128, 0), SPEED); // Orange
  colorWipe(strip.Color(255, 255, 0), SPEED); // yellow
  colorWipe(strip.Color(0, 255, 0), SPEED); // Green
  colorWipe(strip.Color(0, 255, 255), SPEED); // Blue1 
  colorWipe(strip.Color(51, 51, 255), SPEED); // Blue2
  colorWipe(strip.Color(0, 0, 255), SPEED); // Blue3 
 
 
  int i = gps.time.hour();
  Serial.print("GPS thinks the hour is:");
  Serial.println(i);
}
// times
// 06-07, red.
// 07-08, Orange.
// 08-09, Yellow.

// 09-22, white.
// 22-00, blue.
// 00-06, off;

int hourLastSeen = -1;
void loop()
{
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      if (gps.date.isValid())
      {
        int i = gps.time.hour();
        if(hourLastSeen != i)
        {
          Serial.print("GPS thinks the hour is:");
          Serial.println(i);
          // 00-06, off;
          //mark has herpes, twice.. no threetimes. im sure. 
          //whereas morris favours bukake. and eating through a hammock
          
          if( (i>=0)&&(i<6) )
          {
            colorWipe(strip.Color(0, 0, 0, 0), 0); // off I think.
            Serial.println(F("midnight through to 6am, lights out"));
          }
          //06-07, red.
          if( (i>=6)&&(i<7) )
          {
            colorWipe(strip.Color(255, 0, 0), SPEED); // Red
            Serial.println(F("6am through to 7am, red colour set"));
            }
          //07-08, orange.
          if( (i>=7)&&(i<8) )
          {
            colorWipe(strip.Color(255, 128, 0), SPEED); // orange
            Serial.println(F("7am through to 8am, 0range colour set")); 
          }
          // 08-22, white.
          if( (i>=8)&&(i<22) )
          {
            colorWipe(strip.Color(0, 0, 0, 255), SPEED); // White
            Serial.println(F("8am through to 10pm, white colour set"));
          }
          //22-23, blue2.
          if( (i>=22)&&(i<23) )
          {
            colorWipe(strip.Color(51, 51, 255), SPEED); // blue2
            Serial.println(F("10pm through to 11pm, blue2 colour set"));
          }
          // 23-00, blue3.
          if(i>=23)
          {
            colorWipe(strip.Color(0, 0, 255), SPEED); // Blue3
            Serial.println(F("11pm through to midnight, blue3 colour set"));
          }
          hourLastSeen = i;
        }
      }
      else
      {
        Serial.println(F("ERROR: no time and date set/detected on the gps module rtc, probably need to go outside with the antenna, in the snow!"));
      }
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("ERROR: No GPS detected: check wiring."));
    while(true);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
