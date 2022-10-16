//============================================================//
//============== HOLD TO PRESS ===============================//
//============================================================//

void regHoldToToggle(int varName, int holdTime, int byteNum, int bitPos, bool activeHL){
  bool buttonHeld = false;
  int holdCount = 0;
  if (bitRead(PISOdata[byteNum],bitPos) == activeHL){
    buttonHeld = true; 
  }
  while (buttonHeld){
    holdCount++;
    delay(holdTime);
    if (bitRead(PISOdata[byteNum],bitPos) != activeHL){
      buttonHeld = false;
      break;
    }
    if (holdCount >= holdTime){
      varName++;
      break;
    }
  }
}

void pinHoldToToggle(int varName, int holdTime, int pinNum, bool activeHL){
  bool buttonHeld = false;
  int holdCount = 0;
  if (digitalRead(pinNum) == activeHL){
    buttonHeld = true; 
  }
  while (buttonHeld){
    holdCount++;
    delay(holdTime);
    if (digitalRead(pinNum) != activeHL){
      buttonHeld = false;
      break;
    }
    if (holdCount >= holdTime){
      varName++;
      break;
    }
  }
}
