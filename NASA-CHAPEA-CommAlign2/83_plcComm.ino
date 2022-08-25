byte parsePLC(int regNum){
  byte returnValue = 0;
  for (int bitPos = 0; bitPos < 3; bitPos++){
    if (bitRead(PISOdata[regNum],bitPos)){
      bitWrite(returnValue,bitPos,0);
    }
    else{
      bitWrite(returnValue,bitPos,1);
    }
  }
//  bool bitVal1 = bitRead(returnValue,0);
//  bool bitVal3 = bitRead(returnValue,2);
//  bitWrite(returnValue,0,bitVal3);
//  bitWrite(returnValue,2,bitVal1);
  
  return returnValue;
}
