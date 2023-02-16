void parsePLC(){
  if (plcInByte == 1){
    updateControl();
  }
  else if (plcInByte == 2){
    driftScale = 0;
  }
  
  else if (plcInByte >= 3 && plcInByte <= 5){
    driftScale = plcInByte - 2;
  }

  else if (plcInByte == 6){
    commFail = true;
  }

  else if (plcInByte == 7){
    commFail = false;
  }
}
