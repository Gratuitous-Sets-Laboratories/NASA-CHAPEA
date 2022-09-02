//============================================================//
//============== SHIFT REGISTER I/O ==========================//
//============================================================//

/* These functions will read and write via PISO & SIOP shift registers.
 * They require the global variable byte arrays of:
 * byte PISOdata[numPISOregs];
 * 
 * Devices use 74HC165 PISO registers and 74HC595 SIPO registers.
 * 
 * This section updated  Aug 2022
 */

//-------------- RISING EDGE PUSLE ---------------------------//

/* This function will send a rising edge signal to the designated output pin.
 * Paramaters required are:
 * pinName (loadPin, latchPin, clockPin)
 * pulseTime (given in microseconds)
 */

void pulsePin(int pinName, int pulseTime){
  digitalWrite(pinName, LOW);
  delayMicroseconds(pulseTime);
  digitalWrite(pinName, HIGH);
  delayMicroseconds(pulseTime);
}

//-------------- WRITE TO SIPO REGISTERS ---------------------//

/* This function will load a byte of data into a chain of SIPO shift registers.
 * The sr_byte parameter is the bye of data being sent.
 * 
 * Note that this function should be called seperately for each register in the chain,
 * beginning with the furthest register from the Arduino.
 * 
 * After each register has been updated, use the command
 * pulsePin(latchPin,10);
 * in order to activate the chain of registers.
 * 
 */
void sendSIPO(byte sr_data){

  for (int pos=0; pos<8; pos++){                              // for each bit in the byte...
    bool bitVal = bitRead(sr_data, pos);                      // note its value from the paramater
    digitalWrite(dataOutPin, bitVal);                         // write it to the data pin
    pulsePin(clockPin,10);                                    // pulse the clock pin
  }
}
