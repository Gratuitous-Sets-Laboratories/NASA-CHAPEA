void cycleReset(){

  PISOprev[0] = PISOdata[0];
  activeFilterPrev = activeFilterNum;
  purgeFilterPrev = purgeFilterNum;
  powerButtonPrev = powerButtonState;
  for (int filt = 0; filt < 2; filt++){
    filterPrev[filt] = filterStatus[filt];
  }
  redButtonPrev = redButtonState; 

  somethingNew = false;
}