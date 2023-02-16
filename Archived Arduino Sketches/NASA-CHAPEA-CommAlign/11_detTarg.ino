void determineTarget(){
  
  if (!targetX){
    targetX = random(1,33);
    targetY = random(1,8);
  }

  int decayMin[3] = {45,10,1};
  int decayMax[3] = {75,20,3};

  int driftDelay = random(decayMin,decayMax);

  if (millis() >= driftTick + (driftDelay * 60000)){

    int xDrift = random(-1,1);
    targetX += xDrift;
    if (targetX < 1) targetX = random(2,4);
    else if (targetX > 32) targetX = random (29,31);
  
    int yDrift = random(-1,1);
    targetY += yDrift;
    if (targetY < 0) targetY = random (1,2);
    else if (targetY > 7) targetY = random (5,6);
      
    driftTick = millis();
  }
  
}
