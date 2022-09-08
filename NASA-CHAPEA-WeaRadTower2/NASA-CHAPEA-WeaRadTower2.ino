

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

  #include <Adafruit_NeoPixel.h>  // WS2812 (NeoPixel) addressable LEDs

  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
//.............. Identifier Data .............................//
  const String myNameIs = "NASA-CHAPEA-WeaRadTower2";         // name of sketch
  const String versionNum = "1.1";                            // version of sketch
  const String lastUpdate = "2022 Sept 07";                   // last update

  #define numPISOregs 2
  #define numLEDs 3                                           // single pixel for the spaceKey
  
//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */
//.............. Digital Pins ................................//
  #define loadPin     2           // parallel connection to all 74HC165 PISO shift registers, pin 1
  #define dataInPin   3           // serial connection to nearest 74HC165 PISO shift register, pin 9
  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define neoPixelPin 5           // data line to WS2812 (NeoPixel) via 470R resistor
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)
  #define powerLED    10          
  #define powerSwitch A1

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
 
  bool powerPress;
  bool powerOn;
  bool powerOld;
  byte gameStage;
  byte blueFlash;
  

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
  pinMode (powerSwitch, INPUT_PULLUP);
  pinMode (powerLED, OUTPUT);

//-------------- HARDWARE SETUP -------------------------------//

  randomSeed(analogRead(A0));
  
  statusLED.begin();
  statusLED.setBrightness(255);
  for (int p=0; p<numLEDs; p++){
    statusLED.setPixelColor(p,0);
  }
  statusLED.show();

//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();

}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {

  readPISO(0,0);
  updateControlMode(0);
  if (!controlMode) controlMode = 1;

//-------------- POWER ---------------------------------------//

  powerPress = digitalRead(powerSwitch);
  int holdTime = 0;
  while (!powerPress){
    holdTime++;
    if (holdTime >= 150){
      if (!powerOn){
        powerOn = true;
        digitalWrite(powerLED,HIGH);
      }
      else{
        powerOn = false;
        digitalWrite(powerLED,LOW);
      }
//      digitalWrite(powerLED,powerOn);
      break;
    }
    delay(10);
    powerPress = digitalRead(powerSwitch);
  }
  
//----------

  if (powerOn && !powerOld){

    runSeq();
    
    sendSIPO(1);
    sendSIPO(255);
    pulsePin(latchPin,10);
    delay(1500);
    sendSIPO(0);
    sendSIPO(255);
    pulsePin(latchPin,10);
  }

  if (powerOn){
    byte blinkers;
    for (int b = 0; b < 8; b++){
      bitWrite(blinkers,b,blinkOrNo(4,7));
    }
    sendSIPO(0);
    sendSIPO(blinkers);
    pulsePin(latchPin,10);

    if(controlMode == 2){
      statusLED.setPixelColor(1,0,255,0);
      statusLED.show();
    }
    if((PISOdata[0] != PISOprev[0]) && controlMode == 1){
      statusLED.setPixelColor(1,255,0,0);
      statusLED.show();
    }
  }

  if (!powerOn){
    for (int p=0; p<numLEDs; p++){
      statusLED.setPixelColor(p,0);
    }
    statusLED.show();
    sendSIPO(0);
    sendSIPO(0);
    pulsePin(latchPin,10);
  }


  dbts();
  cycleReset();

}

//==

void runSeq(){
  if (powerOn){
    for (int p=0; p<numLEDs; p++){
      for (int b=0; b<10; b++){
        statusLED.setPixelColor(p,150,200,0);
        statusLED.show();
        delay(500);
        statusLED.setPixelColor(p,0);
        statusLED.show();
        delay(500);
      }
      statusLED.setPixelColor(p,200,0,0);
      statusLED.show();
    }
  }
}

//============== ROLL A D6 ===================================//
/*
 * This function will return a boolean T/F based on paramaters:
 * dif = minimum threshhold of success
 * die = the size of the die being rolled
 */
bool blinkOrNo(int dif, int die){
  long roll = random(1,die+1);                                // generate a (pseudo)random value between 1 and the die size
  if (roll >= dif) return true;                               // if the result is at or above the target, return a true
  else return false;                                          // otherwise return a false
}
