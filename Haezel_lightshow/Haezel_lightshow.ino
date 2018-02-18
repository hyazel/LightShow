

#include <Adafruit_NeoPixel.h>
#include <DmxSimple.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

////LLOOGG
//added guards
//suppr strobe
//added uv mode

#define GRID 1s
#define PIN 6
//#define NUM_LEDS_PER_METER 16*5/4*3
#define NUM_LEDS_PER_METER 32
#define NUM_LEDS 5*NUM_LEDS_PER_METER
#define MAX_STRIPS 1

#define DMX_STROBE_CHAN 37
#define DMX_PAR_CHAN 1
#define DMX_PAR_OFFSET 10
#define DMX_PIN 3
#define DMX_MODERN 1 //1:modern

class GridElt{
  
public:

  GridElt(Adafruit_NeoPixel* strip, int ps, int pe){
    this->strip = strip;
    this->ps = ps;
    this->pe = pe;
  }

  void show(){
    strip->show();
  }

  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b){
    if(n < numPixels())
      strip->setPixelColor(n+ps, r, g, b);
  }

  void setPixelColor(uint16_t n, uint32_t c){
    if(n < numPixels())
      strip->setPixelColor(n+ps, c);
  }

  int numPixels(){
    return pe-ps;
  }

  Adafruit_NeoPixel* getStrip(){
    return strip;
  }

  uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel* strip, uint8_t c, int saturation) {
    WheelPos = 255 - WheelPos;
    int r, g, b;
    if(WheelPos < 85) {
      r = (255 - WheelPos * 3);
      g = 0;
      b =  (WheelPos * 3);
    }
    else{
      if(WheelPos < 170) {
        WheelPos -= 85;
        r = 0;
        g = (WheelPos * 3);
        b = (255 - WheelPos * 3);
      }
      else{
        WheelPos -= 170;
        r = (WheelPos * 3);
        g = 255 - WheelPos * 3;
        b = 0;
      }
    }

    r = map(saturation, 255, 0, r, 255);
    g = map(saturation, 255, 0, g, 255/2);
    b = map(saturation, 255, 0, b, 255/2);

    r *= c/255.0;
    g *= c/255.0;
    b *= c/255.0;

    return strip->Color(r, g, b, 0);
  }

  void setColor(int i, uint8_t c, int tint, int saturation){
    if(i < numPixels())
      setPixelColor(i, Wheel(255 - byte(tint) & 255, getStrip(), c, saturation));
  }

private:
  Adafruit_NeoPixel* strip;
  int ps,pe;
};


//byte pixels[7][5]; //WATCH OUT FOR MAX

int auto_ = 0;
int autoRate = 5;

int mode = 3;
int info = 10; //nb drops, random size
int sync = 0;
float brightness = 0.1;
int event = 0;

byte tint = 0;
byte saturation = 0;
byte uv=0;
int strobeMode = 4;
int del_ay = 45;


int agentStrips[10], agentPixels[10];

Adafruit_NeoPixel* strips[MAX_STRIPS];
#ifdef GRID
#define NB_ELT MAX_STRIPS*2
#else
#define NB_ELT MAX_STRIPS
#endif
GridElt* grid[NB_ELT];
GridElt* gridStrips[MAX_STRIPS];
float stripCol[NB_ELT];
float evolution[NB_ELT];
int NB_LED=0;
int LO_LED= 1000;
int HI_LED= 0;

long elapsed = 0;
char buffer[25];
int date=0;
int rDate = 0;
int dateShot = 1;
int eventDate = 0;
int evtId = 0;

boolean isLights = false;

int frameRate = 16;

float PAR_BRIGHT_OFFSET = 0.1;

//**********************************************************************************************************************************************************  
void setup() {
  if(isLights){
    initLights();
  }

  delay(300);
  Serial.begin(115200);
  randomSeed(analogRead(1));
  int numLeds[MAX_STRIPS];
  numLeds[0] = 240;//NUM_LEDS_PER_METER*2.0;
  numLeds[2] = NUM_LEDS_PER_METER*0.75;
  numLeds[1] = NUM_LEDS_PER_METER*1.25; //warning inv !
  numLeds[3] = NUM_LEDS_PER_METER*0.5;
  numLeds[5] = NUM_LEDS_PER_METER*1.25;
  numLeds[4] = NUM_LEDS_PER_METER*0.75;
  numLeds[6] = NUM_LEDS_PER_METER*2.0;
  //    for(int i=0;i<MAX_STRIPS;i++)
  //    numLeds[i] *= 2;

  for(int i=0;i<MAX_STRIPS;i++){
    NB_LED += numLeds[i];
    LO_LED = min(numLeds[i], LO_LED);
    HI_LED = max(numLeds[i], HI_LED);
  }


// for(int i=0;i<MAX_STRIPS;i++)
//  for(int j=0;j<5;j++)
//     pixels[i][j] = 0;

for(int s=0;s<NB_ELT;s++)
      stripCol[s] = 0;

  for(int i=0;i<MAX_STRIPS;i++){
    strips[i] = 0;
    initStrip(i, numLeds[i]); // do dynamic
    strips[i]->setBrightness(brightness*255);
    evolution[i] = i / 4.0;
  }

  //manual calibration of the grid (down/up down/up down/up)
  int gridNb = 6;
  int gridIndexes[NB_ELT];
  for(int i=0;i<NB_ELT;i++)
    gridIndexes[i] = numLeds[int(i/2)]/2.0;
  for(int i=0;i<MAX_STRIPS;i++){
#ifdef GRID
    grid[i] = new GridElt(strips[i], 0, gridIndexes[i*2]);
    grid[i+MAX_STRIPS] = new GridElt(strips[i], gridIndexes[i*2], gridIndexes[i*2]+gridIndexes[i*2+1]);
    gridStrips[i] = new GridElt(strips[i], 0,(gridIndexes[i]+gridIndexes[i*2+1]));
#else
    grid[i] = new GridElt(strips[i], 0,gridIndexes[i]);
#endif
  }

  Serial.setTimeout(50);

  changeMode();
  initAgents();
}

//mode ?
void loop() {  
//  Serial.println(F("free RAM : "));
//  Serial.println(freeRam());
//Serial.flush();
  
  //mode = 30;
  //evolution[0] = 0.25;
  manageIO();

  delay(frameRate);
  elapsed += frameRate;

  if(elapsed > del_ay){

    elapsed -= del_ay;
    if(elapsed < 0) elapsed = 0;

/**** EVENT MGMT ******/
    if(auto_ && date%autoRate == 0){ //emulate the event
      event = 1;
    }

    if(event>0){ //tag the event
      dateShot = 0; 
      eventDate = date;
//      event = 0;
    }
/**********/

    date ++;

    fx();

    if(isLights)
      lights();

    if(event>0)
      event = 0;
  }
}

void fx(){
  switch(mode){
  case 2:
    ramp();
    break;
  case 1:
    flash();
    break;
  case 0:
  case 127:
    bright(0);
    break;
  case 3:
    bright(255);
    break;
  case 4:
    strobe();
    break;
  case 5 :
    info = 10;  
    rain();
    break;
  case 6 :
    info = 15;
    rain();
    break;  
  case 26 :
    random_();
    break;
  case 7:
    oneLight();
    break;
  case 8:
    oneRain();
    break;
  case 9:
    servers();
    break;
  case 11:
    evol();
    break;
  case 12:
    evol2();
    break;
  case 13:
    oneStrike();
    break;
  case 14:
    oneRing();
    break;
  case 15:
    oneScape();
    break;
  case 16:
    oneBlock();
    break;
    case 17:
    oneWave();
    break;
  case 20:
    seq();
    break;
  case 21:
    points();
    break;
  case 22:
    uniseq();
    break;
  case 25:
    agentRain();
    break;
  case 30:
    prana();
    break;
  default:
    bright(0);
  }
}

void changeMode(){
  switch(mode){
  case 4:
    for(int s=0;s<NB_ELT;s++)
      stripCol[s] = (sync==1) ? 1 : ((random(100)<50) ? 1 : 0);
    break;
  case 25:
    {
      initAgents();
    }
    break;
  }
}

void initAgents(){
  clearAgents();
  for(int i=0;i<MAX_STRIPS;i++){
    agentStrips[i] = i;
    agentPixels[i] =  (sync==1) ? 0 : random(gridStrips[i]->numPixels());
  }
}

void clearAgents(){
  for(int i=0;i<MAX_STRIPS;i++){
    agentStrips[i] = -1;
    agentPixels[i] = -1;
  }
}






