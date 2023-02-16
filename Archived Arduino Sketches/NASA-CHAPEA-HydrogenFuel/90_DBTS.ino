void dbts(){

  bool somethingNew = false;

  for (int reg = 0; reg < numPISOregs; reg++){
    if (PISOdata[reg] != PISOprev[reg]){
      somethingNew = true;
    }
  }
  for (int bay = 0; bay < 3; bay++){
    if (HFCinPlace[bay] != HFCinPrev[bay]){
      somethingNew = true;
    }
    if (buttonPress[bay] != buttonPrev[bay]){
      somethingNew = true;
    }
  }

  if (somethingNew){
    Serial.print(PISOdata[0],BIN);
    Serial.print("\t");
    Serial.print("ctrlMd: ");
    Serial.print(controlMode);
    Serial.print("\t");
    Serial.print("Btl# & Chrg: ");
    for (int bay = 0; bay < 3; bay++){
      Serial.print(HFCinPlace[bay]);
      Serial.print(" ");
      Serial.print(bottleChargeLevel[HFCinPlace[bay]]);
      Serial.print(" | ");
    }
    Serial.print("\t");
    Serial.print("Btns: ");
    for (int bay = 0; bay < 3; bay++){
      Serial.print(buttonPress[bay]);
      Serial.print(" ");
    }
    Serial.println();
    
  }
  
}
