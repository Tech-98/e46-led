#include <TimerOne.h>
#include <FastLED.h>
#include <math.h>
#include <simpleRPC.h>
//#include "playthrough.h"
#include "vector.tcc"

int max_frame = 4140;
#define DATA_PIN 2
#define DATA_PIN2 5
#define DATA_PIN3 6
#define DATA_PIN4 7
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS_1 75
#define NUM_LEDS_2 75
#define NUM_LEDS_3 92
#define NUM_LEDS_4 92
#define ANIM_LENGTH 27
#define ANIM_INTRO_LENGTH 7

const int timeStamps[17] = {0, 1, 2, 6, 12, 17, 28, 36, 41, 54, 55, 60, 65, 70, 106, 137, 137};
int fadeInDuration = 90;
int fadeInPosition = 0;
volatile int frame = 0;
volatile int BRIGHTNESS = 100;
CRGB leds1[NUM_LEDS_1];
CRGB leds2[NUM_LEDS_2];
CRGB leds3[126];
CRGB leds4[126];

byte led1SolidColor[3];
byte led2SolidColor[3];
int led1Lenght = 100;
int led2Lenght = 100;
int led1Gradient = 100;
int led2Gradient = 100;

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  //Timer1.initialize(50000);
  Serial.begin(57600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds1, NUM_LEDS_1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds2, NUM_LEDS_2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN3, COLOR_ORDER>(leds3, 126).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN4, COLOR_ORDER>(leds4, 126).setCorrection(TypicalLEDStrip);
  //idle();
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
      setLEDGradient, F("setLEDGradient: Set global illumination brightness. @brightness: Brightness, 0 - 255."),
      setLEDLenght, F("setLEDLenght: Set global illumination brightness. @brightness: Brightness, 0 - 255."),
      setHSV, F("setHSV: Set global illumination brightness. @brightness: Brightness, 0 - 255."),
      setLed, F("setLed: Set global illumination brightness. @num: Brightness, 0 - 255."),
      showVectorFrame, F("showVectorFrame: show frame from vector. @frame: frame data")
);
}

void stopAnimation()
{
  Timer1.detachInterrupt();
}

void blank()
{
  stopAnimation();

  for (int i = 0; i < NUM_LEDS_1; i++)
  {
    leds1[i] = CRGB(0, 0, 0);
    leds2[i] = CRGB(0, 0, 0);
  }

  for (int i = 0; i < 136; i++)
  {
    leds3[i] = CRGB(0, 0, 0);
    leds4[i] = CRGB(0, 0, 0);
  }

  FastLED.show();
}

void idle()
{
  stopAnimation();

  for (int i = 0; i < NUM_LEDS_1; i++)
  {
    leds1[i] = CRGB(128, 128, 128);
    leds2[i] = CRGB(128, 128, 128);
    leds3[i] = CRGB(128, 128, 128);
    leds4[i] = CRGB(128, 128, 128);
  }

  FastLED.show();
}

void showHSV(int stripNo, byte h, byte s, byte v)
{
  
  FastLED[stripNo].showColor(CHSV(h, s, v));
  if(stripNo == 0){
    led1SolidColor[0] = h;
    led1SolidColor[1] = s;
    led1SolidColor[2] = v;
  }else if(stripNo == 1){
    led2SolidColor[0] = h;
    led2SolidColor[1] = s;
    led2SolidColor[2] = v;
  }
}

void showRGB(int stripNo, int r, int g, int b)
{
  stopAnimation();
  FastLED[stripNo].showColor(CRGB(r, g, b));
}

void setLEDBrightness(int lenght, int gradient)
{
  led2Lenght = lenght;
  led2Gradient = gradient;
  showColors();
}

void setHSV(int stripNo, byte h, byte s, byte v)
{
  if(stripNo == 0){
    led1SolidColor[0] = h;
    led1SolidColor[1] = s;
    led1SolidColor[2] = v;
  }else if(stripNo == 1){
    led2SolidColor[0] = h;
    led2SolidColor[1] = s;
    led2SolidColor[2] = v;
  }
  showColors();
}

void setLEDLenght(int id, int lenght)
{
  if (id == 0){
    led1Lenght = lenght;
  }else{
    led2Lenght = lenght;
  }
  showColors();
}

void setLEDGradient(int id, int gradient)
{
  if (id == 0){
    led1Gradient = gradient;
  }else{
    led2Gradient = gradient;
  }
  showColors();
}
void showColors(void)
{
  int LED2_On = (led2Lenght * NUM_LEDS_2) /  100;
  
  for (int i = 0; i < NUM_LEDS_2; i++)
  {
    leds2[i] = CRGB(0,0,0);
  }
  for (int i = 0; i < LED2_On; i++)
  {    
    leds2[(NUM_LEDS_2 + i + ((NUM_LEDS_2-LED2_On)/2)) % NUM_LEDS_2] = CHSV(led2SolidColor[0],led2SolidColor[1],led2SolidColor[2]);
  }
  int gradient2 = led2Gradient  * (NUM_LEDS_2 - LED2_On) / 200;

  for (int i = 0; i < gradient2; i++)
  { 
    if((NUM_LEDS_2-LED2_On)/2 - i > 0){ 
      int j = gradient2 -i;
      leds2[(NUM_LEDS_2-LED2_On)/2 - i] = CHSV(led2SolidColor[0],led2SolidColor[1],j*int(led2SolidColor[2])/gradient2);
      leds2[(NUM_LEDS_2-LED2_On)/2 + LED2_On + i] = CHSV(int(led2SolidColor[0]),int(led2SolidColor[1]),j*int(led2SolidColor[2])/gradient2);
    }
  
  }

//--------

  int LED1_On = (led1Lenght * NUM_LEDS_1) /  100;
  
  for (int i = 0; i < NUM_LEDS_1; i++)
  {
    leds1[i] = CRGB(0,0,0);
  }
  for (int i = 0; i < LED1_On; i++)
  {    
    leds1[(NUM_LEDS_1 + i + ((NUM_LEDS_1-LED1_On)/2)) % NUM_LEDS_1] = CHSV(led1SolidColor[0],led1SolidColor[1],led1SolidColor[2]);
  }
  int gradient1 = led1Gradient  * (NUM_LEDS_1 - LED1_On) / 200;

  for (int i = 0; i < gradient1; i++)
  { 
    if((NUM_LEDS_1-LED1_On)/2 - i > 0){ 
      int j = gradient1 -i;
      leds1[(NUM_LEDS_1-LED1_On)/2 - i] = CHSV(led1SolidColor[0],led1SolidColor[1],j*int(led1SolidColor[2])/gradient1);
      leds1[(NUM_LEDS_1-LED1_On)/2 + LED1_On + i] = CHSV(int(led1SolidColor[0]),int(led1SolidColor[1]),j*int(led1SolidColor[2])/gradient1);
    }
  
  }
  FastLED.show();
}

void setLed(int num)
{
  for (int i = 0; i < num; i++)
  {
    leds4[i] = CRGB(100,100,100);
  }
  FastLED.show();
}

void showVectorFrame(Vector<int> &frame)
{

  for (int i = 0; i < (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4); i++)
  {
    if(i < NUM_LEDS_1){
      leds1[i] = CRGB(frame[(i * 3) + 0], frame[(i * 3) + 1], frame[(i * 3) + 2]);
    }

    else if(i > NUM_LEDS_1 && i < (NUM_LEDS_1 +  NUM_LEDS_2)){
      leds2[i - NUM_LEDS_1] = CRGB(frame[(i * 3) + 0], frame[(i * 3) + 1], frame[(i * 3) + 2]);
    }
 
    else if(i > (NUM_LEDS_1 +  NUM_LEDS_2) && i < (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3)){
      leds3[i - (NUM_LEDS_1 + NUM_LEDS_2) + 20] = CRGB(frame[(i * 3) + 0], frame[(i * 3) + 1], frame[(i * 3) + 2]);
    }

    else if(i > (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 ) && i < (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4 ) ){
      leds4[i - (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 ) + 20] = CRGB(frame[(i * 3) + 0], frame[(i * 3) + 1], frame[(i * 3) + 2]);
    }
  }

  FastLED.show();
}
