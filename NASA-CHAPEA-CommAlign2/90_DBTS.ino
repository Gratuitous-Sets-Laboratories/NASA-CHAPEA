void dbts(){

  bool somethingNew = false;

  for (int reg = 0; reg < numPISOregs; reg++){
    if (PISOdata[reg] != PISOprev[reg]){
      somethingNew = true;
    }
  }

  if (somethingNew){
    Serial.print (PISOdata[0],BIN);
    Serial.print ("\t");
    Serial.print (PISOdata[1],BIN);
    Serial.print ("\t");
    Serial.print ("cmdMod: ");
    Serial.print (commandMode);
    Serial.print ("\t");
    Serial.print ("acc: ");
    Serial.print (accuracy);
    Serial.println();
  }
}
