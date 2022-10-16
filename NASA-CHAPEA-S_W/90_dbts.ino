void dbts(){
  for (int reg = 0; reg < numPISOregs; reg++){
    Serial.print ("Reg #");
    Serial.print (reg);
    Serial.print (": ");
    Serial.print (PISOdata[reg],BIN);
    if (reg < numPISOregs) Serial.print ("\t");
  }
  Serial.println();
}
