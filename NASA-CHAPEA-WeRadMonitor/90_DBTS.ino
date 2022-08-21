void dbts(){

  bool somethingNew = false;

  for (int reg = 0; reg < numPISOregs; reg++){
    if (PISOdata [reg] != PISOprev[reg]){
      somethingNew = true;
    }
  }

  if (somethingNew){
    Serial.print(PISOdata[0],BIN);
    Serial.print("\t");
    Serial.print(PISOdata[1],BIN);
    Serial.print("\t");
    Serial.print("diaSet: ");
    Serial.print(dialSetting);
    Serial.print("\t");
    Serial.print("modSel: ");
    Serial.print(modeSelected);
    Serial.println();
  }
  
}
