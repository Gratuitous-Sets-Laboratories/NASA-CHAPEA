

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
  const String versionNum = "1.2";                            // version of sketch
  const String lastUpdate = "2022 Oct 18";                   // last update

//.............. Game Tuning .................................//
  const int fillTime = 5;                                     // minutes to gain 1%
  const int drainTime = 30;                                   // minutes to expend 1%
  
  const int hethS = 800;                                      // hall effect threshhold south
  const int hethN = 200;                                      // hall effect threshhold north

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

  int filterStatus[3] = {3,95,45};
  int filterPrev[2];
  
  byte activeFilterNum;
  byte activeFilterPrev;
  int lastDrainTick;

  byte purgeFilterNum;
  byte purgeFilterPrev;
  int lastFillTick;

  byte badFilter;

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
  pulsePin(latchPin);

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


//--------------- PLC UPDATE ---------------------------------//

  if (plcSignal(0)){
    delay (500);
    readPISO(0,0);
    byte doubleCheck = plcSignal(0);
    delay (500);
    readPISO(0,0);
    if (plcSignal(0) && plcSignal(0) == doubleCheck){
      controlMode = plcSignal(0);
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
        sendSIPO(1);
        pulsePin(latchPin);
        delay(1500);
        sendSIPO(0);
        pulsePin(latchPin);
        break;
      }
    }
  }
//------------------------------------------------------------//

  else {                                                      // POWER IS ON

    digitalWrite(powerLED,HIGH);
//    normalOpLEDs();
    if (longTickSinceSound >= 30){                            // if it's been 30 minutes...
      playTrack(3);                                           // play HVAC sounds
      longTickSinceSound = longTickCount;                     // update that timestamp
    }

//.............. Drain ...........................

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
    filterStatus[2] = 45;

//............. Fill ..............................

   
    int tickFillNum = fillTime;
    if (longTickCount >= lastFillTick + tickFillNum){
      if (purgeFilterNum){
        filterStatus[purgeFilterNum-1]++;
        lastFillTick = longTickCount;
      }
    }
    

//.............. Failure Mode ...................................//

  if (plcSignal(0) == 2){
    badFilter = activeFilterNum;
  }
  if (controlMode == 2){
    filterStatus[badFilter-1] = 0;
  }

//.............. Apollo 13 Patch ................................//

  if (controlMode == 3){
    activeFilterNum = 3;
    filterStatus[2] = 45;
    filterStatus[badFilter-1] = 0;
  }

//.............. LEDs .......

    normalOpLEDs();

//.............. Communicate Full Bottle ........................//

    for (int filt = 0; filt < 2; filt++){
      if (filterStatus[filt] == 100 && filterPrev[filt] == 99){
        sendSIPO(2);
        pulsePin(latchPin);
        delay(1500);
        sendSIPO(0);
        pulsePin(latchPin);
      }
    }
    
//............. power down  ..............................

    int holdTime = 0;
    while(!digitalRead(powerButton)){
      delay(10);
      holdTime++;
      if (holdTime >= 300){
        powerMode = false;
 //       shutDownAnimation();                                    // !!
        sendAudioCommand(0x0E,0);                                 // pause audio
        break;
      }
    }
    if (!activeFilterNum){
      powerMode = false;
 //     shutDownAnimation();
      sendAudioCommand(0x0E,0);
    }
  }

//.........red button .....................................
  if (!redButtonState){
    Serial.println("RED");
    sendSIPO(3);
    pulsePin(latchPin);
    delay(1500);
    sendSIPO(0);
    pulsePin(latchPin);
  }


//.............. Look for New Data ...........................//
  if (PISOdata[0] != PISOprev[0]) somethingNew = true;
  if (activeFilterPrev != activeFilterNum) somethingNew = true;
  if (purgeFilterPrev != purgeFilterNum) somethingNew = true;
  if (powerButtonPrev != powerButtonState) somethingNew = true;
  if (redButtonPrev != redButtonState) somethingNew = true;
//  if (activeFilterNum != activeFilterPrev) somethingNew = true;
//  if (purgeFilterNum != purgeFilterPrev) somethingNew = true;


//=============== ROUTINE MAINTAINENCE =======================//

  dbts();
  cycleReset();

}
