void ersatzRotaryEncoder(){
  
  readPISO(0,0);
  bool reBtn = bitRead(PISOdata[0],3);
  
  while (!reBtn){
    bool reClk = bitRead((PISOdata[0],1);                        // check the rotary encoder clock pin
    bool reDat = bitRead((PISOdata[0],2);                        // check the rotary encoder data pin

    if (reClk && reDat){                                      // if bot bits are high (no signal)
      dialReady = true;                                       // the encoder is ready to read
    }
    if (dialReady && reClk || !reDat)){                        // if the encoder is ready and only the data bit is active... // ??!reClk??
      dialSetting++;                                          // incriment the dialSetting up one
      dialReady = false;                                      // the dial is no longer ready
      displayData = false;                                    // do not display data yet
    }
    else if (dialReady && !reClk && reDat){                   // otherwise, if the dial is ready and the clock bit is active...
      dialSetting--;                                          // decriment the dial setting by one
      dialReady = false;                                      // the dial is no longer ready
      displayData = false;                                    // do not display data yet
    }

    wxDataType = ((dialSetting/clicksPer) % numOfSettings)+1; // weather data type is derived from the dialSetting

    if      (wxDataType == 1) printText("WIND",2);            // didsplay the following data types based on the dial
    else if (wxDataType == 2) printText("TEMP",2);
    else if (wxDataType == 3) printText("PRES",2);
    else if (wxDataType == 4) printText(" RAD ",1);
    
    readPISO(0,0);
    bool reBtn = bitRead(PISOdata,3);
  }
}

/*
//.............. Ersatz Rotary Encoder .......................//

    bool reClk = bitRead(inputData,1);                        // check the rotary encoder clock pin
    bool reDat = bitRead(inputData,2);                        // check the rotary encoder data pin

    if (reClk && reDat){                                      // if bot bits are high (no signal)
      dialReady = true;                                       // the encoder is ready to read
    }
    if (dialReady && (!reClk || !reDat)){                        // if the encoder is ready and only the data bit is active... // ??!reClk??
      dialSetting++;                                          // incriment the dialSetting up one
      dialReady = false;                                      // the dial is no longer ready
      displayData = false;                                    // do not display data yet
    }

    else if (dialReady && !reClk && reDat){                   // otherwise, if the dial is ready and the clock bit is active...
      dialSetting--;                                          // decriment the dial setting by one
      dialReady = false;                                      // the dial is no longer ready
      displayData = false;                                    // do not display data yet
    }

    wxDataType = ((dialSetting/clicksPer) % numOfSettings)+1; // weather data type is derived from the dialSetting 
*/
