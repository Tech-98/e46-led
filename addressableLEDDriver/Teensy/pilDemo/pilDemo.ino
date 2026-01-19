
#include <FastLED.h>
#include "FastLED_RGBW.h"
#include <simpleRPC.h>

#define DATA_PIN 2
#define DATA_PIN2 5
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS_1 50
#define NUM_LEDS_2 50

// CRGB leds1[NUM_LEDS_1];
// CRGB leds2[NUM_LEDS_2];

CRGBW leds[NUM_LEDS_1];
CRGB *ledsRGB = (CRGB *) &leds[0];

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  //Timer1.initialize(50000);
  Serial.begin(57600);
  // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds1, NUM_LEDS_1).setCorrection(TypicalLEDStrip);
  // FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds2, NUM_LEDS_2).setCorrection(TypicalLEDStrip);
  // FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds, NUM_LEDS_1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, DATA_PIN2, RGB>(ledsRGB, getRGBWsize(NUM_LEDS_1));
  idle();
}

void loop(void)
{
  interface(
    Serial,
    showColor, F("showColor: show color. @pixelNum: number of pixel to light up. @r: red. @g: green. @b: blue. @w: white"),
    blank, F("blank: show blank."),
    idle, F("idle: show idle.")
    // showHsv, F("showHsv: show hsv color. @h: hue. @s: saturation. @v: brightness. @ledNum: ledNum. @pixelNum: pixel number. @ledBrightness: led brightness. ")
   );
}

void blank()
{


  for (int i = 0; i < NUM_LEDS_2; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
    // leds1[0] = CRGB(0, 0, 0);
    // leds2[i] = CRGB(0, 0, 0);
  }

  FastLED.show();
}

void idle()
{


  for (int i = 0; i < NUM_LEDS_2; i++)
  {
    leds[i] = CRGBW(255, 255, 255, 0);
    // leds1[0] = CRGB(0, 0, 0);
    // leds2[i] = CRGB(0, 0, 0);
  }

  FastLED.show();
}

void showColor(int pixelNum, int r, int g, int b, int w) {
  //pixelNum = pixelNum +4;
  blank();

  // leds2[0] = CRGB(r, g, b);
  for (int i = 0; i < NUM_LEDS_2; i++)
  {
  //   leds2[46 - i] = CRGB(r, g, b);
  //   // leds1[i] = CRGB(r, g, b);
    leds[i] = CRGBW(r, g, b, w);
  }
  FastLED.show();

}

// void showHsv(int h, int s, int b, int ledNum, int pixelNum, int ledBrightness) {
//   pixelNum = pixelNum +4;
//   blank();
//   if ( ledNum == 1 ) {
//     for (int i = 0; i < pixelNum; i++)
//     {
//       leds1[46-i] = CHSV(h, s, b);
//     }
//   }else if ( ledNum == 2 ){
//     for (int i = 0; i < pixelNum; i++)
//     {
//       leds2[46-i] = CHSV(h, s, b);
//     }
//   }else{
//     for (int i = 0; i < pixelNum; i++)
//     {
//       leds1[46-i] = CHSV(h, s, b);
//     }

//     for (int i = 0; i < pixelNum; i++)
//     {
//       leds2[46-i] = CHSV(h, s, b);
//     }
//   }

//   FastLED.show();

// }
