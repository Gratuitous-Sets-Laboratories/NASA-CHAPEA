

/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA Hydrogen Fuel Cell
 * Johnson Space Center, TX, USA
 * 28 July 2022
 * 
 * MABOB I (1.0) architecture
 */

//============== DEFINITIONS & DECLAIRATIONS =================//

//-------------- LIBRARIES -----------------------------------//
/* Call all function libraries required by the sketch. 
 */
  #include <MaxMatrix.h>
  #include <avr/pgmspace.h>
  #include <Adafruit_NeoPixel.h>  // WS2812 (NeoPixel) addressable LEDs
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
//.............. Identifier Data .............................//
  const String myNameIs = "NASA-CHAPEA-HFC2";                 // name of sketch
  const String versionNum = "1.1";                            // version of sketch
  const String lastUpdate = "2023 FEB 23";                    // last update

  #define numPISOregs 1
  #define numLEDs 27                                          //
 

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

  #define powerButton 9           
  #define powerLED    10

  const int hfcPin[3] = {A0,A1,A2};
  const int startButton[3] = {A3,A4,A5};

//-------------- HARDWARE PARAMETERS -------------------------//
  
  Adafruit_NeoPixel statusLED = Adafruit_NeoPixel(
    numLEDs, neoPixelPin, NEO_GRB + NEO_KHZ800
    );                                                        // neoPixel object name, # of pixels, signal pin, type

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */

  byte PISOdata[numPISOregs];
  byte PISOprev[numPISOregs];
  bool somethingNew;
  
  int controlMode;
  byte activeCell = 1;

  bool powerOn;
  
  byte HFCinPlace[3];
  byte HFCinPrev[3];
  bool buttonPress[3];
  bool buttonPrev[3];

  byte bottleChargeLevel[8] = {0,255,255,255,255,255,255,255};
  int bottleLastInUse[8];
  int lastDischargeTime;
  byte badBottle = 0;

  int minuteCounter;
  uint32_t lastMinuteTick;

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
  for (int bay = 0; bay < 3; bay++){
    pinMode (hfcPin[bay], INPUT);
    pinMode (startButton[bay], INPUT_PULLUP);
  }
  pinMode (powerButton,INPUT_PULLUP);
  pinMode (powerLED, OUTPUT);

//-------------- HARDWARE SETUP -------------------------------//

  statusLED.begin();
  statusLED.setBrightness(255);
  statusLED.show();

  randomSeed(analogRead(A7));

  sendSIPO(0);
  pulsePin(latchPin,10);

//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();

}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {
  
//-------------- UPDATE VARIABLES ----------------------------//

  readPISO(0,0);
  updateControlMode(0);
  if (!controlMode) controlMode = 1;
  bottleRead();
  buttonRead();

//-------------------------------------------------------------//

  if (!powerOn){
    digitalWrite(powerLED,LOW);
    for (int pxl = 0; pxl < numLEDs; pxl++){
      statusLED.setPixelColor(pxl,0);
    }
    statusLED.show();

//.............. Power Up .......................................//

    int holdTime = 0;
    while (!digitalRead(powerButton)){
      delay (10);
      holdTime++;
      if (holdTime >= 500){
        powerOn = true;
        digitalWrite(powerLED,HIGH);
        drainFirstCells();
//        bootUpTest();
        break;
      }
    }
  }

//-------------------------------------------------------------//

  else {                                                        // otherwise the power in ON

    digitalWrite(powerLED,HIGH);

    minClock();


//.............. Auto-Advance on Dead Cell ....................//
    
    for (int bay = 0; bay < 3; bay++){                         // for each bay...
    
      byte bottlePresent = HFCinPlace[bay];                  // not the bottle installed
      byte remainingCharge = bottleChargeLevel[bottlePresent];// and the charge remaining in that bottle
    
      if (buttonPress[bay]){                                  // if the button for that bay has been pressed...
        activeCell = bay +1;                                 // that's the active cell 
      }
    }
      
    byte activeBottle = HFCinPlace[activeCell-1];
    if (!bottleChargeLevel[activeBottle]){
      activeCell++;
    }
 
    if (activeCell > 3){
      activeCell = 1;
    }
//.............. Do The Stuff with the Lights .................//

    bayStatusIndicator();

    displayRemainingCharge(0);
    displayRemainingCharge(1);
    displayRemainingCharge(2);

//.............. Drain / Magic the Power Levels ...............//

    drainCell();
    magicRecharge();
  }

//.............. Bad Cell & Recovery ..........................//

  if (!badBottle && controlMode == 4){
    byte killBay = random(1,4);
    badBottle = HFCinPlace[killBay];
    controlMode = 1;
  }
  bottleChargeLevel[badBottle] = 0;

  if (controlMode == 5){
    badBottle = 0;
    controlMode = 1;
  }

//.............. Power Down ...................................//

    int holdTime = 0;
    while (!digitalRead(powerButton)){
      delay (10);
      holdTime++;
      if (holdTime >= 1000){
        powerOn = false;
        break;
      }
    }
  
//-------------------------------------------------------------//
  dbts();
  cycleReset();

}
