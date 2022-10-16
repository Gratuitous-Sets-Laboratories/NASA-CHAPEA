void cycleReset(){
  for (int reg = 0; reg < numPISOregs; reg++){
    PISOprev[reg] = PISOdata[reg];
  }
  somethingNew = false;
}
