void dbts(){

  if (somethingNew){
 //   Serial.println(PISOdata[0]);
    Serial.print("PWR ");
    Serial.print(powerMode);
    Serial.print(" - PLC ");
    Serial.print(plcSignal(0));
    Serial.print(" - CM ");
    Serial.print(controlMode);
    Serial.print("\t");
    
    for (int filt = 0; filt < 2; filt++){
      Serial.print ("F");
      Serial.print (filt+1);
      Serial.print (": ");
      Serial.print (filterStatus[filt]);
      Serial.print (" ");
    }
    Serial.print("\t");
    
    Serial.print("ActFilt ");
    Serial.print(activeFilterNum);
    Serial.print("\t");
    Serial.print("PrgFilt ");
    Serial.print(purgeFilterNum);
    Serial.print("\t");
    Serial.print("PWR btn ");
    Serial.print(powerButtonState);
    Serial.print("\t");
    Serial.print("RED btn ");
    Serial.print(redButtonState);
    Serial.println();
  }
  
}
