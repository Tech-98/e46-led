int encoderPin1 = 2;
int encoderPin2 = 3;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT_PULLUP); 
  pinMode(encoderPin2, INPUT_PULLUP);

  //call updateEncoder() when any high/low changed seen
  attachInterrupt(2, updateEncoder, CHANGE); 
  attachInterrupt(3, updateEncoder, CHANGE);

}

void loop(){ 
  // Serial.println(encoderValue);
  // delay(500); //just here to slow down the output, and show it will work  even during a delay

  showPurple(encoderValue);
}

void showPurple(int brightness)
{
  for (int i = 0; i < NUM_LEDS_1; i++)
  {
    leds[i] = CRGBW(100, 0, 255, 0);
  }
  FastLED.setBrightness(brightness);
  FastLED.show();
}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  // Serial.println(
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

  lastEncoded = encoded; //store this value for next time
  if(encoderValue >= 255){
    encoderValue = 255;
  }else if(encoderValue <= 0){
    encoderValue = 0;
  }
}
