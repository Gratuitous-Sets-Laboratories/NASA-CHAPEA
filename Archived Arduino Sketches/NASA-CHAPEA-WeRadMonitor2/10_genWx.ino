void genWx(){
  
  windNow = windPrev + (plusMinus() * windVar[controlMode]);
  windNow = constrain(windNow,windMin[controlMode],windMax[controlMode]);

  tempNow = tempPrev + (plusMinus() * tempVar[controlMode]);
  tempNow = constrain(tempNow,tempMin[controlMode],tempMax[controlMode]);

  presNow = presPrev + (plusMinus() * presVar[controlMode]);
  presNow = constrain(presNow,presMin[controlMode],presMax[controlMode]);

  radNow = radPrev + (plusMinus() * radVar[controlMode]);
  radNow = constrain(radNow,radMin[controlMode],radMax[controlMode]);
}

int plusMinus(){
  return random(-1,2);
}
