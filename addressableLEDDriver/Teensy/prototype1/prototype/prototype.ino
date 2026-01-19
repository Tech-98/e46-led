#include <SD.h>
#include <SPI.h>
#include <FastLED.h>
#include "FastLED_RGBW.h"

#define DATA_PIN 2
#define DATA_PIN2 5
#define DATA_PIN3 6
#define DATA_PIN4 7
#define DATA_PILL 8
#define NUM_LEDS_1 225
#define NUM_LEDS_2 225
#define NUM_LEDS_3 92
#define NUM_LEDS_4 92
#define NUM_PILL 140
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

File Test;
String filename = "test1.txt";
char inputString[100];
String inputChar;
int stringIndex = 0;  // String stringIndexing int;
String strs[2000];
int StringCount = 0;
CRGB leds1[NUM_LEDS_1];
CRGB leds2[NUM_LEDS_2];
CRGB leds3[126];
CRGB leds4[126];
CRGBW leds[NUM_PILL];
CRGB *ledsRGB = (CRGB *) &leds[0];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DATA_PILL, OUTPUT);
  Serial.begin(57600);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds1, NUM_LEDS_1).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds2, NUM_LEDS_2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN3, COLOR_ORDER>(leds3, 126).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN4, COLOR_ORDER>(leds4, 126).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, DATA_PILL, RGB>(ledsRGB, getRGBWsize(NUM_PILL));
  cardinit();
  readdata();
}

void cardinit() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("ititialization failed");
    while (1)
      ;
  }
  Serial.println("SD Card Initialized Successfully");
  Serial.println("-----------------------------\n");
}

void readdata() {
  Serial.println("trying to read");
  int index = 0;
  char stringArray[1000];
  int next;
  char buf[20];
  Test = SD.open(filename.c_str(), FILE_READ);
  if (Test) {

    int rlen = Test.available();
    char ch = Test.read();
    Serial.print("ch:");
    Serial.print(ch);
  } else {
    Serial.println("The SD card failed to initialize");
    while (1)
      ;
  }
}

void showPillLed(int r, int g, int b, int num){
  for (int i = 0; i < 8; i++) {
    leds[i + (num * 8)] = CRGBW(r, g, b, 0);
  }
}

void loop() {
  inputChar = Test.readStringUntil('\n');  // Gets one byte from serial buffer
  if (Test.readStringUntil('\n'))          // define breaking char here (\n isnt working for some reason, i will follow this up later)
  {
    FastLED.show();
    StringCount = 0;
    while (inputChar.length() > 0) {
      int index = inputChar.indexOf(' ');
      if (index == -1)  // No space found
      {
        strs[StringCount++] = inputChar;
        break;
      } else {
        strs[StringCount++] = inputChar.substring(0, index);
        inputChar = inputChar.substring(index + 1);
      }
    }
    for (int i = 0; i < (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4 + 9); i++) {
      if (i < NUM_LEDS_1) {
        leds1[i] = CRGB(strs[(i * 3) + 0].toInt(), strs[(i * 3) + 1].toInt(), strs[(i * 3) + 2].toInt());
      }
      else if (i > NUM_LEDS_1 && i < (NUM_LEDS_1 + NUM_LEDS_2)) {
        leds2[i - NUM_LEDS_1] = CRGB(strs[(i * 3) + 0].toInt(), strs[(i * 3) + 1].toInt(), strs[(i * 3) + 2].toInt());
      }
      else if (i > (NUM_LEDS_1 + NUM_LEDS_2) && i < (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3)) {
        leds3[i - (NUM_LEDS_1 + NUM_LEDS_2) + 20] = CRGB(strs[(i * 3) + 0].toInt(), strs[(i * 3) + 1].toInt(), strs[(i * 3) + 2].toInt());
      }
      else if (i > (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3) && i < (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4)) {
        leds4[i - (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3) + 20] = CRGB(strs[(i * 3) + 0].toInt(), strs[(i * 3) + 1].toInt(), strs[(i * 3) + 2].toInt());
      }
      else if (i > (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4) && i < (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4 + 9)) {
        showPillLed(strs[(i * 3) + 0].toInt(), strs[(i * 3) + 1].toInt(), strs[(i * 3) + 2].toInt(), i - (NUM_LEDS_1 + NUM_LEDS_2 + NUM_LEDS_3 + NUM_LEDS_4) - 1);
      }
    }
    FastLED.show();
  }
}