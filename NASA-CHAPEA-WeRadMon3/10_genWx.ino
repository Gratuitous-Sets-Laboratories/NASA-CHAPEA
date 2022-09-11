void genWx(){
//                    1      2     3     4     5     6     7
//                  Norm  Wind  Storm   Hot   +Rad  !Rad  Fail
  float windMin[7] = {0,    30,   50,    0,    30,   5,    0};
  float windMax[7] = {40,   60,   100,   40,   60,   35,   40};
  int   windVar[7] = {1,    5,    10,    1,    1,    1,    1};
  float windInc = .5;

  float tempMin[7] = {-85, -92,   3,    4,    5,    6,    7};
  float tempMax[7] = {-75, -85,   3,    4,    5,    6,    7};
  int   tempVar[7] = {1,   2,     3,    5,    5,    5,    20};
  float tempInc = .1;

  float presMin[7] = {1,    2,    3,    4,    5,    6,    7};
  float presMax[7] = {1,    2,    3,    4,    5,    6,    7};
  int   presVar[7] = {1,    2,    3,    4,    5,    6,    7};
  float presInc = .1;

  float radMin[7] =  {1,    2,    3,    4,    5,    6,    7};
  float radMax[7] =  {1,    2,    3,    4,    5,    6,    7};
  float radVar[7] =  {1,    2,    3,    4,    5,    6,    7};
  int   radInc = .1;
  float deltaData;
  
  deltaData = random(windVar[controlMode-1]*-1, windVar[controlMode-1]+1) * windInc;
  windNow += deltaData;
  windNow = constrain(windNow,windMin[controlMode-1],windMax[controlMode-1]);

  deltaData = random(tempVar[controlMode-1]*-1, tempVar[controlMode-1]+1) * tempInc;
  tempNow += deltaData;
  tempNow = constrain(tempNow,tempMin[controlMode-1],tempMax[controlMode-1]);
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
