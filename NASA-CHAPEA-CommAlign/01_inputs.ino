void readRegisters(){
  
  pulsePin(loadPin,10);                                             // send a rising edge signal to load current input data into register memory
  
  
    for (int pos=0; pos<8; pos++){                            // and for each bit in that byte...
      bool val = digitalRead(dataInPin);                      // read the dataInPin
      bitWrite(inputByte, pos, val);                       // write that bit to its position in its byte
      pulsePin(clockPin,10);                                        // pulse the clock pin to load the next bit into position
    }

    for (int pos=0; pos<8; pos++){                            // and for each bit in that byte...
      bool val = digitalRead(dataInPin);                      // read the dataInPin
      bitWrite(plcInByte, pos, val);                       // write that bit to its position in its byte
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
