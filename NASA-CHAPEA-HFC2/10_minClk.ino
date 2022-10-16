void minClock(){
    if (millis() >= lastMinuteTick + 60000){
    minuteCounter++;
    somethingNew = true;
//    alarmReady = true;
    lastMinuteTick = millis();
  }
}
