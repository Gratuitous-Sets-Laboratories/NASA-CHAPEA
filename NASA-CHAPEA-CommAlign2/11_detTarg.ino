//============================================================//
//============== DETERMINE TARGET ============================//
//============================================================//

void determineTarget(){

//.............. Drift Variables .............................//

  const int decayMin[4] = {0,45,10,1};    // min time in minutes for an acc dift
  const int decayMax[4] = {0,75,20,3};    // max time in minutes for an acc dift

//.............. (Re)Set Target ..............................//

  if (!targetX || controlMode == 6){    // if there is no target, or control resets the target...
    targetX = random(1,33);             // generate a random X value
    targetY = random(1,8);              // and Y value
  }

//.............. Schedule Drift ..............................//

  int driftDelay;

  if (controlMode <= 4){                                      // if the PLC is communicating drift 
    driftScale = controlMode - 1;                             // set the drift scale (0-3)
  }
  
  if (driftScale &&                                           // if there is an active drift...
  (!nextDrift || (driftScale != driftScalePrev))){            // and there is no scheduled drift, or there has been a chnage from PLC
    driftDelay = random(decayMin[driftScale],decayMax[driftScale]);  // set a drift delay in minutes within the above parameters
    nextDrift = millis() + (driftDelay * 60000);              // schedule the drift for the current time plus the determined delay
  }

//.............. Execute Drift ...............................//

  if (millis() >= nextDrift){                                 // if the clock has passed the scheculed drift time...        

    nextDrift = millis() + (driftDelay * 60000);              // schedule athe next drift

    int xDrift = random(-1,2);                                // chose a value from -1 to +1
    targetX += xDrift;                                        // add that to the targetX
    if (targetX < 1) targetX = random(2,4);                   // if that puts it too far left, push it right
    else if (targetX > 32) targetX = random (29,31);          // or if it's too far right, push it to the left
  
    int yDrift = random(-1,2);                                // chose a value from -1 to +1
    targetY += yDrift;                                        // add that to the targetY
    if (targetY < 0) targetY = random (1,2);                  // keep the target above the horizon 
    else if (targetY > 7) targetY = random (5,6);             // keep the target from going too high
  }
}
