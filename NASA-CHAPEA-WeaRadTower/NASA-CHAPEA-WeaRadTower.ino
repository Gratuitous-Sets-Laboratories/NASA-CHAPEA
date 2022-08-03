

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

  #include <Adafruit_NeoPixel.h>  // WS2812 (NeoPixel) addressable LEDs

  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
  const String myNameIs = "NASA-CHAPEA-WeRadTower: 28 Jul 2022";    // nametag for Serial monitor setup

  #define numLEDs 3                                           // single pixel for the spaceKey
  const int bright = 255;                                     // relative brightness of the Key's neoPixel (0-255)

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
  Serial.println(myNameIs);

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

//-------------- NPX -----------------------------------------//

  statusLED.begin();
  statusLED.setBrightness(bright);
  statusLED.show();

//-------------- HARDWARE SETUP -------------------------------//
  
  statusLED.begin();
  statusLED.setBrightness(bright);
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

//-------------- POWER ---------------------------------------//

  powerPress = digitalRead(powerSwitch);
  int holdTime = 0;
  while (!powerPress){
    holdTime++;
    if (holdTime >= 25){
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
    delay(100);
    powerPress = digitalRead(powerSwitch);
  }
  
//----------

  if (powerOn && !powerOld){
    runSeq();
  }

  if (powerOn){
    sendLightByte(255);
  }

  if (!powerOn){
    for (int p=0; p<numLEDs; p++){
      statusLED.setPixelColor(p,0);
    }
    statusLED.show();
    sendLightByte(0);
  }

/*
  if (!powerOn){
    for (int p=0; p<numLEDs; p++){
      statusLED.setPixelColor(p,0);
    }
    statusLED.show();
    sendLightByte(0);
  }

  else sendLightByte(255);
*/
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
      statusLED.setPixelColor(p,255,0,0);
      statusLED.show();
    }
  }
}
