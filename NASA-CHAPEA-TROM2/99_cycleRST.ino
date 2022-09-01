void cycleReset(){
  
  if (somethingNew) newInputTimeStamp = millis();
  dialOld = dialStt;
  btnOld = btnStt;
  somethingNew = false;
}
