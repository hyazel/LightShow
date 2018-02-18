void manageIO(){

// 0 : mode
// 1 : sync différentes trips
// 2 : bright
// 3 : Info
// 4 : Delay
// 5 : evenement id
// 6 : Hue
// 7 : saturation
// 10-15 : level meter

int bufferSize = 2;
  if(Serial.available() > 0){
  
    if(Serial.readBytes(buffer, 2) >= 2){
      switch(buffer[0]){
        case 98:{
        brightness = buffer[1] / 255.0;
        for(int i=0;i<MAX_STRIPS;i++)
            strips[i]->setBrightness(brightness*255);
        }break;
        case 116:
        tint = buffer[1]*2;
        break;
        case 117:
        saturation = buffer[1]*2;
        break;
        case 100:{
        int newmode = (int)buffer[1];
        if(newmode!=mode){
          mode=newmode;
          changeMode();
        }
        
        break;}
        }

        while(Serial.read() != -1);
    }
 }
 /*
  int bufferSize = 24;
 if(Serial.available() > 0){
    
    if(Serial.readBytesUntil('\n', buffer, bufferSize+1) >= bufferSize){
      int off = 0;
      int newmode = (int) buffer[0];
      if(newmode!=mode)
        changeMode();
      mode = newmode;
      sync = (int) buffer[1];
      float b = (float) ((buffer[2]) / 255.0);
      if(b>1)
      b = 1;
      if(b != brightness){
        brightness += (b-brightness)*0.8; //easing
        for(int i=0;i<MAX_STRIPS;i++)
          strips[i]->setBrightness(brightness*255);
      }

      info = (int) buffer[3];
      if(info<0) info = 0;

      del_ay = (int) buffer[4];

      //event = 0; //test comment
      if(evtId != (int) buffer[5]){
        evtId = buffer[5];
        event = 1;
      }

      for(int i=0;i<NB_ELT && i<5;i++)
        evolution[i] = buffer[i+10]/127.0;

//      if(event==0)
//        event = (int) ((buffer[5]));

      tint = (int) ((buffer[6]));
      saturation = (int) ((buffer[7]));
      strobeMode =  (int) buffer[8];
      uv = (int) buffer[9];

      //changeMode();
      while(Serial.read() != -1); //try commenting this
    }
  }
  */
}
