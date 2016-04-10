#include <Adafruit_NeoPixel.h>
#include "SparkTunes_LedStrips.h"

//test all the LED strips. We just need to test the "core" leds works: R, G and B
void ledStrips_test(Adafruit_NeoPixel *strip) {
  uint32_t colour[3];
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
  for (int led = 0; led < MAX_NUM_LEDS; led++)
    strip->setPixelColor(led, 0, 0, 0);
  strip->show();
}

//flash LEDs 3 times. Accepts an array of LED indexes. Each LED needs to be mapped a colour
void ledStrips_flashLeds(Adafruit_NeoPixel *strip, int leds[], uint32_t colour[], unsigned long pulse) {
  for (int iteration = 0; iteration < 3; iteration++) {
    ledStrips_reset(strip);

    delay(pulse);

    for (int i = 0; i < sizeof(leds) / sizeof(int); i++)
      strip->setPixelColor(leds[i], colour[i]);
    strip->show();
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
  //int offset-
  //random mappign feature- since we have 16 switches shared between 30 LEDs, we'll
  //assign 2 LEDs
  //TODO: implement me
  return;
}

void ledStrips_displayComposeStatus(Adafruit_NeoPixel *strip) {
  uint32_t goldenYellow = strip->Color(255, 247, 0);
  ledStrips_reset(strip);
  for (int led = 0 ; led < LEDS_PER_STRIP * NUM_STRIPS; led++)
    strip->setPixelColor(led, goldenYellow);
  strip->show();
}
