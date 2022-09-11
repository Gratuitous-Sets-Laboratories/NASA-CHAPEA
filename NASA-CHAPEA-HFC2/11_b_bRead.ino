void bottleRead(){

  for (int bay = 0; bay < 3; bay++){
    
    int target[8] = {0,237,319,409,511,609,696,838};            // calculated analog read values for each key's chosen resistor
    int range = 10;
    int rawRead = analogRead(hfcPin[bay]);

    HFCinPlace[bay] = 0;

     for (int bot = 1; bot < 8; bot ++) {                      // for each of 7 possible keys...
      if (rawRead >= target[bot]-range                          // if the read is at or above the low end...
      && rawRead <= target[bot]+range) {                        // and it is at or below the high end...
        HFCinPlace[bay] = bot;                                          // that is the key number
      }
    } 
  }
}

void buttonRead(){
  for (int bay = 0; bay < 3; bay++){
    buttonPress[bay] = false;
    if (!digitalRead(startButton[bay])){
      buttonPress[bay] = true;
    }
  }
}
