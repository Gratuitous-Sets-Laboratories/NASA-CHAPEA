/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA TROM (Trash Recepticle On Mars)
 * Johnson Space Center, TX, USA
 * 
 * MABOB I (1.0) architecture
 */

//============== DEFINITIONS & DECLAIRATIONS =================//

//-------------- LIBRARIES -----------------------------------//
/* Call all function libraries required by the sketch. 
 */
  #include <SoftwareSerial.h>     // Serial communication for DFPlayer Mini
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
  const String myNameIs = "NASA-CHAPEA-BobSled";              // name of sketch
  const String verNum = "1.0";                                // version of sketch
  const String lastUpdate = "2022 Oct 18";                    // last update

  #define numPISOregs 2

//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */

  #define loadPin     2           // parallel connection to all 74HC165 PISO shift registers, pin 1
  #define dataInPin   3           // serial connection to nearest 74HC165 PISO shift register, pin 9
  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)

  const int anCathPin[2] = {A1,A2};

  #define audioTxPin 11           // data pin for DF Player Mini, pin 11
  #define audioRxPin 12           // data pin for DF Player Mini, pin 10

//-------------- HARDWARE PARAMETERS -------------------------//

//.............. Serial for DFPlayer Mini ....................//
  const int mp3vol = 30;                                      // confirm min/max of this paramater
  const int mp3eq = 0;
  SoftwareSerial mp3Serial(audioRxPin, audioTxPin);           // RX, TX on Arduino side

//============== GLOBAL VARIABLES ============================//
/* Decrlare variables used by various functions.
 */
  byte PISOdata[numPISOregs];
  byte PISOprev[numPISOregs];
  bool anCath[2];
  bool anCathPrev[2];
  bool somethingNew;

  int controlMode;                                            // last command from PLC
  int processStep = 0;                                        // 
  uint32_t pressTime;                                         // timestamp for button press
  uint32_t heldTime;                                          // timestamp for button hold completion
  uint32_t nextTimeCheck;                                     // scheduled millis count for next event
  uint32_t lastRunSound;                                      // timestamp of last run sound
  int fillProgress;                                           // progress of bottle fill (0-24)

  byte plcMISO;                                               // info to send to PLC
  byte lightByte;                                             // info to sent to LEDs

//============================================================//
//============== SETUP =======================================//
//============================================================//

void setup() {

//-------------- SERIAL MONITOR ------------------------------//

  Serial.begin(19200);                                        // !! Serial monitor must be set to 19200 baud to read feedback !!
  
  Serial.println("Setup initialized.");
  Serial.print(myNameIs);                                     // report the sketch name and last update
  Serial.print(" ver ");
  Serial.println(verNum);
  Serial.print("Last updated on ");
  Serial.println(lastUpdate);

//-------------- PINMODES ------------------------------------//

//.............. Shift Registers .............................//
  pinMode (clockPin, OUTPUT);
  pinMode (loadPin, OUTPUT);
  pinMode (dataInPin, INPUT);
  pinMode (latchPin, OUTPUT);
  pinMode (dataOutPin, OUTPUT);

//.............. Other Pin Modes .............................//
  for (int e = 0; e < 2; e++){
    pinMode (anCathPin[e], INPUT_PULLUP);
  }

//-------------- HARDWARE SETUP -------------------------------//

  randomSeed(analogRead(A0));

//.............. MP3 .........................................//
  mp3Serial.begin(9600);            // begin software serial comm (DFPlayer must use the 9600 baud)
  sendAudioCommand(0x0D,0);         // start mp3
  sendAudioCommand(0x06,mp3vol);    // set volume
  sendAudioCommand(0x07,mp3eq);     // set EQ

//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();
}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {

//-------------- RECEIVE INPUTS ------------------------------//

  readPISO(0,1);                                              // read both PISO registers
  
  for (int r = 0; r < 2; r++){                                // if the data is new, raise the flag
    if (PISOdata[r] != PISOprev[r]){
      somethingNew = true;
    }
  }

  bool bottlesInPlace = true;                                 // assume all 3 HFC bottles are in place
  for (int btl = 5; btl <8; btl++){                           // for each one...
    if (bitRead(PISOdata[0],btl)){                            // if it's not there (active LOW)
      bottlesInPlace = false;                                 // the bottle are not in place
    }
  }

  bool anCathInPlace = true;                                  // assume both anode & cathode are in place
  for (int e = 0; e < 2; e++){                                // for each one...
    bool bitVal = digitalRead(anCathPin[e]);                  // check if it's there
    anCath[e] = bitVal;                                       // record that
    if (bitVal) anCathInPlace = false;                        // if it wasn't there (active LOW), lower the flag
    if (anCath[e] != anCathPrev[e]){                          // if either has changed...
      somethingNew = true;                                    // raise that flag
    }
  }

//-------------- PARSE PLC COMMAND ------------------------------//

  if (plcSignal(1)){                                            // if there is data incoming from the PLC...
    delay (500);                                                // wait 1/2 second
    readPISO(1,1);                                              // re-read the data
    byte doubleCheck = plcSignal(1);                            // record the data
    delay (500);                                                // wait another 1/2 second
    readPISO(1,1);                                              // re-read the data again
    if (plcSignal(1) && plcSignal(1) == doubleCheck){           // if the data is consistant and non-zero...
      controlMode = plcSignal(1);                               // make that the new controlMode
    }
  }
  if (!controlMode) controlMode = 1;                            // if there is no controlMode value, make it a 1 (nominal)
  
/*
 * Failure Modes
 * 1 = norminal
 * 2 = filter
 * 3 = spark fail
 * 4 = rock jam
 */


//============== MAIN TASK FLOW ==============================//

  switch (processStep){
//-------------- RESET ---------------------------------------//
    case 0:                                                   // reset
      plcMISO = 0;
      fillProgress = 0;
      processStep++;
//-------------- POWER UP ------------------------------------//
    case 1:                                                   // power button
      plcMISO = 0;
      if(pressAndHold(0,5000)){
        playTrack(1);
        nextTimeCheck = millis() + 15000;                     // 15 second power up seq
        processStep++;      
      }
       break;
//............................................................//
    case 2:                                                   // power up animation
      blinkLED(0);
      if (millis() >= nextTimeCheck){
        if (!bottlesInPlace){
          lightByte = 0;
          processStep = 1;
        }
        else{
          bitWrite(lightByte,0,1);
          plcMISO = 1;
          processStep++;
        }
        somethingNew = true;
      }
      break;
//-------------- AUTOCLAVE -----------------------------------//
    case 3:                                                   // autoclave button
      if(pressAndHold(1,5000)){
        if (controlMode == 2){
          lightByte = 0;
          processStep = 1;
          break;
        }
        else{
          playTrack(2);
          nextTimeCheck = (millis() + 300000);                // 5 minute autoclave warmup
          processStep++;
        }
      }
      break;
//............................................................//
    case 4:                                                   // autoclave animation
      blinkLED(1);
      if (millis() >= nextTimeCheck){
        bitWrite(lightByte,1,1);
        plcMISO = 2;
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- AUGER ---------------------------------------//
    case 5:                                                   // auger button
      if(pressAndHold(2,5000)){
        playTrack(3);
        lastRunSound = millis();
        nextTimeCheck = millis() + 30000;                     // 30 second auger start
        processStep++;
      }
      break;
//............................................................//
    case 6:                                                   // auger animation
      blinkLED(2);
      if (millis() >= nextTimeCheck){
        bitWrite(lightByte,2,1);
        plcMISO = 3;
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- ELECTROLYZER --------------------------------//
    case 7:                                                   // electrolyzer button
      if(pressAndHold(3,5000)){
        if (!anCathInPlace || (controlMode == 3)){            // electrolyzer fail
          playTrack(6);
          lightByte = 0;
          processStep = 1;
          break;
        }
        else{                                                 // nominal function
          playTrack(4);
          lastRunSound = millis();
          nextTimeCheck = millis() + 10000;
          processStep++;
        }
      }
      break;
//............................................................//
    case 8:                                                   // electrolyzer animation
      blinkLED(3);
      if (millis() >= nextTimeCheck){
        bitWrite(lightByte,3,1);
        plcMISO = 4;
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- FILL ----------------------------------------//
    case 9:                                                   // fill button
      if(pressAndHold(4,5000)){
        playTrack(5);
        lastRunSound = millis();
        nextTimeCheck = millis() + 300000;                    // five minutes
        plcMISO = 5;
        processStep++;
      }
      break;
//............................................................//
    case 10:                                                  // fill progress
      blinkLED(4);
      if (controlMode == 4){
        playTrack(7);
        lightByte = 0;
        processStep = 1;
        break;
      }
      if (millis() >= nextTimeCheck){
        nextTimeCheck = millis() + 300000;                    // five minutes
        fillProgress++;
        somethingNew = true;
      }
      if (fillProgress >= 24){
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- ---------------------------------------------//
    case 11:                                                  // done
      bitWrite(lightByte,4,1);
      plcMISO = 6;
      break;
//-------------- ---------------------------------------------//
  }                                                           // EXIT SWITCH CASE

//.............. Power Off ...................................//
  if (processStep >= 2){
    if(pressAndHold(0,10000)){
      processStep = 0;
      plcMISO = 7;
      lightByte = 0;
      sendSIPO(plcMISO);
      sendSIPO(lightByte);
      pulsePin(latchPin);
      delay(10000);
    }
  }

//............ Anode & Cathode .............................//

  if (processStep >= 8 && processStep < 11){
    for (int bitPos = 6; bitPos <= 7; bitPos++){
      bitWrite(lightByte,bitPos,blinkOrNo(3,5));
    }
    if (!anCathInPlace){
      playTrack(6);
      lightByte = 0;
      processStep = 1;
    }
  }
  if (processStep >= 11){
    bitWrite(lightByte,6,0);
    bitWrite(lightByte,7,0);
  }

//.............. Process Run Sounds .........................//

  if (processStep >= 7 && processStep < 11){
    if (millis()>= lastRunSound + 300000){
      playTrack(3);
      lastRunSound = millis();
    }
  }
//-------------- REGISTER OUTPUTS ----------------------------//

 // if (millis() >= heldTime + 1500){
 //   plcMISO = 0;
 // }
  sendSIPO(plcMISO);
  sendSIPO(lightByte);
  pulsePin(latchPin);
  delay (50);
  
  
//-------------- ROUTINE MAINTAINENCE ------------------------//

  dbts();
  cycleReset();

}
