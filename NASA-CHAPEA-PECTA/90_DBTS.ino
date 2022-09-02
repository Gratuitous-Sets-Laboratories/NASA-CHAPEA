void dbts(){

  if (txIsGo != txOld) somethingNew = true;
  if (rxIsGo != rxOld) somethingNew = true;

  if (somethingNew){
    Serial.print("PWR: ");
    Serial.print(powerOn);
    Serial.print("\t");
    Serial.print("Ant: ");
    Serial.print(txIsGo);
    Serial.print("\t");
    Serial.print("Aim: ");
    Serial.print(rxIsGo);
    Serial.println();
  }
  
}
