void updateControl(){
  
  for (int pos=0; pos<8; pos++){
    bool bitVal = bitRead(plcOutByte, pos);
    digitalWrite(dataOutPin, bitVal);
    pulsePin(clockPin,10);
  }
  pulsePin(latchPin,10);
}
