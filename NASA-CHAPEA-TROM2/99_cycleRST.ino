void cycleReset(){
  
  if (somethingNew) newInputTimeStamp = millis();
  btnOld = btnStt;
  somethingNew = false;
}
