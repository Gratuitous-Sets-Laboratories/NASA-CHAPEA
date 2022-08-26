void dbts(){

  bool somethingNew = false;

  for (int reg = 0; reg < numPISOregs; reg++){
    if (PISOdata[reg] != PISOprev[reg]){
      somethingNew = true;
    }
  }

  if (somethingNew){
    Serial.print(PISOdata[0],BIN);
    Serial.print("\t");
    Serial.print("ctrlMd: ");
    Serial.print(controlMode);
    Serial.println();
    
  }
  
}
