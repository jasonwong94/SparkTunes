#include <Adafruit_NeoPixel.h>

#ifndef SPARKTUNES_LEDSTRIPS_H
#define SPARKTUNES_LEDSTRIPS_H
  const int LEDS_PER_STRIP = 30;
  const int NUM_STRIPS = 2;
  const int MAX_NUM_LEDS = LEDS_PER_STRIP*2; //change if need be
  const int PULSE_ITERATION = 3;

  void ledStrips_test(Adafruit_NeoPixel *strip);
  void ledStrips_reset(Adafruit_NeoPixel *strip);
  void ledStrips_setColour(Adafruit_NeoPixel *strip, uint32_t colour);
  void ledStrips_setColour(Adafruit_NeoPixel *strip, int leds[], uint32_t colour);
  void ledStrips_setColour(Adafruit_NeoPixel *strip, int leds[], uint32_t colour[]);
  void ledStrips_flashLeds(Adafruit_NeoPixel *strip, int leds[], uint32_t colour[], unsigned long pulse);
  void ledStrips_displayIterations(Adafruit_NeoPixel *strip, int iteration);
  void ledStrips_displayPlayProgress(Adafruit_NeoPixel *strip, int beat);
  void ledStrips_displayComposeStatus(Adafruit_NeoPixel *strip);
  
#endif
