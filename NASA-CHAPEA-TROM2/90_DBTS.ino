void dbts(){

  if (somethingNew){
    Serial.print("Hat: ");
    Serial.print(hatchStt);
    Serial.print(" | ");
    Serial.print("Btn: ");
    Serial.print(leverStt);
    Serial.print(" | ");
    Serial.print("Lvr: ");
    Serial.print(leverStt);
    Serial.print("\t| ");
    Serial.print("TROM status: ");
    Serial.print(tromStatus);
    Serial.print(" | ");
    Serial.print("Selected Type");
    Serial.println(settingName[procSetting]);
  }
}
