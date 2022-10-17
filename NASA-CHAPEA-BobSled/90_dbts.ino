//============================================================//
//============== DEBUG & TROUBLESHOOT ========================//
//============================================================//

void dbts(){
  if (somethingNew){
    printPISOdata();
    printAnCath();
    Serial.print("ctrlMode: ");
    Serial.print(controlMode);
    Serial.print("\tprocStep: ");
    Serial.print(processStep);
    Serial.print("\tfillProg: ");
    Serial.print(fillProgress);
    Serial.println();
    Serial.println();
  }
}

void printPISOdata(){
  for (int r = 0; r < numPISOregs; r++){
   Serial.print("Reg #");
   Serial.print(r);
   Serial.print(": ");
   for (int b = 7; b >=0; b--){
    bool bitVal = bitRead(PISOdata[r],b);
    Serial.print(bitVal);
   }
   Serial.println();
  }
}

void printAnCath(){
  Serial.print("Anode = ");
  Serial.print(anCath[0]);
  Serial.print(" | Cathode = ");
  Serial.println(anCath[1]);
}
