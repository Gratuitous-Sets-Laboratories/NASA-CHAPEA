//==
//==
//==

void bayStatusIndicator(){
  
  for (int bay = 0; bay < 3; bay++){
    
    byte bottlePresent = HFCinPlace[bay];
    byte remainingCharge = bottleChargeLevel[bottlePresent];
    
    if (HFCinPlace[bay]){
      statusLED.setPixelColor(24+bay,150,150,0);
      if (remainingCharge == 0){
        statusLED.setPixelColor(24+bay,150,0,0);
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

  byte bottlePresent = HFCinPlace[bayNum];
  byte remainingCharge = bottleChargeLevel[bottlePresent];

  byte barPxl = 0;
  for (int pxl = bayNum * 8; pxl < bayNum * 8 + 8; pxl++){
    if (barPxl < remainingCharge / 32){
      statusLED.setPixelColor(pxl,0,100,0);
    }
    else{
      statusLED.setPixelColor(pxl,0);
    }
    barPxl++;
  }
  statusLED.show();
}
