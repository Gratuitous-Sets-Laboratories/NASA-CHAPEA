//============================================================//
//============== NASA CHAPEA PLC ADAPTER =====================//
//============================================================//

//-------------- PARSE PLC MOSI DATA -------------------------//

/*
 * This function will take the indicated PISO register input
 * and convert it to a 3 bit value
 * from the active LOW of the PLC
 */

byte plcSignal(int regNum){
  byte returnValue = 0;
  for (int bitPos = 0; bitPos < 3; bitPos++){
    if (bitRead(PISOdata[regNum],bitPos) == 0){
      bitWrite(returnValue,2-bitPos,1);
    }
  }
  return returnValue;
}

//-------------- CHECK FOR PLC MASTER ------------------------//

/*
 * This function will check for a signal from the PLC from the requested byte
 * and update the controlMode global variable
 */
void updateControlMode(int regNum){
  if (plcSignal[regNum]){
    delay(250);
    readPISO(regNum,regNum);
    byte doubleCheck = plcSignal(regNum);
    delay(250);
    readPISO(regNum,regNum);
    if (plcSignal(regNum) && plcSignal(regNum) == doubleCheck){
      controlMode = doubleCheck;
      genWx();
    }
  }
}

//-------------- SEND PLC MISO DATA --------------------------//

void sendToPLC(byte sr_data, int signalLength){
  sendSIPO(sr_data);
  pulsePin(latchPin);
  delay(signalLength);
  sendSIPO(0);
  pulsePin(latchPin);
}

//------------------------------------------------------------//
