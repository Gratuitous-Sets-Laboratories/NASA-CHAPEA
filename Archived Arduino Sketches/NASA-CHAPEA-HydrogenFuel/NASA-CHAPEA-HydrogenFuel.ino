

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
  #include <MaxMatrix.h>
  #include <avr/pgmspace.h>
  #include <Adafruit_NeoPixel.h>  // WS2812 (NeoPixel) addressable LEDs
  #include <SoftwareSerial.h>
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
  const String myNameIs = "NASA-CHAPEA-HydrogenFuel: Aug 2022";    // nametag for Serial monitor setup

  #define numPISOregs 1
  #define numLEDs 27                                           // single pixel for the spaceKey
 

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

  const int hfcPin[3] = {A0,A1,A2};
  const int startButton[3] = {A3,A4,A5};

//-------------- CHARACTER SPECS  ----------------------------//

  String settingName[5] = {"TRASH","RECY","3DPW","P.P.B","FECL"};



//-------------- HARDWARE PARAMETERS -------------------------//
  
  Adafruit_NeoPixel statusLED = Adafruit_NeoPixel(
    numLEDs, neoPixelPin, NEO_GRB + NEO_KHZ800
    );                                                        // neoPixel object name, # of pixels, signal pin, type

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */

  byte PISOdata[numPISOregs];
  byte PISOprev[numPISOregs];
  int controlMode;
  byte activeCell;
  
  byte HFCinPlace[3];
  byte HFCinPrev[3];
  bool buttonPress[3];
  bool buttonPrev[3];

  byte bottleChargeLevel[7] = {0,16,32,64,128,200,255};
  uint32_t lastDischargeTime;

//============================================================//
//============== SETUP =======================================//
//============================================================//

void setup() {

//-------------- SERIAL MONITOR ------------------------------//

  Serial.begin(19200);                                        // !! Serial monitor must be set to 19200 baud to read feedback !!
  Serial.println();
  Serial.println("Setup initialized.");
  Serial.println(myNameIs);

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

//-------------- HARDWARE SETUP -------------------------------//

  statusLED.begin();
  statusLED.setBrightness(255);
  statusLED.show();

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

//----

  for (int bay = 0; bay < 3; bay++){
    
    byte bottlePresent = HFCinPlace[bay];
    byte remainingCharge = bottleChargeLevel[bottlePresent];
    
    if (buttonPress[bay]){
      activeCell = bay+1;
      if (!remainingCharge){
        activeCell++;
      }
      if (activeCell > 3){
        activeCell = 1;
      }
    }
  }
  

  bayStatusIndicator();

  displayRemainingCharge(0);
  displayRemainingCharge(1);
  displayRemainingCharge(2);
  
/*
  for (int bay = 0; bay < 3; bay++){
    if (buttonPress[bay]){
      sendToPLC(bay+1,1500);
    }
  }
*/
//-----------------------------------------------------------//
  dbts();
  cycleReset();

}
