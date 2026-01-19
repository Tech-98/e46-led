#include <TimerOne.h>
#include <FastLED.h>
#include <math.h>
#include <simpleRPC.h>
#include "vector.tcc"

int max_frame = 4140;
const uint8_t playthroughArr[4140][136][3] PROGMEM;
#define DATA_PIN 2
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 136
#define NUM_LEDS_2 24

int fadeInDuration = 90;
volatile int BRIGHTNESS = 100;
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS_2];

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  Timer1.initialize(50000);
  Serial.begin(57600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<NEOPIXEL, 5>(leds2, NUM_LEDS_2).setCorrection(TypicalLEDStrip);
  idle();
}

void stopAnimation()
{
  Timer1.detachInterrupt();
}

void blank()
{
  stopAnimation();

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }

  FastLED.show();
}

void idle()
{
  stopAnimation();

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(128, 128, 128);
  }

  FastLED.show();
}

void showHSV(int stripNo, byte h, byte s, byte v)
{
  stopAnimation();
  FastLED[stripNo].showColor(CHSV(h, s, v));
}

void showRGB(int stripNo, int r, int g, int b)
{
  stopAnimation();
  FastLED[stripNo].showColor(CRGB(r, g, b));
}

void setLEDBrightness(byte brightness)
{
  FastLED.setBrightness(brightness);
  FastLED.show();
}

void showVectorFrame(Vector<int> &frame)
{
  Vector<int> r(frame.size);

  for (size_t i{0}; i < frame.size; ++i)
  {
    r[i] = frame[i];
  }

  int checkSum = 0;

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[(NUM_LEDS + i - 25) % NUM_LEDS] = CRGB(frame[(i * 3) + 0], frame[(i * 3) + 1], frame[(i * 3) + 2]);
    checkSum = checkSum + frame[(i * 3) + 0] + frame[(i * 3) + 1] + frame[(i * 3) + 2];
  }

  if (checkSum == frame[408])
  {
    FastLED.show();
  }
}

void showFadeinFrame(Vector<int> &frame, int position)
{
  int checkSum = 0;

   

  for (int i = 0; i < NUM_LEDS; i++)
  {
    uint8_t r = frame[(i * 3) + 0];
    uint8_t g = frame[(i * 3) + 1];
    uint8_t b = frame[(i * 3) + 2];

    uint8_t rOnLightbar = leds[(NUM_LEDS + i - 25) % NUM_LEDS].r;
    uint8_t gOnLightbar = leds[(NUM_LEDS + i - 25) % NUM_LEDS].g;
    uint8_t bOnLightbar = leds[(NUM_LEDS + i - 25) % NUM_LEDS].b;

    if (position < fadeInDuration)
    {
      r = (double)rOnLightbar + (double(r - rOnLightbar)  * position / fadeInDuration);
      g = (double)gOnLightbar + (double(g - gOnLightbar)  * position / fadeInDuration);
      b = (double)bOnLightbar + (double(b - bOnLightbar)  * position / fadeInDuration);
    }
    leds[(NUM_LEDS + i - 25) % NUM_LEDS] = CRGB(r,g,b);
    checkSum = checkSum + frame[(i * 3) + 0] + frame[(i * 3) + 1] + frame[(i * 3) + 2];
  }

  if (checkSum == frame[408])
  {
    FastLED.show();
  }
}

void loop(void)
{
  interface(
      Serial,
      idle, F("idle: Set LEDs to idle white state"),
      blank, F("blank: Blanks LEDs and stops all animations."),
      showHSV, F("showHSV: Show HSV color on LED ring. @stripNo: Strip number, 0 for LED ring 1 for other. @h: Hue. @s: Saturation. @v: Value."),
      showRGB, F("showRGB: Show rgb colour on whole strip. @stripNo: Strip number, 0 for LED ring 1 for other. @r: Red. @g: Green. @b: Blue."),
      setLEDBrightness, F("setLEDBrightness: Set global illumination brightness. @brightness: Brightness, 0 - 255."),
      showVectorFrame, F("showVectorFrame: show frame from vector. @frame: frame data"),
      showFadeinFrame, F("showFadeinFrame: show frame from vector with a fade. @frame: frame data"));
}
