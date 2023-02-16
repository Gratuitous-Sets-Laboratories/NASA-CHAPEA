

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
  #include <SoftwareSerial.h>
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
  const String myNameIs = "NASA-CHAPEA-CO2, Aug 2022";    // nametag for Serial monitor setup

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
  
  byte PISOdata;
  byte PISOprev;

  byte functionMode;
  bool powerOn = 0;
  uint32_t cycleBeginTime;
  bool cycleActiveFlag;
  
  int activeFiltRead;
  byte activeFilterNum;
  byte activeFilterPrev;
  int purgeFiltRead;
  byte purgeFilterNum;
  byte purgeFilterPrev;
  bool powerButtonState;
  bool powerButtonPrev;
  bool redButtonState;
  bool redButtonPrev;

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

//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();
}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {
  
//-------------- UPDATE VARIABLES ----------------------------//

  readPISO();
  
  activeFiltRead = analogRead(activeFilt);
  purgeFiltRead = analogRead(purgeFilt);
  powerButtonState = digitalRead(powerButton);
  redButtonState = digitalRead(redButton);
  

//------------------ PLC UPDATE ----------------------------//
/*
  if (parsePLC(0)){
    delay (500);
    readPISO();
    byte doubleCheck = parsePLC(1);
    delay (500);
    readPISO();
    if (parsePLC(0) && parsePLC(0) == doubleCheck){
      functionMode = parsePLC(0);
    }
  }
*/
//------------------------------------------------------------//

  activeFilterNum = 0;
  if (activeFiltRead > 800) activeFilterNum = 2;
  else if (activeFiltRead < 200) activeFilterNum = 1; 

  purgeFilterNum = 0;
  if (purgeFiltRead > 800) purgeFilterNum = 2;
  else if (purgeFiltRead < 200) purgeFilterNum = 1; 

//------------------------------------------------------------//

  if (!powerOn){
    
    digitalWrite(powerLED, LOW);
    for (int p = 0; p < numLEDs; p++){
      statusLED.setPixelColor(p,0);
    }
    statusLED.show();

    int buttonHold = 0;
    while (!powerButtonState){
      powerButtonState = digitalRead(powerButton);
      buttonHold++;
      delay(10);
      if (buttonHold >= 100){
        powerOn = true;
        cycleActiveFlag = true;
        cycleBeginTime = millis();
        
     // animation
     
        sendSIPO(1);
        pulsePin(latchPin,10);
        delay (1500);
        sendSIPO(0);
        pulsePin(latchPin,10);
    
        break;
      }
    }
  }

  else if (powerOn){

    digitalWrite(powerLED,HIGH);

    for (int pxl = 0; pxl < numLEDs; pxl++){
      statusLED.setPixelColor(pxl,100,100,100);
    }
    statusLED.show();
    
    if (cycleActiveFlag && millis() >= cycleBeginTime + 5000){
      cycleActiveFlag = false;
      sendSIPO(2);
      pulsePin(latchPin,10);
      delay(1500);
      sendSIPO(0);
      pulsePin(latchPin,10);
    }
    
    int buttonHold = 0;
    while (!powerButtonState){
      powerButtonState = digitalRead(powerButton);
      buttonHold++;
      delay(10);
      if (buttonHold >= 100){
        powerOn = false;
        break;
      }
    }
  }

//------------------------------------------------------------//

  if (!redButtonState){
    sendSIPO(3);
    pulsePin(latchPin,10);
    delay(1500);
    sendSIPO(0);
    pulsePin(latchPin,10);
  }

//------------------------------------------------------------//

//  if (PISOdata != PISOprev) lastInputTime = millis();

  dbts();
  cycleReset();

}
