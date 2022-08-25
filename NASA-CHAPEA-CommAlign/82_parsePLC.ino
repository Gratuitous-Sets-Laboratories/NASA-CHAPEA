/*
 * 
 */

//

void parsePLC(){
  plcInByte = 0;
  for (int plcBit = 0; plcBit < 3; plcBit++){
    if (bitRead (PISOdata[1],plcBit) bitWrite(plcInbyte,plcBit,0);\
    else bitWrite(plcInByte,plcBit,1);
  }
}
