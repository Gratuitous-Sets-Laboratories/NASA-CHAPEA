void showMeTheDigits(){

  float dataNow;
  byte unitMarker;
  
  if      (wxDataType == 1){
    dataNow = windNow;
    unitMarker = 3;
  }
  else if (wxDataType == 2){
    dataNow = tempNow;
    unitMarker = 12;
  }
  else if (wxDataType == 3){
    dataNow = presNow;
    unitMarker = 48;
  }
  else if (wxDataType == 4){
    dataNow = radNow;
    unitMarker = 192;
  }
  
   char printDig[6];
   String printNum;
   printNum=String(dataNow);
   printNum.toCharArray(printDig,6);
        
   printText(printDig,1);
   grid.setColumn(22,unitMarker);
   grid.setColumn(23,unitMarker);
      
}
