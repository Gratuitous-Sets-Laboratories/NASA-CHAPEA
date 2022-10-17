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
  int reg = 0;
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

//============== ROLL A D6 ===================================//
/*
 * This function will return a boolean T/F based on paramaters:
 * dif = minimum threshhold of success
 * die = the size of the die being rolled
 */
bool blinkOrNo(int dif, int die){
  long roll = random(1,die+1);                                // generate a (pseudo)random value between 1 and the die size
  if (roll >= dif) return true;                               // if the result is at or above the target, return a true
  else return false;                                          // otherwise return a false
}
