#include "neopixel.h"

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D4
#define PIXEL_COUNT 24
#define MAX_BRIGHTNESS 10
#define MIN_BRIGHTNESS 5
#define BREATH_STEP_MILLISECONDS 10000
#define PIXEL_TYPE SK6812RGBW

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// Prototypes for local build, ok to leave in for Build IDE
void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);
bool lightsOn = false;
uint32_t timeOff = 0;

uint16_t currentWhiteBrightness = MAX_BRIGHTNESS;
int breathDirection = -1;
uint32_t timeToChangeBrightness = 0;

int turnOn(String args) {
  lightsOn = true;
  timeOff = millis() + 10000;
  return 0;
}

void setup()
{
  bool success = Particle.function("turnOn", turnOn);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void loop()
{
  if (millis() >= timeOff) {  // Should we be off, ie breathing white
    for (int i = 0; i < PIXEL_COUNT; i++) {
      if (millis() >= timeToChangeBrightness) {
        if (currentWhiteBrightness >= MAX_BRIGHTNESS) {
          breathDirection = -1;
        }
        if (currentWhiteBrightness <= MIN_BRIGHTNESS) {
          breathDirection = 1;
        }
        currentWhiteBrightness = currentWhiteBrightness + breathDirection;
        timeToChangeBrightness = millis() + BREATH_STEP_MILLISECONDS;
      }
      strip.setBrightness(currentWhiteBrightness);
      strip.setPixelColor(i, 0, 0, 0, 255);
    }
    strip.begin();
    strip.show();
    lightsOn = false;
  }

   if (lightsOn) {
     strip.setBrightness(255);
     rainbow(10);
   }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

