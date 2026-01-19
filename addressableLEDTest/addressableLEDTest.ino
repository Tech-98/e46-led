#include <FastLED.h>
// #include "FastLED_RGBW.h"
#include <simpleRPC.h>

#define DATA_PIN 23
#define DATA_PIN_PILL 22
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 300

int fadeInDuration = 90;
CRGB leds[NUM_LEDS];
void setup() {
  // put your setup code here, to run once:
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS_1));
  FastLED.addLeds<WS2812B, DATA_PIN_PILL, GRB>(leds, NUM_LEDS);
  delay(1000);
  blank();
  fadeIn(100,30,255,1,50,300);
}

void loop() {
  // put your main code here, to run repeatedly:
  // for( int j = 0; j < 100)
  //   if(j< 100){
  //     j++;
  //   }else if( j < 1){
  //     j--;
  //   }
  // for(int i = 0; i < NUM_LEDS; i++){
  //   leds[i] = CRGB(255,0,0);
  // }
  // FastLED.show();

  interface(
      Serial,
      blank, F("blank: Blanks LEDs and stops all animations."),
      showColor, F("showColor: show color r, g, b"),
      fadeOut, F("fadeOut: fade out"),
      fadeIn, F("fadeIn: fade in"),
      fill, F("fillIn: fill in" )
);

}

void fadeOut(int r, int g, int b, int duration, int increment, int led)
{
  for(int j = increment; j >= 0; j--){

    for (int i = 0; i < led; i++)
    {
      leds[i] = CRGB(int(r*j/increment), int(g*j/increment), int(b*j/increment));
    }
    
    FastLED.show();
    delay((duration*1000)/increment);
  }
  
}

void fadeIn(int r, int g, int b, int duration, int increment, int led)
{
  for(int j = 0; j <= increment; j++){

    for (int i = 0; i < led; i++)
    {
      leds[i] = CRGB(int(r*j/increment), int(g*j/increment), int(b*j/increment));
    }
    
    FastLED.show();
    delay((duration*1000)/increment);
  }
  
}

void blank()
{
  for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB(0, 0, 0);
    }
    
    FastLED.show();
}

void showColor(int r, int g, int b, int led)
{
  blank();
  for (int i = 0; i < led; i++)
  {
    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
}
void showRGB(int r, int g, int b, int i)
{
  if(i > 0 && i< NUM_LEDS){
      leds[i] = CRGB(r, g, b);
  }
}
void fill(int r, int g, int b, int led, int len, float duration ){
  for(int i = 0; i < (led/ 2); i++){
      for(int j = 0; j < len; j++){
        showRGB(int(r*j/len),int(g*j/len),int(b*j/len),i-j);
        showRGB(int(r*j/len),int(g*j/len),int(b*j/len),NUM_LEDS-i+j);
      }
      for(int j = len; j < (len*2); j++){
        showRGB(int(r*(len-j)/len),int(g*(len-j)/len),int(b*(len-j)/len), i-j);
        showRGB(int(r*(len-j)/len),int(g*(len-j)/len),int(b*(len-j)/len),NUM_LEDS-i+j);
      }
      delay((duration*1000)/(led/ 2));
      FastLED.show();
  }
}

void showVectorFrame(Vector<int> &frame)
{
  Vector<int> r(frame.size);

  for (size_t i{0}; i < frame.size; ++i)
  {
    r[i] = frame[i];
  }

  int checkSum = 0;

  for (int i = 0; i < 136; i++)
  {
    leds[i] = CRGB(frame[(i * 3) + 0], frame[(i * 3) + 1], frame[(i * 3) + 2]);
    leds[300 - i] = CRGB(frame[(i * 3) + 0], frame[(i * 3) + 1], frame[(i * 3) + 2]);
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

  for (int i = 0; i < 136; i++)
  {
    uint8_t r = frame[(i * 3) + 0];
    uint8_t g = frame[(i * 3) + 1];
    uint8_t b = frame[(i * 3) + 2];

    uint8_t rOnLightbar = leds[i].r;
    uint8_t gOnLightbar = leds[i].g;
    uint8_t bOnLightbar = leds[i].b;

    if (position < fadeInDuration)
    {
      r = (double)rOnLightbar + (double(r - rOnLightbar)  * position / fadeInDuration);
      g = (double)gOnLightbar + (double(g - gOnLightbar)  * position / fadeInDuration);
      b = (double)bOnLightbar + (double(b - bOnLightbar)  * position / fadeInDuration);
    }
    leds[i] = CRGB(r,g,b);
    leds[300 - i] = CRGB(r,g,b);
    checkSum = checkSum + frame[(i * 3) + 0] + frame[(i * 3) + 1] + frame[(i * 3) + 2];
  }

  if (checkSum == frame[408])
  {
    FastLED.show();
  }
}

