//============================================================//
//============== CYCLE REESET ================================//
//============================================================//

void cycleReset(){

  for (int r = 0; r < numPISOregs; r++){
    PISOprev[r] = PISOdata[r];
  }
  for (int e = 0; e < 2; e++){
    anCathPrev[e] = anCath[e]; 
  }

  somethingNew = false;
}
