#include <SD.h>
#include <SPI.h>
#include <FastLED.h>
#include "FastLED_RGBW.h"

#define DATA_PIN 2
#define DATA_PIN2 5
#define DATA_PIN3 6
#define DATA_PIN4 7
#define DATA_PILL 8

#define LEFT_LEDS 358
#define RIGHT_LEDS 358
#define TOP_LEDS 125
#define BOTTOM_LEDS 135
#define NUM_PILLS 8
#define LEDS_PER_PILL 8

#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define TOTAL_LEDS 976
#define PILL_LEDS 64

byte ledData[TOTAL_LEDS * 3 + PILL_LEDS * 4];
CRGBW leds[NUM_PILLS * LEDS_PER_PILL]; // 8 pills
CRGB *ledsRGB = (CRGB *) &leds[0];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DATA_PILL, OUTPUT);
  Serial.begin(57600);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>((CRGB *) &ledData[0], LEFT_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>((CRGB *) &ledData[LEFT_LEDS * 3], RIGHT_LEDS).setCorrection(TypicalLEDStrip); // NUM_LEDS_1
  FastLED.addLeds<LED_TYPE, DATA_PIN3, COLOR_ORDER>((CRGB *) &ledData[(LEFT_LEDS + RIGHT_LEDS) * 3], TOP_LEDS).setCorrection(TypicalLEDStrip); // (NUM_LEDS_1 + NUM_LEDS_2)
  FastLED.addLeds<LED_TYPE, DATA_PIN4, COLOR_ORDER>((CRGB *) &ledData[(LEFT_LEDS + RIGHT_LEDS + TOP_LEDS) * 3], BOTTOM_LEDS).setCorrection(TypicalLEDStrip); //(NUM_LEDS_1 + NUM_LEDS_2 + MIDDLE_LEDS_TO_LIGHT)
  FastLED.addLeds<WS2812B, DATA_PILL, RGB>((CRGB *) &ledData[(LEFT_LEDS + RIGHT_LEDS + TOP_LEDS + BOTTOM_LEDS) * 3], 86);
  Serial.println("Ready to go");

  Serial.println(TOTAL_LEDS * 3 + PILL_LEDS * 4);
}

void loop() {
  Serial.readBytes(ledData, TOTAL_LEDS * 3 + PILL_LEDS * 4);

  FastLED.show();
}