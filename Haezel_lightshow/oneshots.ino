int oneShotStrip = 0;

//void oneLight(){
//  if(dateShot==0||auto_){
//    if(sync){
//      oneShotStrip = oneShotStrip+1;
//      if(oneShotStrip>=NB_ELT)
//        oneShotStrip = 0;
//    }
//    else{
//      int oldStrip = oneShotStrip;
//      do{
//        oneShotStrip = random(NB_ELT);
//      }
//      while(oneShotStrip == oldStrip);
//    }
//  }
//
//  for(int s=0;s<NB_ELT;s++){
//    GridElt* strip = grid[s];
//    uint8_t c = 0;
//    if(s==oneShotStrip && eventDate>date-4)
//      c = 255;
//    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++)
//      strip->setColor(i, c, tint);
//    strip->show();
//  }
//  dateShot++;
//}

void oneLight(){
  if(dateShot==0||auto_){
    //    for(int i=0;i<NB_ELT;i++){
    //    evolution[i] = 0;
    //    }
    int count = 0;
    if(sync){
      for(int i=0;i<NB_ELT;i++){
        if(evolution[i]>0){
          evolution[i] = 0;
          evolution[i % NB_ELT] = 1;
        }
        evolution[i] = 0;
      }
      oneShotStrip = oneShotStrip+1;
      if(oneShotStrip>=NB_ELT)
        oneShotStrip = 0;
    }
    else{
      //      int oldStrip = oneShotStrip;
      //      do{
      //        oneShotStrip = random(NB_ELT);
      //      }
      //      while(oneShotStrip == oldStrip);
      for(int i=0;i<NB_ELT;i++){
        evolution[i] = 0;
      }
    }

    while(count<event){ // random fill
      for(int i=0;i<NB_ELT && count<event;i++)
        if(random(100)<50){
          evolution[i] = 1;
          count ++;
        }
      count ++;
    }
  }

  for(int s=0;s<NB_ELT;s++){
    GridElt* strip = grid[s];
    uint8_t c = 0;
    if(evolution[s]>0 && eventDate>date-4)
      c = 255;
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++)
      strip->setColor(i, c, tint, saturation);
    strip->show();
  }
  dateShot++;
}

void oneRing(){
  if(dateShot==0||auto_){
    if(sync){
      oneShotStrip = oneShotStrip+1;
      if(oneShotStrip>=NB_ELT)
        oneShotStrip = 0;
    }
    else{
      int oldStrip = oneShotStrip;
      do{
        oneShotStrip = random(NB_ELT);
      }
      while(oneShotStrip == oldStrip);
    }
  }

  for(int s=0;s<MAX_STRIPS;s++){
    GridElt* strip = gridStrips[s];

    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){
      uint8_t c = 0;
      if(i!=oneShotStrip)
        c = 255;
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
  dateShot++;
}

void oneScape(){
  if(dateShot==0||auto_){
    if(sync){
      oneShotStrip = oneShotStrip+1;
      if(oneShotStrip>=NB_ELT)
        oneShotStrip = 0;
    }
    else{
      int oldStrip = oneShotStrip;
      do{
        oneShotStrip = random(NB_ELT);
      }
      while(oneShotStrip == oldStrip);
    }
  }

  for(int s=0;s<MAX_STRIPS;s++){
    GridElt* strip = gridStrips[s];

    uint8_t c = 0;
    if(abs(s-3) == -eventDate+date)
      c = 255;

    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){

      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
  dateShot++;
}

void oneBlock(){
  if(dateShot==0){

    int oldStrip = oneShotStrip;
    for(int s=0;s<MAX_STRIPS;s++){
      stripCol[s] = random(100)<50 ? 0 : 1;
    }
  }

  float fade = 1 - (dateShot>5 ? 5:dateShot)/5.0;

  for(int s=0;s<MAX_STRIPS;s++){
    GridElt* strip = gridStrips[s];
    uint8_t c = 0;
    if(stripCol[s]>0)
      c = 255;

    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++)
      strip->setColor(i, c*fade, tint, saturation);
    strip->show();
  }
  dateShot++;
}





void oneRain(){
  float speed_ = info*0.1 + 0.1;
//  dateShot = 0;
  if((dateShot==0)){
//  if((event==1)){
    if(sync)
      oneShotStrip = (oneShotStrip+1) % MAX_STRIPS;
    else{
      int oldStrip = oneShotStrip;
      do{
        oneShotStrip = random(MAX_STRIPS);
      }
      while(oneShotStrip == oldStrip);
    }
    evolution[oneShotStrip] = 1;
  }

  for(int s=0;s<MAX_STRIPS;s++){
    GridElt* strip = gridStrips[s];
    uint8_t c = 0;
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++) {
      c = 0;
      if(abs(i - evolution[s]*strip->numPixels()) < 0.6)
        c = 255;
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
        evolution[s] -= speed_;
//        if(evolution[s]<0)
//          evolution[s] = 0;
  }
  dateShot++;
}





void oneStrike(){
  int speed_ = 6/1;

  if((dateShot==0 || auto_>0) && dateShot%speed_==0){
    if(sync)
      oneShotStrip = (oneShotStrip+1) % NB_ELT;
    else{
      int oldStrip = oneShotStrip;
      do{
        oneShotStrip = random(NB_ELT);
      }
      while(oneShotStrip == oldStrip);
    }
  }

  for(int s=0;s<NB_ELT;s++){
    GridElt* strip = gridStrips[s];
    uint8_t c = 0;
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++) {
      c = 0;
      if(s==oneShotStrip && i*speed_/float(strip->numPixels()) <= (dateShot%speed_))
        c = 255;
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
  dateShot++;
}

float tmp[MAX_STRIPS];
void oneWave(){
 /* float fade = 0.25;
  float fade2 = 0.5;

  if(dateShot==0){    
//    oneShotStrip = getRandomStrip();
oneShotStrip = random(MAX_STRIPS);
//    stripCol[oneShotStrip] = 0.5;

int solo = 0;
for(int i=0;i<10;i+=2)
if(agentPixels[i]==0){
solo = i;
  break;
}
    agentPixels[oneShotStrip] = 1;
    agentPixels[oneShotStrip+1] = -1;
  }

  for(int s=0;s<MAX_STRIPS;s++){
    GridElt* strip = gridStrips[s];
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++)
      strip->setColor(i,  stripCol[s]*255, tint, saturation);
    strip->show();
  }
  
  for(int i=0;i<10;i+=2){
    if(agentPixels[i]!=0){
      agentPixels[i] -= 0.2;
    }
  }
  
    for(int s=0;s<MAX_STRIPS;s++){
      tmp[s] = 0;
  }
  
    for(int s=0;s<MAX_STRIPS-1;s++)
  if(stripCol[s]>0)
  tmp[s+1] = tmp[s+1]+stripCol[s]/2;
      for(int s=MAX_STRIPS-1;s>0;s--)
  if(stripCol[s]>0)
  tmp[s-1] = tmp[s-1]+stripCol[s]/2;
  
  for(int s=0;s<MAX_STRIPS;s++){
//          stripCol[s] = max(0, stripCol[s]-fade);    
  stripCol[s] = tmp[s]+stripCol[s];
  
  if(stripCol[s]>1)
  stripCol[s] = 1;
  
            stripCol[s] = stripCol[s]-fade;  
            
          if(stripCol[s] < 0)
        stripCol[s] = 0;
}
  
  dateShot++;*/
}




