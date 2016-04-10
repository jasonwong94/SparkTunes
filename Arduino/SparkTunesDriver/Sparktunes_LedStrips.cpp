#include <Adafruit_NeoPixel.h>
#include "SparkTunes_LedStrips.h"

//clears out all the values
void ledStrips_reset(Adafruit_NeoPixel *strip){
  for(int led=0; led<LEDS_PER_STRIP*NUM_STRIPS; led++)
    strip->setPixelColor(led, 0, 0, 0);
  strip->show();
}

//flashed LEDs all at the same time- LEDs don't have to be adjacent to each other but
//have to be the same colour
void ledStrips_flashLeds(Adafruit_NeoPixel *strip, int leds[], uint32_t colour[], unsigned long pulse){
  for(int iteration = 0; iteration < 3; iteration++){
    for(int i =0; i< sizeof(leds)/sizeof(int); i++)
      strip->setPixelColor(leds[i], 0, 0, 0);
    strip->show();
    
    delay(pulse);
    
    for(int i =0; i< sizeof(leds)/sizeof(int); i++)
      strip->setPixelColor(leds[i], colour[i]);
    strip->show();
  }
}

//a visual indicator to indicate which iteration (out of 5) the music is playing
void ledStrips_displayIterations(Adafruit_NeoPixel *strip, int iteration){
  uint32_t green = strip->Color(0, 255, 100);
  int numLEDs = LEDS_PER_STRIP*NUM_STRIPS/5;
  int LEDs[numLEDs];
  uint32_t LEDcolour[numLEDs];
  
  int upper_led = numLEDs*iteration;
  int lower_led = (iteration == 0) ? 0 : numLEDs*(iteration -1);

  ledStrips_reset(strip);
  //want to turn all the LEDs up to the upper_led

  for(int led=0; led<upper_led; led++){
    strip->setPixelColor(led, green);
  }
  strip->show();

  for(int i=0; i<numLEDs; i++){
    LEDs[i] = lower_led;
    LEDcolour[i] = green;
    lower_led++;
  }

  ledStrips_flashLeds(strip, LEDs, LEDcolour, 500);
}

void ledStrips_displayPlayProgress(Adafruit_NeoPixel *strip, int beat){
  //int offset- 
  //random mappign feature- since we have 16 switches shared between 30 LEDs, we'll
  //assign 2 LEDs 
  //TODO: implement me
  return;
}

void ledStrips_displayComposeStatus(Adafruit_NeoPixel *strip){
  uint32_t goldenYellow = strip->Color(255, 247, 0);
  ledStrips_reset(strip);
  for(int led=0 ; led<LEDS_PER_STRIP*NUM_STRIPS; led++)
    strip->setPixelColor(led, goldenYellow);

}
