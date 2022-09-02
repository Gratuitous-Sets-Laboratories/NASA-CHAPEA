void readDial(){

  const int numOfSettings = 5;
  const int clicksPerSetting = 3;
  
  bool reData = digitalRead(reDat);
  if (reData) dialStt--;
  else dialStt++;

  procSetting = (dialStt/clicksPerSetting) % numOfSettings;

}
