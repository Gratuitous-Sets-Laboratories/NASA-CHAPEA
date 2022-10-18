void genWx(){
//                    1      2     3     4     5     6 
//                  Norm  Wind  Storm   +Rad  !Rad  Fail
  float windMin[6] = {0,    30,   50,    25,   40,   0};
  float windMax[6] = {40,   60,   100,   50,   60,   40};
  int   windVar[6] = {1,    5,    10,    2,    3,    1};
  float windInc = .1;

  float tempMin[6] = {-70, -75,  -80,   -75,  -80,  -10};
  float tempMax[6] = {-60, -65,  -70,   -65,  -70,   50};
  int   tempVar[6] = { 10,  20,   30,    50,   50,   200};
  float tempInc = .1;

  int   presMin[6] = {700,  725,  750,  725,  750,  100};
  int   presMax[6] = {900,  925,  950,  925,  950,  250};
  int   presVar[6] = {2,    3,    5,    3,    5,    9};
  int   presInc = 1;

  float radMin[6] =  {1,    2,    3,    5,    6,    7};
  float radMax[6] =  {1,    2,    3,    5,    6,    7};
  float radVar[6] =  {1,    2,    3,    5,    6,    7};
  int   radInc = .1;
  float deltaData;
  
  deltaData = random(windVar[controlMode-1]*-1, windVar[controlMode-1]+1) * windInc;
  windNow += deltaData;
  windNow = constrain(windNow,windMin[controlMode-1],windMax[controlMode-1]);

  deltaData = random(tempVar[controlMode-1]*-1, tempVar[controlMode-1]+1) * tempInc;
  tempNow += deltaData;
  tempNow = constrain(tempNow,tempMin[controlMode-1],tempMax[controlMode-1]);
  tempNow += timeTempMod;

  deltaData = random(presVar[controlMode-1]*-1, presVar[controlMode-1]+1) * presInc;
  presNow += deltaData;
  presNow = constrain(presNow,presMin[controlMode-1],presMax[controlMode-1]);

  deltaData = random(radVar[controlMode-1]*-1, radVar[controlMode-1]+1) * radInc;
  radNow += deltaData;
  radNow = constrain(radNow,radMin[controlMode-1],radMax[controlMode-1]);
/*
  presNow = presPrev + (plusMinus() * presVar[controlMode-1]);
  presNow = constrain(presNow,presMin[controlMode-1],presMax[controlMode-1]);

  radNow = radPrev + (plusMinus() * radVar[controlMode]);
  radNow = constrain(radNow,radMin[controlMode-1],radMax[controlMode-1]);
*/
}
/*
int plusMinus(){
  int value = random(-1,2);
  return value;
}
*/
