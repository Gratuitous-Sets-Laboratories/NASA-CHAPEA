void dbts(){



  if (somethingNew){
    Serial.print(PISOdata[0],BIN);
    Serial.print("\t");
//    Serial.print(PISOdata[1],BIN);
//    Serial.print("\t");
    Serial.print("PLC Sig: ");
    Serial.print(plcSignal(0));
    Serial.print("\t");
    Serial.print("modSel: ");
    Serial.print(wxDataType);
    Serial.print("\t");
    Serial.print("Wind: ");
    Serial.print(windNow);
    Serial.print("\t");
    Serial.print("Temp: ");
    Serial.print(tempNow);
    Serial.print("\t");
    Serial.print("Pres: ");
    Serial.print(presNow);
    Serial.print("\t");
    Serial.print("Rad: ");
    Serial.print(radNow);
    Serial.print("\t");
    Serial.print("weaSit: ");
    Serial.print(controlMode);
    Serial.print("\t");
    Serial.print("ttm: ");
    Serial.print(timeTempMod());
    Serial.println();
    
  }
  
}