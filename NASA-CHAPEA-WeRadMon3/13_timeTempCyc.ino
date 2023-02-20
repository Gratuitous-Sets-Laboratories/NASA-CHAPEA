//==

void timeTempCycle(){
  minutesIntoTempCycle++;
  if (minutesIntoTempCycle > 720){
    minutesIntoTempCycle = 0;
    if (risingTempCycle) risingTempCycle = false;
    else risingTempCycle = true;
  }
}

/*
void hourClock(){
  if (controlMode == 7){
    hour = 12;
    hourTick = millis();
  }
  if (millis() >= (hourTick + (1000*60*60))){
    hour++;
    if (hour == 12){
      sendSIPO(7);
      pulsePin(latchPin);
      delay(1500);
      sendSIPO(0);
      pulsePin(latchPin);
    }
    hourTick = millis();
  }
  if (hour > 23){
    hour = 0;
  }
  if (hour < 6){
    timeTempMod = -4*(hour);
  }
  else if (hour >= 6 && hour < 18){
    timeTempMod = 4*(hour-12);
  }
  else {
    timeTempMod = 4*(24-hour);
  }
}
*/
