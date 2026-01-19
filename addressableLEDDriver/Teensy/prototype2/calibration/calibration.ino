#include <FastLED.h>
#include "FastLED_RGBW.h"
#include <simpleRPC.h>
 
#define DATA_PIN 23
#define DATA_PIN_PILL 22
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 100
 
int encoderPin1 = 34;
int encoderPin2 = 33;
int encoderPin3 = 36;
int encoderPin4 = 35;
int buttonPin1 = 14;
int buttonPin2 = 37;
 
 
volatile int lastEncoded = 0;
volatile long hue = 180;
volatile long saturation = 200;
volatile long brightness = 250;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;
int colorCycle = 0;
int colorCyclePill = 0;
int pillLED = 50;
int skip = 1;
int pillBrightness = 255;
int sideBrightness = 150;
 
CRGBW ledsPill[22];
CRGB *ledsRGBPill = (CRGB *) &ledsPill[0];
CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

CHSV colors[5] = {
  CHSV(280, 255, 30),
  CHSV(239, 255, 30),
  CHSV(184, 255, 30)
};

CHSV colorsPill[5] = {
  CHSV(100, 255, 30),
  CHSV(180, 255, 30),
  CHSV(170, 255, 30)
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
 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
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
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
  FastLED.show();
}
 
void showColor()
{
  int index = 0;
  leds[0] = CHSV(hue, saturation, brightness);
  for (int i = 1; i <= NUM_LEDS; i++)
  {  
    index++;
    if(index > skip){
      index = 0;
      leds[i] = CHSV(hue, saturation, brightness);
    }
  }
 
  for (int i = 0; i <= 22; i++)
  {
     ledsPill[i] = CHSV(hue, saturation, brightness);
  }
 
  FastLED.show();
}
 
void updateEncoderPill(){
  int MSB = digitalRead(encoderPin3); //MSB = most significant bit
  int LSB = digitalRead(encoderPin4); //LSB = least significant bit
  // Serial.println(
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) saturation ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) saturation --;
 
  lastEncoded = encoded; //store this value for next time
  // saturation = saturation - 3;
  if(saturation > 255){
    saturation = 255;
  }else if(saturation < 0){
    saturation = 0;
  }
 
  Serial.println("Hue:");
  Serial.println(hue);
  Serial.println("Saturation:");
  Serial.println(saturation);
  Serial.println("Brightness:");
  Serial.println(brightness);
}
 
void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  // Serial.println(
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) hue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) hue --;
 
  lastEncoded = encoded; //store this value for next time
  if(hue > 255){
    hue = 0;
  }else if(hue < 0){
    hue = 255;
  }
  Serial.println("Hue:");
  Serial.println(hue);
  Serial.println("Saturation:");
  Serial.println(saturation);
  Serial.println("Brightness:");
  Serial.println(brightness);
}

void buttonUpdate(){
  brightness = brightness - 2;
  if (brightness < 0){
    brightness = 0;
  }
  Serial.println("Hue:");
  Serial.println(hue);
  Serial.println("Saturation:");
  Serial.println(saturation);
  Serial.println("Brightness:");
  Serial.println(brightness);
}
 
void buttonPillUpdate(){
  brightness = brightness + 2;
  if (brightness > 255){
    brightness = 255;
  }
  Serial.println("Hue:");
  Serial.println(hue);
  Serial.println("Saturation:");
  Serial.println(saturation);
  Serial.println("Brightness:");
  Serial.println(brightness);
}