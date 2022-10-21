void cycleReset(){

  for (int reg = 0; reg < numPISOregs; reg++){
    PISOprev[reg] = PISOdata[reg];
  }

  inputOld = inputByte;

  driftScalePrev = driftScale;

  if (controlMode > 5) controlMode = 1;

  somethingNew = false;

}
