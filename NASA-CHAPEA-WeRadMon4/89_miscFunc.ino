/*
 * Press and hold
 */

 bool pressAndHold(int button, int duration, bool highLow){

  bool returnVal = false;
  uint32_t pressTime;

  if (digitalRead(button) == highLow){
    pressTime = millis();
  }

  while (digitalRead(button) == highLow){
    if (millis() >= pressTime + duration){
      returnVal = true;
      somethingNew = true;
      break;
    }
  }
  return returnVal;
 }
