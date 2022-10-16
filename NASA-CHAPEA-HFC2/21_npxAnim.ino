//==
//==
//==

void bayStatusIndicator(){
/*
 * NOTE: 5mm NPX are GRB
 */
  
  for (int bay = 0; bay < 3; bay++){
    
    byte bottlePresent = HFCinPlace[bay];
    byte remainingCharge = bottleChargeLevel[bottlePresent];
    
    if (HFCinPlace[bay]){
      statusLED.setPixelColor(24+bay,100,100,0);
      if (remainingCharge == 0){
        statusLED.setPixelColor(24+bay,0,150,0);
      }
      else if (bay == activeCell-1){
        statusLED.setPixelColor(24+bay,0,0,150);
      }
    }
    else{
      statusLED.setPixelColor(24+bay,0,0,0);
    } 
  }
  statusLED.show();
}

//--


void displayRemainingCharge(byte bayNum){

/*
 * NOTE: 8PX NPX bars are RGB
 */

  byte bottlePresent = HFCinPlace[bayNum];
  byte remainingCharge = bottleChargeLevel[bottlePresent];

  byte barPxl = 0;
  byte red = (255 - remainingCharge) / 2;
  byte grn = (remainingCharge / 2);
  for (int pxl = bayNum * 8; pxl < bayNum * 8 + 8; pxl++){
    if (barPxl <= remainingCharge / 32){
      statusLED.setPixelColor(pxl,red,grn,0);
    }
    else{
      statusLED.setPixelColor(pxl,0);
    }
    barPxl++;

    if (!remainingCharge){
      statusLED.setPixelColor(bayNum*8,0);
    }

//    if (remainingCharge){
//      statusLED.setPixelColor(bayNum*8,red,grn,0);
//    }
  }
  statusLED.show();
}

void bootUpTest(){
  for (int bayNum = 0; bayNum < 3; bayNum++){
    
    byte barPxl = 0;
    
    for (int frame = 0; frame < 128; frame++){
      byte red = frame;
      byte grn = 128 - frame;
      for (int pxl = bayNum * 8; pxl < bayNum * 8 + 8; pxl++){
        if (barPxl <= frame /16){
          statusLED.setPixelColor(pxl,red,grn,0);
        }
        else{
          statusLED.setPixelColor(pxl,0);
        }
        statusLED.show();
      }
      barPxl++;
      delay (10);
    }
    for (int frame = 127; frame <= 0; frame--){
      byte red = 127 - frame;
      byte grn = frame;
      for (int pxl = bayNum * 8 + 7; pxl >= bayNum * 8; pxl--){
        if (barPxl <= frame / 16){
          statusLED.setPixelColor(pxl,red,grn,0);
        }
        else{
          statusLED.setPixelColor(pxl,0);
        }
      statusLED.show();
      }
    barPxl--;
    delay (10);  
    }
  }
}
