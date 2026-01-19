#include <FastLED.h>
#include "FastLED_RGBW.h"
#include <simpleRPC.h>

#define DATA_PIN 23
#define DATA_PIN_PILL 22
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS_1 100

int encoderPin1 = 34;
int encoderPin2 = 33;
int encoderPin3 = 36;
int encoderPin4 = 35;
int buttonPin1 = 14;
int buttonPin2 = 37;

volatile int lastEncoded = 0;
volatile long encoderValueSide = 150;
volatile long encoderValuePill = 255;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;
int colorCycle = 0;
int colorCyclePill = 0;
int colorLength = 4;
int skip = 1;
int pillBrightness = 255; // default pill leds brightness
int sideBrightness = 150; // default side leds brightness

CRGBW ledsPill[22];
CRGB *ledsRGBPill = (CRGB *)&ledsPill[0];
CRGBW leds[NUM_LEDS_1];
CRGB *ledsRGB = (CRGB *)&leds[0];
// colors to cycle the side leds
CHSV colors[4] = {
    CHSV(158, 203, sideBrightness),
    CHSV(178, 255, sideBrightness),
    CHSV(246, 83, sideBrightness),
    CHSV(155, 12, sideBrightness),
};
// colors to cycle the pill leds
CHSV colorsPill[4] = {
    CHSV(158, 203, sideBrightness),
    CHSV(178, 255, sideBrightness),
    CHSV(246, 83, sideBrightness),
    CHSV(155, 12, sideBrightness),
};

void setup(void)
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  pinMode(encoderPin3, INPUT_PULLUP);
  pinMode(encoderPin4, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS_1));
  FastLED.addLeds<WS2812B, DATA_PIN_PILL, RGB>(ledsRGBPill, getRGBWsize(22));

  attachInterrupt(encoderPin1, updateEncoder, CHANGE);
  attachInterrupt(encoderPin2, updateEncoder, CHANGE);
  attachInterrupt(buttonPin2, buttonPillUpdate, RISING);
  attachInterrupt(buttonPin1, buttonUpdate, RISING);
  attachInterrupt(encoderPin3, updateEncoderPill, CHANGE);
  attachInterrupt(encoderPin4, updateEncoderPill, CHANGE);
  updateEncoder();
}

void loop(void)
{
  delay(50);
  showColor();
}

void blank()
{
  for (int i = 0; i < NUM_LEDS_1; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
  FastLED.show();
}

void showColor()
{
  int index = 0;
  leds[0] = CHSV(colors[(int)colorCycle].h, colors[(int)colorCycle].s, encoderValueSide);
  for (int i = 1; i <= NUM_LEDS_1; i++)
  {
    index++;
    if (index > skip)
    {
      index = 0;
      leds[i] = CHSV(colors[(int)colorCycle].h, colors[(int)colorCycle].s, encoderValueSide);
    }
  }
  for (int i = 0; i <= 22; i++)
  {
    ledsPill[i] = CHSV(colorsPill[(int)colorCyclePill].h, colorsPill[(int)colorCyclePill].s, encoderValuePill);
  }
  FastLED.show();
}

void updateEncoderPill()
{
  int MSB = digitalRead(encoderPin3); // MSB = most significant bit
  int LSB = digitalRead(encoderPin4); // LSB = least significant bit
  int encoded = (MSB << 1) | LSB; // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoderValuePill++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoderValuePill--;

  lastEncoded = encoded; // store this value for next time

  if (encoderValuePill >= 255)
  {
    encoderValuePill = 255;
  }
  else if (encoderValuePill <= 0)
  {
    encoderValuePill = 0;
  }

  Serial.println("PILL Brightness:");
  Serial.println(encoderValuePill);
}

void updateEncoder()
{
  int MSB = digitalRead(encoderPin1); // MSB = most significant bit
  int LSB = digitalRead(encoderPin2); // LSB = least significant bit
  int encoded = (MSB << 1) | LSB; // converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    encoderValueSide++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    encoderValueSide--;
  lastEncoded = encoded; // store this value for next time
  if (encoderValueSide >= 255)
  {
    encoderValueSide = 255;
  }
  else if (encoderValueSide <= 0)
  {
    encoderValueSide = 0;
  }
  Serial.println("Side Brightness:");
  Serial.println(encoderValueSide);
}

void buttonUpdate()
{
  colorCycle = colorCycle + 1;
  if (colorCycle >= colorLength)
  {
    colorCycle = 0;
  }
  colorCyclePill = colorCycle;
}

void buttonPillUpdate()
{
  colorCyclePill = colorCyclePill + 1;
  if (colorCyclePill >= colorLength)
  {
    colorCyclePill = 0;
  }
}
