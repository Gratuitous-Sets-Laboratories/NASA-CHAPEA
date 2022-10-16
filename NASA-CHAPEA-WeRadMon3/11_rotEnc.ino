
 
void ersatzRotaryEncoder(){

  const int numOfSettings = 4;
  const int clicksPer = 2;

  displayData = false;

    

    readPISO(0,0);
    
    bool reClk = bitRead(PISOdata[0],1);                      // check the rotary encoder clock pin
    bool reDat = bitRead(PISOdata[0],2);                      // check the rotary encoder data pin

    if (reClk && reDat){                                      // if bot bits are high (no signal)
      dialReady = true;                                       // the encoder is ready to read
    }
    if (dialReady && reClk && !reDat){                       // if the encoder is ready and only the data bit is active... // ??!reClk??
      dialSetting++;                                          // incriment the dialSetting up one
      grid.clear();
      dialReady = false;                                      // the dial is no longer ready
    }
    else if (dialReady && !reClk && reDat){                   // otherwise, if the dial is ready and the clock bit is active...
      dialSetting--;                                          // decriment the dial setting by one
      grid.clear();
      dialReady = false;                                      // the dial is no longer ready
    }

    wxDataType = ((dialSetting/clicksPer) % numOfSettings)+1; // weather data type is derived from the dialSetting

    if      (wxDataType == 1) printText("WIND",2);            // didsplay the following data types based on the dial
    else if (wxDataType == 2) printText("TEMP",1);
    else if (wxDataType == 3) printText("PRES",2);
    else if (wxDataType == 4) printText(" RAD ",1);
  
}
