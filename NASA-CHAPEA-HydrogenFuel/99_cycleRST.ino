void cycleReset(){

  for (int reg = 0; reg < numPISOregs; reg++){
    PISOprev[reg] = PISOdata[reg];
  }
  for (int bay = 0; bay < 3; bay++){
    HFCinPrev[bay] = HFCinPlace[bay];
    buttonPrev[bay] = buttonPress[bay];
  }

}
