void cycleReset(){
  
  if (somethingNew) newInputTimeStamp = millis();
  
  hatchOld = hatchStt;
  leverOld = leverStt;
  execOld = execStt;

  somethingNew = false;
}
