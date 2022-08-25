void dbts(){
  
  bool somethingNew = false;


  if (PISOdata != PISOprev) somethingNew = true;
  if (activeFilterPrev != activeFilterNum) somethingNew = true;
  if (purgeFilterPrev != purgeFilterNum) somethingNew = true;
  if (powerButtonPrev != powerButtonState) somethingNew = true;
  if (redButtonPrev != redButtonState) somethingNew = true; 

  if (somethingNew){
    Serial.print(PISOdata,BIN);
    Serial.print("\t");
    Serial.print(activeFilterNum);
    Serial.print("\t");
    Serial.print(purgeFilterNum);
    Serial.print("\t");
    Serial.print(powerButtonState);
    Serial.print("\t");
    Serial.print(redButtonState);
    Serial.println();
  }
  
}
