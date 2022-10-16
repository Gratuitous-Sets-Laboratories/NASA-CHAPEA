

/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA CO2 SCRUBBER (Lovell & Krantz)
 * Johnson Space Center, TX, USA
 * 
 * MABOB I (1.0) architecture
 * 
 * NOTES:
 * Need solution for Apollo 13 condition
 * shutdown sound or silence (find STOP cmd for DFP)
 */

//============== DEFINITIONS & DECLAIRATIONS =================//

//-------------- LIBRARIES -----------------------------------//
/* Call all function libraries required by the sketch. 
 */
  #include <Adafruit_NeoPixel.h>  // WS2812 (NeoPixel) addressable LEDs
  #include <SoftwareSerial.h>
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
//.............. Identifier Data .............................//
  const String myNameIs = "NASA-CHAPEA-CO2-2";                // name of sketch
  const String versionNum = "1.1";                            // version of sketch
  const String lastUpdate = "2022 Sept 09";                   // last update

//.............. Game Tuning .................................//
  const int fillTime = 5;                                     // minutes to gain 1%
  const int drainTime = 30;                                   // minutes to expend 1%
  
  const int hethS = 800;
  const int hethN = 200;

//.............. Hardware Installed ..........................//
  #define numPISOregs 1
  #define numLEDs 2

//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */

  #define loadPin     2           // parallel connection to all 74HC165 PISO shift registers, pin 1
  #define dataInPin   3           // serial connection to nearest 74HC165 PISO shift register, pin 9
  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)
  
  #define neoPixelPin 5
  #define audioTxPin  11          // data pin for DF Player Mini, pin 11
  #define audioRxPin  12          // data pin for DF Player Mini, pin 10
 
  #define powerLED    10
  #define redButton   8
  #define powerButton 9
  #define activeFilt  A1
  #define purgeFilt   A2

//============== HARDWARE PARAMATERS =========================//

  Adafruit_NeoPixel statusLED = Adafruit_NeoPixel(
    numLEDs, neoPixelPin, NEO_GRB + NEO_KHZ800
  );                                                          // neoPixel object name, # of pixels, signal pin, type
  SoftwareSerial mp3Serial(audioRxPin, audioTxPin);           // RX, TX on Arduino side

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
  
  byte PISOdata[numPISOregs];
  byte PISOprev[numPISOregs];
  bool somethingNew;

  int filterStatus[2] = {3,95};
  int filterPrev[2];
  
  byte activeFilterNum;
  byte activeFilterPrev;
  int lastDrainTick;

  byte purgeFilterNum;
  byte purgeFilterPrev;
  int lastFillTick;

  uint32_t longTickCount;
  uint32_t lastLongTick = millis();
  int longTickSinceSound;
  bool alarmReady;
  
  bool powerButtonState;
  bool powerButtonPrev;
  bool redButtonState;
  bool redButtonPrev;

  byte controlMode;
  bool powerMode;

//============================================================//
//============== SETUP =======================================//
//============================================================//

void setup() {

//-------------- SERIAL MONITOR ------------------------------//

  Serial.begin(19200);                                        // !! Serial monitor must be set to 19200 baud to read feedback !!
  Serial.println();
  Serial.println("Setup initialized.");
  Serial.print(myNameIs);                                     // report the sketch name and last update
  Serial.print(" ver ");
  Serial.println(versionNum);
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
  pinMode (powerLED, OUTPUT);
  pinMode (powerButton, INPUT_PULLUP);
  pinMode (redButton, INPUT_PULLUP);
  pinMode (activeFilt, INPUT);
  pinMode (purgeFilt, INPUT);

//-------------- HARDWARE SETUP -------------------------------//

  statusLED.begin();
  statusLED.setBrightness(255);
  statusLED.show();

  sendSIPO(0);
  pulsePin(latchPin,10);

  mp3Serial.begin(9600);
  sendAudioCommand(0x0D,0);         // start mp3
  sendAudioCommand(0x06,30);        // set volume to max
  sendAudioCommand(0x07,0);         // set EQ to normal

//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();
}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {
  
//-------------- UPDATE VARIABLES ----------------------------//
//.............. Raw Data ....................................//
  readPISO(0,0);                                              // incoming signal from PLC

  powerButtonState = digitalRead(powerButton);                // active LOW from power button
  redButtonState = digitalRead(redButton);                    // cative LOW from mystery buttons
  
  int activeFiltRead = analogRead(activeFilt);                // raw data from "active" hall effect
  activeFilterNum = 0;                                        // assume no active filter
  if (activeFiltRead > hethS) activeFilterNum = 2;            // if the analog read is above the South mark it is #2
  else if (activeFiltRead < hethN) activeFilterNum = 1;       // or if its below the North mark it's #1

  int purgeFiltRead = analogRead(purgeFilt);                  // raw data from "purge" hall effect
  purgeFilterNum = 0;                                         // assume no active filter
  if (purgeFiltRead > hethS) purgeFilterNum = 2;              // if the analog read is above the South mark it is #2
  else if (purgeFiltRead < hethN) purgeFilterNum = 1;         // or if its below the North mark it's #1

//.............. Look for New Data ...........................//
  if (PISOdata[0] != PISOprev[0]) somethingNew = true;
  if (activeFilterPrev != activeFilterNum) somethingNew = true;
  if (purgeFilterPrev != purgeFilterNum) somethingNew = true;
  if (powerButtonPrev != powerButtonState) somethingNew = true;
  if (redButtonPrev != redButtonState) somethingNew = true;
  if (activeFilterNum != activeFilterPrev) somethingNew = true;
  if (purgeFilterNum != purgeFilterPrev) somethingNew = true;

//--------------- PLC UPDATE ---------------------------------//
  byte plcMOSI = plcSignal(0);
  if (plcMOSI){
    delay (500);
    readPISO(0,0);
    byte doubleCheck = PISOdata[0];
    delay (500);
    readPISO(0,0);
    if (plcMOSI && plcMOSI == doubleCheck){
      controlMode = plcMOSI;
    }
  }
  if (!controlMode) controlMode = 1;

//=============== LONG MINUTE TIMER ==========================//

  int minutes = 1;
  if (millis() >= lastLongTick + (minutes * 60000)){            // RESET TO 60K for MINUTES!
    longTickCount++;
    somethingNew = true;
    alarmReady = true;
    lastLongTick = millis();
  }

//=============== MAIN GAME FLOW =============================//

  if (!powerMode){                                            // POWER IS OFF
    
    digitalWrite(powerLED,LOW);
    statusLED.setPixelColor(0,0);
    statusLED.setPixelColor(1,0);
    statusLED.show();
    
//.............. Power Up ....................................//
    int holdTime = 0;
    while(!digitalRead(powerButton)){
      delay(10);
      holdTime++;
      if (holdTime >= 300){
        powerMode = true;
        startUpAnimation();                                   // !!
        break;
      }
    }
  }
//------------------------------------------------------------//

  else {                                                      // POWER IS ON

    digitalWrite(powerLED,HIGH);
    normalOpLEDs();
    if (longTickSinceSound >= 30){
      playTrack(3);
      longTickSinceSound = longTickCount;
    }

//............. Fill / Drain ..............................

    int tickFillNum = fillTime;
    if (longTickCount >= lastFillTick + tickFillNum){
      if (purgeFilterNum){
        filterStatus[purgeFilterNum-1]++;
        lastFillTick = longTickCount;
      }
    }
    int tickDrainNum = drainTime;
    if (longTickCount >= lastDrainTick + tickDrainNum){
      if (activeFilterNum){
        filterStatus[activeFilterNum-1]--;
        lastDrainTick = longTickCount;
      }
    }
    for (int filt = 0; filt < 2; filt++){
      filterStatus[filt] = constrain(filterStatus[filt],0,100);
    }

//......................................

    for (int filt = 0; filt < 2; filt++){
      if (filterStatus[filt] == 100 && filterPrev[filt] == 99){
        sendSIPO(2);
        pulsePin(latchPin,10);
        delay(1500);
        sendSIPO(0);
        pulsePin(latchPin,10);
      }
    }
    
//............. power down  ..............................

    int holdTime = 0;
    while(!digitalRead(powerButton)){
      delay(10);
      holdTime++;
      if (holdTime >= 300){
        powerMode = false;
        shutDownAnimation();                                   // !!
        break;
      }
    }
    if (!activeFilterNum){
      powerMode = false;
      shutDownAnimation();
    }
  }

//.........red button .....................................
  if (!redButtonState){
    Serial.println("RED");
    sendSIPO(3);
    pulsePin(latchPin,10);
    delay(1500);
    sendSIPO(0);
    pulsePin(latchPin,10);
  }



//=============== ROUTINE MAINTAINENCE =======================//

  dbts();
  cycleReset();

}
