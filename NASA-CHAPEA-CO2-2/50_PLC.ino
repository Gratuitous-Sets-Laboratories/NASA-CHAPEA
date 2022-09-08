//============================================================//
//============== NASA CHAPEA PLC ADAPTER =====================//
//============================================================//

//-------------- PARSE PLC MOSI DATA -------------------------//

/*
 * This function will take the indicated PISO register input
 * and convert it to a 3 bit value
 * from the active low of the PLC
 */

byte plcSignal(int regNum){
  byte returnValue = 0;
  for (int bitPos = 0; bitPos < 3; bitPos++){
    if (bitRead(PISOdata[regNum],bitPos) == 0){
      bitWrite(returnValue,bitPos,1);
    }
  }
  return returnValue;
}
