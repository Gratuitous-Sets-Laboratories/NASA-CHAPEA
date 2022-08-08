void readRegisters(){
  
  pulsePin(loadPin,10);                                             // send a rising edge signal to load current input data into register memory
  
  
    for (int pos=0; pos<8; pos++){                            // and for each bit in that byte...
      bool val = digitalRead(dataInPin);                      // read the dataInPin
      bitWrite(inputByte, pos, val);                       // write that bit to its position in its byte
      pulsePin(clockPin,10);                                        // pulse the clock pin to load the next bit into position
    }
}
  
//==
/*
 * Rising edge
 */
void pulsePin(int pinName, int pulseTime){
  digitalWrite(pinName, LOW);
  delayMicroseconds(pulseTime);
  digitalWrite(pinName, HIGH);
  delayMicroseconds(pulseTime);
}

//==

void sendLightByte(byte lightByte){
//  for (int dump = 0; dump < 8; dump++){
//    pulsePin(clockPin,10);
//  }
  for (int pos=0; pos<8; pos++){
    bool bitVal = bitRead(lightByte, pos);
    digitalWrite(dataOutPin, bitVal);
    pulsePin(clockPin,10);
  }
  pulsePin(latchPin,10);
}
