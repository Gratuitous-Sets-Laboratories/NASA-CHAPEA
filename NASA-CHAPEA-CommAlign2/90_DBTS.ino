void dbts(){

  if (somethingNew){
    Serial.print (PISOdata[0],BIN);
    Serial.print ("\t");
    Serial.print (PISOdata[1],BIN);
    Serial.print ("\t");
    Serial.print ("ctrlMod: ");
    Serial.print (controlMode);
    Serial.print ("\t");
    Serial.print ("acc: ");
    Serial.print (accuracy);
    Serial.println();
  }
}
