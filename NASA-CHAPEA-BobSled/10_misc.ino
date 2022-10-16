//============================================================//
//==============  ================================//
//============================================================//

void sendSIPO(){
  bool bitVal;
  for (int b = 0; b < 8; b++){
    bitVal = bitRead(plcMISO,b);
    pulsePin(clockPin);
  }
  for (int b = 0; b < 8; b++){
    bitVal = bitRead(lightByte,b);
    digitalWrite(dataOutPin,bitVal);
    pulsePin(clockPin);
  }
  pulsePin(latchPin);
}

//------------------------------------------------------------//

bool pressAndHold(int btnNum, int holdTime){
  int reg = 1;
  if (!bitRead(PISOdata[reg],btnNum) && bitRead(PISOprev[reg],btnNum)){
    pressTime = millis();
    Serial.print("Button ");
    Serial.print(btnNum);
    Serial.println(" pressed.");
  }
  if (!bitRead(PISOdata[reg],btnNum) && millis() >= pressTime + holdTime){
    Serial.print("Button ");
    Serial.print(btnNum);
    Serial.println(" held.");
    heldTime = millis();
    return true;
  }
  else return false;
}

//------------------------------------------------------------//

void blinkLED(int LED){
  int interval = 250;
  if ((millis()/interval)%2){
    bitWrite(lightByte,LED,1);
  }
  else{
    bitWrite(lightByte,LED,0);
  }
}
