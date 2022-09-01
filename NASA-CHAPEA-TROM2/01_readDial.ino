void readDial(){
  bool reData = digitalRead(reDat);
  if (reData) dialStt--;
  else dialStt++;

  setting = (dialStt/clicksPerSetting) % numOfSettings;

}
