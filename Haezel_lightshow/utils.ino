void initStrip(int i, int numLeds){
  if(i<MAX_STRIPS){
    Adafruit_NeoPixel* strip = new Adafruit_NeoPixel(numLeds, PIN+i, NEO_GRB + NEO_KHZ800);
    strip->begin();
    strip->show(); 
    strip->setBrightness(25);
    strips[i] = strip;
  }
}

void applyToStrip(int s, uint8_t c){
  GridElt* strip = grid[s];
  for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){
   strip->setColor(i, c, tint, saturation);
  }
  strip->show();
}

void applyToWholeStrip(int s, uint8_t c){
  GridElt* strip = gridStrips[s];
  for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){
   strip->setColor(i, c, tint, saturation);
  }
//  strip->show();
}

void bright(uint8_t b){
  for(int s=0;s<NB_ELT;s++)
    applyToStrip(s, b);
}

uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel* strip, uint8_t c) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip->Color((255 - WheelPos * 3)*c/255.0, 0, (WheelPos * 3)*c/255.0,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, (WheelPos * 3)*c/255.0, (255 - WheelPos * 3)*c/255.0,0);
  }
  WheelPos -= 170;
  return strip->Color((WheelPos * 3)*c/255.0, (255 - WheelPos * 3)*c/255.0, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

void setColor(Adafruit_NeoPixel* strip, int i, uint8_t c){
if(tint<0)
    strip->setPixelColor(i, c,c,c);
else
        strip->setPixelColor(i, Wheel(255 - byte(tint) & 255, strip, c));
}

int getPtGridElt(int pt){
  int count = 0;
  for(int i=0;i<NB_ELT;i++){
    count += grid[i]->numPixels();
    if(pt<count)
    return i;
  }
  return 0;
}

int getPtGridId(int pt){
  int count = 0;
  for(int i=0;i<NB_ELT;i++){
    count += grid[i]->numPixels();
    if(pt<count)
    return pt-count + grid[i]->numPixels();
  }
  return 0;
}

//void pushPixels(){
//  for(int i=0;i<MAX_STRIPS;i++){
//    for(int j=0;j<HI_LED;j++)
//        gridStrips[i]->setColor(j, pixels[i][j], tint);
//        gridStrips[i]->show();
//  }
//}

int getRandomStrip(){
    int oldStrip = oneShotStrip; //todo gfetrandom strip
    if(sync)
      oneShotStrip = (oneShotStrip+1) % MAX_STRIPS;
    else{
      do{
        oneShotStrip = random(MAX_STRIPS);
      } while(oneShotStrip == oldStrip);
    }
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
