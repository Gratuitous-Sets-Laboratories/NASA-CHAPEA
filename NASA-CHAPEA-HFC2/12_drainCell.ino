void drainCell(){

  int drainTime[7] = {12,9,30,1,1,1,1};              // number of minutes to drain one cell increment (0-255)

  int tickDrainNum = drainTime[controlMode-1];
  
  for (int bay = 0; bay < 3; bay++){                            // for each bay...
    byte bottlePresent = HFCinPlace[bay];                       // note which bottle is in place
    if (bay == activeCell-1){                                   // if that bay is the active one
      if (minuteCounter >= lastDischargeTime + tickDrainNum){   // if the minute counter has passed its last discharge plus delay
        if (bottleChargeLevel[bottlePresent]){                  // it it's not already dead
          bottleChargeLevel[bottlePresent]--;                   // decrement its charge by one
        }
        lastDischargeTime = minuteCounter;                      // make a note of the last discharge time
      }
    }
  }
}

void magicRecharge(){

  int magicChargeDelay = 1440;                                  // time to magically refil a bottle
  
  for (int bay = 0; bay < 3; bay++){                            // for each bay...
    for (int bot = 1; bot <= 7; bot++){                          // and each bottle...
      if (HFCinPlace[bay] == bot){                              // if the bay contains that bottle...
        bottleLastInUse[bot] = minuteCounter;                   // note the minute counter it was there
      }
    }
  }
  for (int bot = 0; bot < 7; bot++){                            // for each bottle...
    if (minuteCounter >= bottleLastInUse[bot] + magicChargeDelay){    // if it's been long enough...
      bottleChargeLevel[bot] = 255;                                 // magically top off that bottle
    }
  }
  bottleChargeLevel[0] = 0;
}

void drainFirstCells(){

  for (int bay = 0; bay < 3; bay++){                                // for each bay...
    byte bottlePresent = HFCinPlace[bay];                       // note which bottle is in place
    bottleChargeLevel[bottlePresent] = 5;                       // KILL IT!
  }
}
