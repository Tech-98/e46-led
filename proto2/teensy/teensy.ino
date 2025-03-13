#define LIR_PIN 2
#define LIG_PIN 0
#define LIB_PIN 1

#define RIR_PIN 4
#define RIG_PIN 3
#define RIB_PIN 5

#define LMR_PIN 7
#define LMG_PIN 11
#define LMB_PIN 6

#define RMR_PIN 9
#define RMG_PIN 10
#define RMB_PIN 8

#define RI_IN_PIN 24
#define LI_IN_PIN 25
// #define TPIN_0 15
#define Loop_Delay 10

int mainColor[3]  = {225, 175, 255};
int indicatorColor[3]  = {255,60,0};
int black[3]  = {0, 0, 0};

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillisL;
unsigned long currentMillisR;

int wakeDelay = 10;
int ind = 0;

// int indicatorColorR = 255,140,0;

int LIR = 0;
int LIG = 0;
int LIB = 0;

int RIR = 0;
int RIG = 0;
int RIB = 0;

int RMR = 0;
int RMG = 0;
int RMB = 0;

int LMR = 0;
int LMG = 0;
int LMB = 0;

int sumR = 0;
int sumL = 0;
int highR = 0;
int highL = 0;

int inc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  startMillis = millis();

  pinMode(LIR_PIN, OUTPUT);
  pinMode(LIG_PIN, OUTPUT);
  pinMode(LIB_PIN, OUTPUT);

  pinMode(RIR_PIN, OUTPUT);
  pinMode(RIG_PIN, OUTPUT);
  pinMode(RIB_PIN, OUTPUT);

  pinMode(LMR_PIN, OUTPUT);
  pinMode(LMG_PIN, OUTPUT);
  pinMode(LMB_PIN, OUTPUT);

  pinMode(RMR_PIN, OUTPUT);
  pinMode(RMG_PIN, OUTPUT);
  pinMode(RMB_PIN, OUTPUT);

  // pinMode(RI_IN_PIN, INPUT_PULLUP);
  // pinMode(LI_IN_PIN, INPUT_PULLUP);

  wake();

}

void rgbTest(){
  int r=0;
  int g=0;
  int b=0;

  for(int i = 0; i <=150; i++){
    r++;
    putAll(r,g,b);
    Serial.print("\nR;");
    Serial.print(r);
    Serial.print("G;");
    Serial.print(g);
    Serial.print("B;");
    Serial.print(b);
    delay(Loop_Delay);
  }

  r=0;
  g=0;
  b=0;
  for(int i = 0; i <=150; i++){
    g++;
    putAll(r,g,b);
    Serial.print("\nR;");
    Serial.print(r);
    Serial.print("G;");
    Serial.print(g);
    Serial.print("B;");
    Serial.print(b);
    delay(Loop_Delay);
  }

  r=0;
  g=0;
  b=0;
  for(int i = 0; i <=150; i++){
    b++;
    putAll(r,g,b);
    Serial.print("\nR;");
    Serial.print(r);
    Serial.print("G;");
    Serial.print(g);
    Serial.print("B;");
    Serial.print(b);
    delay(Loop_Delay);
  }
  Serial.print("\nR;");
  Serial.print(r);
  Serial.print("G;");
  Serial.print(g);
  Serial.print("B;");
  Serial.print(b);
}

void putAll(int r,int g,int b){

  analogWrite(LIR_PIN,r);
  analogWrite(LIG_PIN,g);
  analogWrite(LIB_PIN,b);

  analogWrite(RIR_PIN,r);
  analogWrite(RIG_PIN,g);
  analogWrite(RIB_PIN,b);

  analogWrite(LMR_PIN,r);
  analogWrite(LMG_PIN,g);
  analogWrite(LMB_PIN,b);

  analogWrite(RMR_PIN,r);
  analogWrite(RMG_PIN,g);
  analogWrite(RMB_PIN,b);


}

void putLI(int r,int g,int b){

  analogWrite(LIR_PIN,r);
  analogWrite(LIG_PIN,g);
  analogWrite(LIB_PIN,b);

  // analogWrite(RIR_PIN,r);
  // analogWrite(RIG_PIN,g);
  // analogWrite(RIB_PIN,b);

  // analogWrite(LMR_PIN,r);
  // analogWrite(LMG_PIN,g);
  // analogWrite(LMB_PIN,b);

  // analogWrite(RMR_PIN,r);
  // analogWrite(RMG_PIN,g);
  // analogWrite(RMB_PIN,b);


}

void putRI(int r,int g,int b){

  // analogWrite(LIR_PIN,r);
  // analogWrite(LIG_PIN,g);
  // analogWrite(LIB_PIN,b);

  analogWrite(RIR_PIN,r);
  analogWrite(RIG_PIN,g);
  analogWrite(RIB_PIN,b);

  // analogWrite(LMR_PIN,r);
  // analogWrite(LMG_PIN,g);
  // analogWrite(LMB_PIN,b);

  // analogWrite(RMR_PIN,r);
  // analogWrite(RMG_PIN,g);
  // analogWrite(RMB_PIN,b);


}
void putLM(int r,int g,int b){

  // analogWrite(LIR_PIN,r);
  // analogWrite(LIG_PIN,g);
  // analogWrite(LIB_PIN,b);

  // analogWrite(RIR_PIN,r);
  // analogWrite(RIG_PIN,g);
  // analogWrite(RIB_PIN,b);

  analogWrite(LMR_PIN,r);
  analogWrite(LMG_PIN,g);
  analogWrite(LMB_PIN,b);

  // analogWrite(RMR_PIN,r);
  // analogWrite(RMG_PIN,g);
  // analogWrite(RMB_PIN,b);


}
void putRM(int r,int g,int b){

  // analogWrite(LIR_PIN,r);
  // analogWrite(LIG_PIN,g);
  // analogWrite(LIB_PIN,b);

  // analogWrite(RIR_PIN,r);
  // analogWrite(RIG_PIN,g);
  // analogWrite(RIB_PIN,b);

  // analogWrite(LMR_PIN,r);
  // analogWrite(LMG_PIN,g);
  // analogWrite(LMB_PIN,b);

  analogWrite(RMR_PIN,r);
  analogWrite(RMG_PIN,g);
  analogWrite(RMB_PIN,b);


}
void fadeLI(int r, int g, int b, int interval, int time){
  float R = LIR;
  float G = LIG;
  float B = LIB;

  float difR= (r - R) / interval;
  float difG= (g - G) / interval;
  float difB= (b - B) / interval;

  for(int i =0; i <= interval; i++){
    R = R + difR;
    G = G + difG;
    B = B + difB;
    putLI((int)R ,(int)G ,(int)B );
    Serial.print("\n R: ");
    Serial.print((int)R);
  }
  delay(time);
}
void showRgb(){

  analogWrite(LIR_PIN,LIR);
  analogWrite(LIG_PIN,LIG);
  analogWrite(LIB_PIN,LIB);

  analogWrite(RIR_PIN,RIR);
  analogWrite(RIG_PIN,RIG);
  analogWrite(RIB_PIN,RIB);

  analogWrite(LMR_PIN,LMR);
  analogWrite(LMG_PIN,LMG);
  analogWrite(LMB_PIN,LMB);

  analogWrite(RMR_PIN,RMR);
  analogWrite(RMG_PIN,RMG);
  analogWrite(RMB_PIN,RMB);

  // Serial.print("\nRI R: ");
  // Serial.print(RIR);
  // Serial.print(" G: ");
  // Serial.print(RIG);
  // Serial.print(" B: ");
  // Serial.print(RIB);

}

int colorCorrect(int val){
  if(val > 254){
    val = 254;
  }else if (val < 0){
    val = 0;
  }
  return val;
}

void wake(){
  LIR = 0;
  LIG = 0;
  LIB = 0;

  RIR = 0;
  RIG = 0;
  RIB = 0;

  RMR = 0;
  RMG = 0;
  RMB = 0;

  LMR = 0;
  LMG = 0;
  LMB = 0;

  //wake main
  for(int i = 0; i <200; i++){
    RMR ++;
    RMB ++;

    LMR ++;
    LMB ++;
    colorCorrect(RMR);
    colorCorrect(RMB);
    colorCorrect(LMR);
    colorCorrect(LMB);

    showRgb();
    delay(wakeDelay);
  }




  //wake indicator
    for(int i = 0; i <255; i++){
    RIR ++;
    RIB ++;

    LIR ++;
    LIB ++;
    colorCorrect(RIR);
    colorCorrect(RIB);
    colorCorrect(LIR);
    colorCorrect(LIB);
    
    RMR ++;
    RMB ++;

    LMR ++;
    LMB ++;
    colorCorrect(RMR);
    colorCorrect(RMB);
    colorCorrect(LMR);
    colorCorrect(LMB);

    showRgb();
    Serial.print("\n i : ");
    Serial.print(i);
    // if( i == 50){
    //   Serial.println("glitch 1");

    //   glitchRI(200,25);
        
    // }
    // else if (i == 150){
    //   Serial.println("glitch 2");
    //   glitchRI(50,50);
    // }else if(i == 175){
    //   Serial.println("glitch 3");
    //   glitchRI(100,75);
    // }
    
    delay(wakeDelay);
  }
  //  for(int i = 0; i <=175; i++){

  //   showRgb();
  //   delay(wakeDelay);
  // }
   for(int i = 0; i <=175; i++){
    RIG ++;
    LIG ++;
    colorCorrect(RIG);
    colorCorrect(LIG);

    RMG ++;
    LMG ++;
    colorCorrect(LMG);
    colorCorrect(LMG);

    showRgb();
    delay(wakeDelay);
  }
}

// void glitchRI(unsigned long time, int low){
//   currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  
//   int r = RIR - low;
//   int g = RIG - low;
//   int b = RIB - low;
//   Serial.println(currentMillis - startMillis);
//   if (currentMillis - startMillis < time)  //test whether the period has elapsed
//   {
//     Serial.println("glitching");
//     Serial.println(currentMillis - startMillis);
//     putRI(r,g,b);
//   }

// }
void checkIndicatores(){
  sumR=0;
  sumL=0;
  // high = 0;
  for(int i =0; i < 51;i++){
    sumR = sumR + analogRead(21);
    sumL = sumL + analogRead(20);
    delay(1);
  }
  
  // put your main code here, to run repeatedly:
  int RIval = sumR/50;
  int LIval = sumL/50;
  putIndicators(LIval, RIval);
  
}
int limit = 500;
void putIndicators(int LIval, int RIval){
  // int LIval = analogRead(LI_IN_PIN);
  // int RIval = analogRead(RI_IN_PIN);
  // Serial.print("\n RI: ");
  // Serial.print(RIval);
  // Serial.print(" lI: ");
  // Serial.print(LIval);

  if(millis() - currentMillisR < 1000){
    
    putRI(0,0,0);
    ind = 0;
  }else if(millis() - currentMillisL < 1000){
    putLI(0,0,0);
    ind = 0;
  }
  if(RIval >= limit){
    currentMillisR = millis();
    // Serial.print("\tright on");
    // Serial.print(RIval);
    putRI(indicatorColor[0],indicatorColor[1],indicatorColor[2]);
    // delay(275);
    // putRI(0, 0, 0);
    // delay(275);
  }
  if(LIval >= limit){
    
    currentMillisL = millis();
    // fadeLI(255, 255, 0,20,10);
    // delay(1000);
    // fadeLI(0, 0, 0,20,10);
    // delay(1000);
    // Serial.print("\tleft on");
    // Serial.print(LIval);
    
    putLI(indicatorColor[0],indicatorColor[1],indicatorColor[2]);
    // delay(275);
    // putLI(0, 0, 0);
    // delay(275);
  // }else if(LIval <= 300 && RIval <= 300){
    
  //   currentMillis = millis();
  //   Serial.print("\n both on");
  //   putLI(255,60,0);
  //   putRI(255,60,0);
  //   delay(175);
  //   putLI(0, 0, 0);
  //   putRI(0, 0, 0);
  //   delay(175);
  }
    // delay(500);
    // 
    // Serial.print("\tmillis R:");
    // Serial.print(millis() - currentMillisR);
    // Serial.print("\tmillis L:");
    // Serial.print(millis() - currentMillisL);
  if(millis() - currentMillisR > 1000){
    putRI(mainColor[0],mainColor[1],mainColor[2]);
    ind = 0;
  }else if (millis() - currentMillisL > 1000){
    putLI(mainColor[0],mainColor[1],mainColor[2]);
    ind = 0;
  }
  // else{
  //   if(RIval < limit){
  //     putRI(0,0,0);
  //   }else if(LIval < limit){
  //     putLI(0,0,0);
  //   }
  // }


}

void printLoop(){
  sumR=0;
  sumL=0;
  // high = 0;
  for(int i =0; i < 21;i++){
    sumR = sumR + analogRead(21)/10;
    sumL = sumL + analogRead(20)/10;
    delay(1);
  }
  
  // put your main code here, to run repeatedly:
  int Rval = sumR/20;
  int Lval = sumL/20;
  if(Rval > highR){
    highR = Rval;
  }else if(Lval > highL){
    highL = Lval;
  }
  // float voltage = sensorValue * (5.0 / 1023.0);
  Serial.print("\n Rval: ");
  Serial.print(Rval);
  Serial.print(" \tLval: ");
  Serial.print(Lval);
  Serial.print(" \tHighR: ");
  Serial.print(highR);
  Serial.print(" \tHighL: ");
  Serial.print(highL);
  // delay(20);

  if(millis() - startMillis > 5000){
    highR = 0;
    highL = 0;
    startMillis = millis();
    Serial.print("\n..........................................................................................");
  }
}
void loop(){
  // putAll(0,0,0);
  // delay(2000);
  // rgbTest();
  // Serial.print("\n RI: ");
  // Serial.print(digitalRead(RI_IN_PIN));
  // Serial.print(" lI: ");
  // Serial.print(digitalRead(LI_IN_PIN));
  // delay(50);
// printLoop();
  // fadeLI(indicatorColor[0],indicatorColor[1],indicatorColor[2],200,1);
  // delay(2000);
  checkIndicatores();
}