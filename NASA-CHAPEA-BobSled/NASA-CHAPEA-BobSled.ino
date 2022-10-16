

/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA TROM (Trash Recepticle On Mars)
 * Johnson Space Center, TX, USA
 * 28 July 2022
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
  const String verNum = "B.0";                                // version of sketch
  const String lastUpdate = "2022 Oct 16";                    // last update

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

  int controlMode;
  int processStep = 0;
  uint32_t pressTime;
  uint32_t heldTime;
  uint32_t nextTimeCheck;
  int fillProgress;

  byte plcMISO;
  byte lightByte;

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

//-------------- REVEIVE INPUTS ------------------------------//

  readPISO(0,1);
  for (int r = 0; r < 2; r++){
    if (PISOdata[r] != PISOprev[r]){
      somethingNew = true;
    }
  }
  
  for (int e = 0; e < 2; e++){
    anCath[e] = digitalRead(anCathPin[e]);
    if (anCath[e] != anCathPrev[e]){
      somethingNew = true;
    }
  }

  controlMode = PISOdata[0] % 8;
/*
 * Failure Modes
 * 1 = norminal
 * 3 = spark fail
 * 2 = filter
 * 4 = rock jam
 */


//============== MAIN TASK FLOW ==============================//

  sendSIPO();

  switch (processStep){
//-------------- ---------------------------------------------//
    case 0:                                                   // reset
      fillProgress = 0;
      processStep++;
//-------------- ---------------------------------------------//
    case 1:                                                   // power button
      if(pressAndHold(0,5000)){
        playTrack(1);
        nextTimeCheck = millis() + 15000;
        processStep++;
      }
       break;
//-------------- ---------------------------------------------//
    case 2:                                                   // power up animation
      blinkLED(0);
      if (millis() >= nextTimeCheck){
        bitWrite(lightByte,0,1);
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- ---------------------------------------------//
    case 3:                                                   // autoclave button
      if(pressAndHold(1,5000)){
        if (controlMode == 2){                                // filter fail
          //playTrack(x);
          processStep = 1;
          break;
        }
        else{                                                 // nominal function
          playTrack(2);
          nextTimeCheck = (millis() + 30000);
          processStep++;
        }
      }
      break;
//-------------- ---------------------------------------------//
    case 4:                                                   // autoclave animation
      blinkLED(1);
      if (millis() >= nextTimeCheck){
        bitWrite(lightByte,1,1);
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- ---------------------------------------------//
    case 5:                                                   // auger button
      if(pressAndHold(2,5000)){
        playTrack(3);
        nextTimeCheck = millis() + 30000;
        processStep++;
      }
      break;
//-------------- ---------------------------------------------//
    case 6:                                                   // auger animation
      blinkLED(2);
      if (millis() >= nextTimeCheck){
        bitWrite(lightByte,2,1);
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- ---------------------------------------------//
    case 7:                                                   // electrolyzer button
      if(pressAndHold(3,5000)){
        if (anCath[0] || anCath[1]){
          controlMode = 3;
        }
        if (controlMode == 3){                                // electrolyzer fail
          playTrack(6);
          processStep = 1;
          break;
        }
        else{                                                 // nominal function
          playTrack(4);
          nextTimeCheck = millis() + 10000;
          processStep++;
        }
      }
      break;
//-------------- ---------------------------------------------//
    case 8:                                                   // electrolyzer animation
      blinkLED(3);
      if (millis() >= nextTimeCheck){
        bitWrite(lightByte,3,1);
        processStep++;
        somethingNew = true;
      }
      break;
//-------------- ---------------------------------------------//
    case 9:                                                   // fill button
      if(pressAndHold(4,5000)){
        playTrack(5);
        nextTimeCheck = millis() + (5*60*1000);
        processStep++;
      }
      break;
//-------------- ---------------------------------------------//
    case 10:                                                  // fill progress
      blinkLED(4);
      if (controlMode == 4){
        playTrack(7);
        processStep = 1;
        break;
      }
      if (millis() >= nextTimeCheck){
        nextTimeCheck = millis() + 10000;
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
      break;
//-------------- ---------------------------------------------//
  }                                                           // EXIT SWITCH CASE

//-------------- ROUTINE MAINTAINENCE ------------------------//

  dbts();
  cycleReset();

}
