void genWx(){
//                    1      2     3     4     5     6     7
//                  Norm  Wind  Storm   Hot   +Rad  !Rad  Fail
  int windMin[7] = {0,    30,   50,    0,    30,   5,    0};
  int windMax[7] = {40,   60,   100,   40,   60,   35,   40};
  int windVar[7] = {1,    5,    10,    1,    1,    1,    1};
  int windInc = 1;

  int tempMin[7] = {-70,  -70,   -80,   -50,  -50,  -40,  0};
  int tempMax[7] = {-50,  -50,   -60,   -30,  -30,  -30,  60};
  int tempVar[7] = {1,    1,     1,     1,    1,    1,    4};
  int tempInc = 1;

  int presMin[7] = {700,  800,  900,  750,  700,   700,  50};
  int presMax[7] = {800,  900,  950,  850,  800,   800,  150};
  int presVar[7] = {1,    2,    5,    2,    1,     1,    7};
  int presInc = 1;

  int radMin[7] =  {180,  195,  195,  210,  250,  300,  7900};
  int radMax[7] =  {270,  210,  210,  225,  275,  350,  8350};
  int radVar[7] =  {1,    1,    1,    1,    2,    3,    1};
  int radInc = 1;
  
  int deltaData;
  
  deltaData = random(windVar[controlMode-1]*-1, windVar[controlMode-1]+1) * windInc;
  windNow += deltaData;
  windNow = constrain(windNow,windMin[controlMode-1],windMax[controlMode-1]);

  deltaData = random(tempVar[controlMode-1]*-1, tempVar[controlMode-1]+1) * tempInc;
  tempNow += deltaData;
  tempNow = constrain(tempNow,tempMin[controlMode-1],tempMax[controlMode-1]);
  tempNow += timeTempMod();

  deltaData = random(presVar[controlMode-1]*-1, presVar[controlMode-1]+1) * presInc;
  presNow += deltaData;
  presNow = constrain(presNow,presMin[controlMode-1],presMax[controlMode-1]);

  deltaData = random(radVar[controlMode-1]*-1, radVar[controlMode-1]+1) * radInc;
  radNow += deltaData;
  radNow = constrain(radNow,radMin[controlMode-1],radMax[controlMode-1]);

}

int timeTempMod(){
  if (risingTempCycle){
    return (minutesIntoTempCycle - 360) / 10;
  }
  else{
    return (360 - minutesIntoTempCycle) / 10;
  }
}
