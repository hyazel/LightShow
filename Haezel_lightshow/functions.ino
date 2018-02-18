/** GLOBAL FUNCTIONS **/

void random_(){
  for(int s=0;s<NB_ELT;s++){
    GridElt* strip = grid[s];
    uint8_t c = 0;
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++) {
      if(i%(info+1)==0){
        float f = random(127)/(float)127;
        c = f*f*255;
      }
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
}

void rain(){
  for(int s=0;s<NB_ELT;s++){
    GridElt* strip = grid[s];
    uint8_t c;
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++) {
      if((i%(NUM_LEDS/(info+1)))==rDate)
        c = 255;
      else
        c = 0;
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
  rDate++;
  if(rDate>=(NUM_LEDS/(info+1)))
    rDate = 0;
}

void ramp(){
  uint8_t c;
  for(int s=0;s<NB_ELT;s++){
    GridElt* strip = grid[s];
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){
      c = date*i/float(NUM_LEDS) * (255);
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
}


//random avec controle de densité
void points(){
  uint8_t c;
  for(int s=0;s<NB_ELT;s++){
    GridElt* strip = grid[s];
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){
      c = random(100)<evolution[0]*100 ? 255 : 0; 
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
}


void flash(){
  uint8_t c = random(255);
  for(int s=0;s<NB_ELT;s++){
    if(!sync) c = random(255);
    applyToStrip(s, c);
  }
}

void strobe(){
  uint8_t c;
  for(int s=0;s<NB_ELT;s++){
    stripCol[s] = (stripCol[s]<0.5) ? 1 : 0;
    c = (uint8_t) (stripCol[s] * 255);
    applyToStrip(s, c);
  }
}

void seq(){
  uint8_t c;
  for(int s=0;s<NB_ELT;s++){
    c = ((date+s)%NB_ELT)==0 ? 255 : 0;
    applyToStrip(s, c);
  }
}

void uniseq(){
  uint8_t c;
  int speed_ = info+5;
  
    for(int s=0;s<MAX_STRIPS;s++){
  applyToStrip(s, 0);
  }
  
  for(int s=0;s<LO_LED*MAX_STRIPS;s++){
    int dd = (s-date*speed_)%(LO_LED*MAX_STRIPS);
    c = ((dd) == 0) ? 255 : 0;
    gridStrips[s%MAX_STRIPS]->setColor(s/MAX_STRIPS, c, tint, saturation);
  }
  for(int s=0;s<MAX_STRIPS;s++)
  gridStrips[s]->show();
  
//  for(int s=0;s<MAX_STRIPS;s++){
//        for(int i=0; (i< gridStrips[s]->numPixels() && i<NUM_LEDS); i++){
//          c = i < LO_LED ? 255 : 0;
//      gridStrips[s]->setColor(i, c, tint);
//        }
//        gridStrips[s]->show();
//      }
}



int serverMode = 0;
void servers(){
       uint8_t c;

  switch(serverMode){
  case 0:
    {
      bright(255);
    }
    break;
  case 1:
    {
      for(int s=0;s<NB_ELT;s++){
        for(int i=0; (i< grid[s]->numPixels() && i<NUM_LEDS); i++){
          c = random(100) > 10 ? 255 : 0;
grid[s]->setColor(i, c, tint, saturation);
        }
        grid[s]->show();
      }
    }
    break;
    case 2 :{
      for(int s=0;s<NB_ELT;s++){
        for(int i=0; (i< grid[s]->numPixels() && i<NUM_LEDS); i++){
          c = random(100) > 25 ? 255 : 0;
      grid[s]->setColor(i, c, tint, saturation);
        }
        grid[s]->show();
      }
    }break;
    case 3 :{
      for(int s=0;s<NB_ELT;s++){
        for(int i=0; (i< grid[s]->numPixels() && i<NUM_LEDS); i++){
          c = random(100) > 50 ? 255 : 0;
      grid[s]->setColor(i, c, tint, saturation);
        }
        grid[s]->show();
      }
    }break;
  }
  
  if(random(100)<5)
  serverMode = random(4);
}

void evol(){
  /* for(int s=0;s<NB_ELT;s++){
        for(int i=0; (i< grid[s]->numPixels() && i<NUM_LEDS); i++)
//        if(info==0)
//              grid[s]->setColor(i, abs((i)/(float)(grid[s]->numPixels()-1) - 0.5) * 2 < evolution[s] ? 255 : 0, tint);
//          else
//                        grid[s]->setColor(i, i < (grid[s]->numPixels()-1)*evolution[s], tint);
                        grid[s]->setColor(i, 255*i*1/float(grid[s]->numPixels()-1)*0.5, tint);
        grid[s]->show();
    }*/
    
      uint8_t c;
  for(int s=0;s<MAX_STRIPS;s++){
    GridElt* strip = gridStrips[s];
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){
      c = i < (strip->numPixels()-1)*evolution[0] ? 255 : 0;// * 0.5;
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
}

void prana(){
   uint8_t c;
  for(int s=0;s<MAX_STRIPS;s++){
    GridElt* strip = gridStrips[s];
    for(int i=0; (i< strip->numPixels() && i<NUM_LEDS); i++){
      //c = i < (strip->numPixels()-1)*evolution[0] ? 255 : 0;// * 0.5;
      float index = (1 - i / float(strip->numPixels()))*255;
c = (index - 200 + evolution[0]*2);
//c = (1 - i / float(strip->numPixels()))*255;
float d = index - evolution[0]*2;
if(abs(d) < 50)
c -= random(0, 255);
      strip->setColor(i, c, tint, saturation);
    }
    strip->show();
  }
}

void evol2(){
   for(int s=0;s<NB_ELT;s++){
        for(int i=0; (i< grid[s]->numPixels() && i<NUM_LEDS); i++)
                                grid[s]->setColor(i, abs((i/(float)(grid[s]->numPixels()-1)) - evolution[s]) < 0.1 ? 255 : 0, tint, saturation);
        grid[s]->show();
    }
}


void agentRain(){
  int speed_ = 1 + info;
  
  for(int s=0;s<MAX_STRIPS;s++)
  applyToWholeStrip(s, 0);
  
  for(int i=0;i<MAX_STRIPS;i++){
    int s = agentStrips[i];
    if(s>=0){
    int p = agentPixels[i];
    gridStrips[s]->setColor(p, 255, tint, saturation);
    agentPixels[i] = (p + speed_);
    while(agentPixels[i]>gridStrips[s]->numPixels())
      agentPixels[i] -= gridStrips[s]->numPixels();
    }
  }
  
    for(int s=0;s<MAX_STRIPS;s++)
  gridStrips[s]->show();
}

