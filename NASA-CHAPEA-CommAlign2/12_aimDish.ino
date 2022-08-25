void aimDish(){

  if (inputByte % 16 != B1111){

    bool up = bitRead(inputByte,0);
    bool dn = bitRead(inputByte,1);
    bool lf = bitRead(inputByte,2);
    bool rt = bitRead(inputByte,3);

//    Serial.print(up);
//    Serial.print(dn);
//    Serial.print(lf);
//    Serial.println(rt);


    if ((inputOld % 16 == B1111)
    || (millis() >= inputTick + 50)){
      inputTick = millis();
      
      if (!rt) lockedX--;
      else if (!lf) lockedX++;
      lockedX = constrain(lockedX,1,32);
      
      if (!up) lockedY++;
      else if (!dn) lockedY--;
      lockedY = constrain(lockedY,0,7);
    }
  }
  int xErr = abs(lockedX-targetX);
  int yErr = abs(lockedY-targetY);
  accuracy = 100 - (5 * xErr) - (8 * yErr);
  accuracy = constrain (accuracy,0,100);
}
