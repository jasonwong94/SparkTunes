#include <Adafruit_NeoPixel.h>
#include "SparkTunes_LedStrips.h"

//test all the LED strips. We just need to test the "core" leds works: R, G and B
void ledStrips_test(Adafruit_NeoPixel *strip) {
  uint32_t colour[4];
  colour[0] = strip->Color(255, 0, 0);
  colour[1] = strip->Color(0, 255, 0);
  colour[2] = strip->Color(0, 0, 255);
  colour[3] = strip->Color(255, 255, 255);
  
  for(int i = 0; i < 4; i++){
     for (int led = 0; led < MAX_NUM_LEDS; led++) {
      strip->setPixelColor(led, colour[i]);
    }
    strip->show();
    delay(100); 
  }

//  red ->yellow -> green
//  for (red = 255; red >= 0; red--) {
//    for (green = 0; green < 256; green++) {
//      strip->setPixelColor(led, red, green, blue);
//      delay(10);
//    }
//  }
//
//  //green -> blue
//  for (green = 255; green >= 0; green--) {
//    for (blue = 0; blue < 256; blue++) {
//      strip->setPixelColor(led, red, green, blue);
//      delay(10);
//    }
//  }
//
//  //blue -> red
//  for (blue = 255; blue >= 0; blue--) {
//    for (red = 0; red < 256; red++) {
//      strip->setPixelColor(led, red, green, blue);
//      delay(10);
//    }
//  }
}

//clears out all the values
void ledStrips_reset(Adafruit_NeoPixel *strip) {
  ledStrips_setColour(strip, strip->Color(0,0,0) );
}

//flash LEDs 3 times of the SAME colour. Accepts an array of LED indexes. 
void ledStrips_setColour(Adafruit_NeoPixel *strip, int leds[], uint32_t colour){
  ledStrips_reset(strip);
  for (int i = 0; i < sizeof(leds) / sizeof(int); i++)
    strip->setPixelColor(leds[i], colour);
  strip->show();
}

void ledStrips_setColour(Adafruit_NeoPixel *strip, uint32_t colour){
  for (int i = 0; i < MAX_NUM_LEDS; i++)
    strip->setPixelColor(i, colour);
  strip->show();
}

void ledStrips_setColour(Adafruit_NeoPixel *strip, int leds[], uint32_t colour[]){
  ledStrips_reset(strip);
  for (int i = 0; i < sizeof(leds) / sizeof(int); i++)
    strip->setPixelColor(leds[i], colour[i]);
  strip->show();
}

//flash LEDs 3 times. Accepts an array of LED indexes. Each LED needs to be mapped a colour
void ledStrips_flashLeds(Adafruit_NeoPixel *strip, int leds[], uint32_t colour[], unsigned long pulse) {
  for (int iteration = 0; iteration < PULSE_ITERATION; iteration++) {
    ledStrips_reset(strip);
    delay(pulse);
    ledStrips_setColour(strip, leds, colour);
    delay(pulse);
  }
}

//a visual indicator to indicate which iteration (out of 5) the music is playing
void ledStrips_displayIterations(Adafruit_NeoPixel *strip, int iteration) {
  uint32_t green = strip->Color(0, 255, 100);
  int numLEDs = MAX_NUM_LEDS / 5;
  int LEDs[numLEDs];
  uint32_t LEDcolour[numLEDs];

  int upper_led = numLEDs * iteration;
  int lower_led = (iteration == 0) ? 0 : numLEDs * (iteration - 1);

  ledStrips_reset(strip);
  //want to turn all the LEDs up to the upper_led

  for (int led = 0; led < upper_led; led++) {
    strip->setPixelColor(led, green);
  }
  strip->show();

  for (int i = 0; i < numLEDs; i++) {
    LEDs[i] = lower_led;
    LEDcolour[i] = green;
    lower_led++;
  }

  ledStrips_flashLeds(strip, LEDs, LEDcolour, 500);
}

void ledStrips_displayPlayProgress(Adafruit_NeoPixel *strip, int beat) {
  int num_beats = 32;
  int effective_num_leds = MAX_NUM_LEDS - 1;
  beat++;
  int turn_on_up_to = beat * effective_num_leds / num_beats;
  const uint32_t ON_COLOR = strip->Color(0,255,0);
  const uint32_t OFF_COLOR = strip->Color(0,0,0);
  
  for (int i = 0; i < MAX_NUM_LEDS; i++) {
    int on = (i >= (effective_num_leds - turn_on_up_to + 1));
    strip->setPixelColor(i, on ? ON_COLOR : OFF_COLOR);
  }
  strip->show();

  return;
}

void ledStrips_displayComposeStatus(Adafruit_NeoPixel *strip) {
  uint32_t goldenYellow = strip->Color(255, 247, 0);
  for (int led = 0 ; led < MAX_NUM_LEDS; led++)
    strip->setPixelColor(led, goldenYellow);
  strip->show();
}
