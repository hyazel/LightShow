float convr,convg,convb,convuv=0;
void hsv2r(float hue){
  /*  if(hue<10){
      convr = 255;
      convg = 0;
      convb = 0;
      return;
    }
  
    if(hue<200){
      convr = 0;
      convg = 0;
      convb = 255;
      return;
    }
  
    if(hue<255){
      convr = 150;
      convg = 0;
      convb = 255;
      return;
    }*/
    
   double r, g, b;
hue = hue/255.0;
float l = 0.5;
    if (saturation == 0) {
        convr = convg = convb = 255; // achromatic
    } else {
      float s = saturation/255.0;
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hue2rgb(p, q, hue + 1/3.0);
        g = hue2rgb(p, q, hue);
        b = hue2rgb(p, q, hue - 1/3.0);
    }

    convr = r * 255;
    convg = g * 255;
    convb = b * 255;
}

double hue2rgb(double p, double q, double t) {
    if(t < 0) t += 1;
    if(t > 1) t -= 1;
    if(t < 1/6.0) return p + (q - p) * 6 * t;
    if(t < 1/2.0) return q;
    if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
    return p;
}

void initLights(){
  DmxSimple.usePin(DMX_PIN);
  for(int i=0;i<255;i++) //DMX RESET
    DmxSimple.write(i, 0);  
}

void lights(){
//  float b = PAR_BRIGHT_OFFSET*brightness;
  float b = brightness;
  float b1,b2;

  if(saturation>=100){
    hsv2r(tint);
  } else{
    convr = convb = convg = 255;
  }


  switch(strobeMode){
  case 1:  {
      b1 = date%2==0 ? b : 0;
      b2 = date%2==1 ? b : 0;
    }  break;
  case 2: {
      b1 = random(255)<127 ? b : 0;
      b2 =random(255)<127 ? b : 0 ;
    } break;
  case 3 :{
      b1 = b;
      b2 = b;
    }  break;
  case 4 :{
      b1 = b*random(255)/255.0;
      b2 = b*random(255)/255.0;
    }  break;
  case 11: {
      b1 = date%2==0 ? b : 0;
      b2 = 0; } 
    break;
  case 12:  {
      b1 = random(255)<127 ? b : 0;
      b2 = 0; } 
    break;
  case 13: {
      b1 = b;
      b2 = 0;
    }  break;
  case 14: {
      b1 = b*random(255)/255.0;
      b2 = 0;
    }  break;
  default: {
      b1 = 0;
      b2 = 0;
    }
  }

#ifdef DMX_MODERN
if(uv>0){
  convr = convb = convg = 0;
}
  convuv = uv*255*4; //adaptation

  DmxSimple.write(DMX_PAR_CHAN, convr*b1);
  DmxSimple.write(DMX_PAR_CHAN+1, convg*b1);
  DmxSimple.write(DMX_PAR_CHAN+2, convb*b1);
  DmxSimple.write(DMX_PAR_CHAN+3, convuv*b1);

  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET, convr*b2);
  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET+1, convg*b2);
  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET+2, convb*b2);
  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET+3, convuv*b2);
#else
  DmxSimple.write(DMX_PAR_CHAN, b1*255);
  DmxSimple.write(DMX_PAR_CHAN+1, convr);
  DmxSimple.write(DMX_PAR_CHAN+2, convg);
  DmxSimple.write(DMX_PAR_CHAN+3, convb);
//  DmxSimple.write(DMX_PAR_CHAN+4, convuv);

  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET, b2*255);
  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET+1, convr);
  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET+2, convg);
  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET+3, convb);
//  DmxSimple.write(DMX_PAR_CHAN+DMX_PAR_OFFSET+4, convuv);
#endif
}

