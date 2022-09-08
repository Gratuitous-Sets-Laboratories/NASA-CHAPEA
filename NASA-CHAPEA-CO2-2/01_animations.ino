


void normalOpLEDs(){
//.............. Blank Status ................................// GRB format

    bool blinker = (millis()/500)%2;
    
    statusLED.setPixelColor(0,0);                             // assume active side is empty and make it dark
    statusLED.setPixelColor(1,0,200,0);                       // assume purge side is empty and make it red
    
//.............. Purge LED Status ............................//  
    if (purgeFilterNum){                                      // if the is a filter on the purge side...
      statusLED.setPixelColor(1,100,100,0);                   // assume it's dirty and set it to amber
      if (blinker){
        statusLED.setPixelColor(1,0);
      }
      if (filterStatus[purgeFilterNum-1] > 95){               // if the filter is clean (enough)...
        statusLED.setPixelColor(1,0,0,250);                   // set it to blue
      }
    }
//.............. Active LED Status ...........................// 
    if (activeFilterNum){                                     // if there is a filter on the active side...
      statusLED.setPixelColor(0,250,0,0);                     // assume it's clean and set it to green 
      if (filterStatus[activeFilterNum-1] < 50){              // if it's below 50...
        statusLED.setPixelColor(0,150,75,0);                  // amber up the color
      }
      if (filterStatus[activeFilterNum-1] < 25){              // if it's below 25...
        statusLED.setPixelColor(0,75,150,0);                  // orange it more
      }
      if (filterStatus[activeFilterNum-1] < 5){               // if it's below 5...
        statusLED.setPixelColor(0,0,250*blinker,0);           // set it to flash red
      }
    }
//............................................................//
    statusLED.show();
}

void startUpAnimation(){

  sendSIPO(1);
  pulsePin(latchPin,10);
  delay(1500);
  sendSIPO(0);
  pulsePin(latchPin,10);
}

void shutDownAnimation(){
  
}
