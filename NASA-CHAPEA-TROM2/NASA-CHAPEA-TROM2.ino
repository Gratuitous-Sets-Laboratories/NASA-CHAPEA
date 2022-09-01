

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
  #include <MaxMatrix.h>
  #include <avr/pgmspace.h>
  #include <Adafruit_NeoPixel.h>  // WS2812 (NeoPixel) addressable LEDs
  #include <SoftwareSerial.h>
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
  const String myNameIs = "NASA-CHAPEA-TROM2: 31 Aug 2022";    // nametag for Serial monitor setup
  
  #define gridsInUse 3

  #define numLEDs 1                                           // single pixel for the spaceKey
  const int bright = 255;                                     // relative brightness of the Key's neoPixel (0-255)
  const int numOfSettings = 5;
  const int clicksPerSetting = 3;

//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */

  #define reClk       2
  #define reDat       3
  #define reBtn       4
  #define neoPixelPin 5
  const int leverPin [2] = {6,7};
  #define execPin     8
  #define relayPin    9           // trigger pin for relay K1 via 1K resistor & 2N2222 or 2N7000
  #define hatchPin    10
  #define audioTxPin  11          // data pin for DF Player Mini, pin 11
  #define audioRxPin  12          // data pin for DF Player Mini, pin 10
  #define gridClk     A1
  #define gridCS      A2
  #define gridDat     A3

//-------------- CHARACTER SPECS  ----------------------------//

  String settingName[5] = {"TRASH","RECY","3DPW","P.P.B","FECL"};

PROGMEM const unsigned char CH[] = {
  3, 8, B00000000, B00000000, B00000000, B00000000, B00000000, // space
  1, 8, B01011111, B00000000, B00000000, B00000000, B00000000, // !
  3, 8, B00000011, B00000000, B00000011, B00000000, B00000000, // "
  5, 8, B00010100, B00111110, B00010100, B00111110, B00010100, // #
  4, 8, B00100100, B01101010, B00101011, B00010010, B00000000, // $
  5, 8, B01100011, B00010011, B00001000, B01100100, B01100011, // %
  5, 8, B00110110, B01001001, B01010110, B00100000, B01010000, // &
  1, 8, B00000011, B00000000, B00000000, B00000000, B00000000, // '
  3, 8, B00011100, B00100010, B01000001, B00000000, B00000000, // (
  3, 8, B01000001, B00100010, B00011100, B00000000, B00000000, // )
  5, 8, B00101000, B00011000, B00001110, B00011000, B00101000, // *
  5, 8, B00001000, B00001000, B00111110, B00001000, B00001000, // +
  2, 8, B10110000, B01110000, B00000000, B00000000, B00000000, // ,
  4, 8, B00001000, B00001000, B00001000, B00001000, B00000000, // -
  2, 8, B01100000, B01100000, B00000000, B00000000, B00000000, // .
  4, 8, B01100000, B00011000, B00000110, B00000001, B00000000, // /
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // 0
  3, 8, B01000010, B01111111, B01000000, B00000000, B00000000, // 1
  4, 8, B01100010, B01010001, B01001001, B01000110, B00000000, // 2
  4, 8, B00100010, B01000001, B01001001, B00110110, B00000000, // 3
  4, 8, B00011000, B00010100, B00010010, B01111111, B00000000, // 4
  4, 8, B00100111, B01000101, B01000101, B00111001, B00000000, // 5
  4, 8, B00111110, B01001001, B01001001, B00110000, B00000000, // 6
  4, 8, B01100001, B00010001, B00001001, B00000111, B00000000, // 7
  4, 8, B00110110, B01001001, B01001001, B00110110, B00000000, // 8
  4, 8, B00000110, B01001001, B01001001, B00111110, B00000000, // 9
  2, 8, B01010000, B00000000, B00000000, B00000000, B00000000, // :
  2, 8, B10000000, B01010000, B00000000, B00000000, B00000000, // ;
  3, 8, B00010000, B00101000, B01000100, B00000000, B00000000, // <
  3, 8, B00010100, B00010100, B00010100, B00000000, B00000000, // =
  3, 8, B01000100, B00101000, B00010000, B00000000, B00000000, // >
  4, 8, B00000010, B01011001, B00001001, B00000110, B00000000, // ?
  5, 8, B00111110, B01001001, B01010101, B01011101, B00001110, // @
  4, 8, B01111110, B00010001, B00010001, B01111110, B00000000, // A
  4, 8, B01111111, B01001001, B01001001, B00110110, B00000000, // B
  4, 8, B00111110, B01000001, B01000001, B00100010, B00000000, // C
  4, 8, B01111111, B01000001, B01000001, B00111110, B00000000, // D
  4, 8, B01111111, B01001001, B01001001, B01000001, B00000000, // E
  4, 8, B01111111, B00001001, B00001001, B00000001, B00000000, // F
  4, 8, B00111110, B01000001, B01001001, B01111010, B00000000, // G
  4, 8, B01111111, B00001000, B00001000, B01111111, B00000000, // H
  3, 8, B01000001, B01111111, B01000001, B00000000, B00000000, // I
  4, 8, B00110000, B01000000, B01000001, B00111111, B00000000, // J
  4, 8, B01111111, B00001000, B00010100, B01100011, B00000000, // K
  4, 8, B01111111, B01000000, B01000000, B01000000, B00000000, // L
  5, 8, B01111111, B00000010, B00001100, B00000010, B01111111, // M
  5, 8, B01111111, B00000100, B00001000, B00010000, B01111111, // N
  4, 8, B00111110, B01000001, B01000001, B00111110, B00000000, // O
  4, 8, B01111111, B00001001, B00001001, B00000110, B00000000, // P
  4, 8, B00111110, B01000001, B01000001, B10111110, B00000000, // Q
  4, 8, B01111111, B00001001, B00001001, B01110110, B00000000, // R
  4, 8, B01000110, B01001001, B01001001, B00110010, B00000000, // S
  5, 8, B00000001, B00000001, B01111111, B00000001, B00000001, // T
  4, 8, B00111111, B01000000, B01000000, B00111111, B00000000, // U
  5, 8, B00001111, B00110000, B01000000, B00110000, B00001111, // V
  5, 8, B00111111, B01000000, B00111000, B01000000, B00111111, // W
  5, 8, B01100011, B00010100, B00001000, B00010100, B01100011, // X
  5, 8, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
  4, 8, B01100001, B01010001, B01001001, B01000111, B00000000, // Z
  2, 8, B01111111, B01000001, B00000000, B00000000, B00000000, // [
  4, 8, B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
  2, 8, B01000001, B01111111, B00000000, B00000000, B00000000, // ]
  3, 8, B00000010, B00000001, B00000010, B00000000, B00000000, // hat
  4, 8, B01000000, B01000000, B01000000, B01000000, B00000000, // _
  2, 8, B00000001, B00000010, B00000000, B00000000, B00000000, // `
  4, 8, B00100000, B01010100, B01010100, B01111000, B00000000, // a
  4, 8, B01111111, B01000100, B01000100, B00111000, B00000000, // b
  4, 8, B00111000, B01000100, B01000100, B00101000, B00000000, // c
  4, 8, B00111000, B01000100, B01000100, B01111111, B00000000, // d
  4, 8, B00111000, B01010100, B01010100, B00011000, B00000000, // e
  3, 8, B00000100, B01111110, B00000101, B00000000, B00000000, // f
  4, 8, B10011000, B10100100, B10100100, B01111000, B00000000, // g
  4, 8, B01111111, B00000100, B00000100, B01111000, B00000000, // h
  3, 8, B01000100, B01111101, B01000000, B00000000, B00000000, // i
  4, 8, B01000000, B10000000, B10000100, B01111101, B00000000, // j
  4, 8, B01111111, B00010000, B00101000, B01000100, B00000000, // k
  3, 8, B01000001, B01111111, B01000000, B00000000, B00000000, // l
  5, 8, B01111100, B00000100, B01111100, B00000100, B01111000, // m
  4, 8, B01111100, B00000100, B00000100, B01111000, B00000000, // n
  4, 8, B00111000, B01000100, B01000100, B00111000, B00000000, // o
  4, 8, B11111100, B00100100, B00100100, B00011000, B00000000, // p
  4, 8, B00011000, B00100100, B00100100, B11111100, B00000000, // q
  4, 8, B01111100, B00001000, B00000100, B00000100, B00000000, // r
  4, 8, B01001000, B01010100, B01010100, B00100100, B00000000, // s
  3, 8, B00000100, B00111111, B01000100, B00000000, B00000000, // t
  4, 8, B00111100, B01000000, B01000000, B01111100, B00000000, // u
  5, 8, B00011100, B00100000, B01000000, B00100000, B00011100, // v
  5, 8, B00111100, B01000000, B00111100, B01000000, B00111100, // w
  5, 8, B01000100, B00101000, B00010000, B00101000, B01000100, // x
  4, 8, B10011100, B10100000, B10100000, B01111100, B00000000, // y
  3, 8, B01100100, B01010100, B01001100, B00000000, B00000000, // z
  3, 8, B00001000, B00110110, B01000001, B00000000, B00000000, // {
  1, 8, B01111111, B00000000, B00000000, B00000000, B00000000, // |
  3, 8, B01000001, B00110110, B00001000, B00000000, B00000000, // }
  4, 8, B00001000, B00000100, B00001000, B00000100, B00000000, // ~
};

//-------------- HARDWARE PARAMETERS -------------------------//

  MaxMatrix grid(gridDat,gridCS,gridClk,gridsInUse);
  
  Adafruit_NeoPixel statusLED = Adafruit_NeoPixel(
    numLEDs, neoPixelPin, NEO_GRB + NEO_KHZ800
    );                                                        // neoPixel object name, # of pixels, signal pin, type
  SoftwareSerial mp3Serial(audioRxPin, audioTxPin);           // RX, TX on Arduino side

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
//.............. Raw Input Data ..............................//
  bool hatchStt;
  bool hatchOld;
  int leverStt;
  int leverOld;
  bool execStt;
  bool execOld;

  bool somethingNew;            // used to trigger Serial monitor feedback - made global to wake the system from sleep mode
  uint32_t newInputTimeStamp    // updated whenever there has been an input, used to trigger sleep mode

  byte tromStatus;              // general mode (sleep, input, process, etc)
  
  byte dialStt = 128;           // the "absolute" location of the rotary encodery, arbitrarily starting in the middle
  byte dialOld;                 // the previous cycle's dialStt
  byte setting;                 // the trash process setting the the TROM has selected
/*
  bool btnStt;                  // the current state of the EXECUTE button
  bool btnOld;                  // the previous state of the EXECUTE button
  bool levStt[2];               // the current state of the PHES lever
  bool levOld[2];               // the previous state of the PHES lever
*/
  byte buffer[10];              // used for the MAX7219 1088AS LED grids

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

  pinMode (reClk,INPUT);
  pinMode (reDat,INPUT);
  pinMode (reBtn,INPUT);
  for (int x=0; x<2; x++) pinMode (leverPin[x],INPUT);
  pinMode (execPin,INPUT_PULLUP);
  pinMode (hatchPin,INPUT_PULLUP);
  pinMode (relayPin,OUTPUT);
  pinMode (audioRxPin, INPUT);
  pinMode (audioTxPin, OUTPUT);

//-------------- HARDWARE SETUP -------------------------------//

  attachInterrupt(digitalPinToInterrupt(reClk), readDial, FALLING);

  grid.init();
  grid.setIntensity(6);
  
  statusLED.begin();
  statusLED.setBrightness(bright);
  statusLED.show();

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
  
  hatchStt = false;                                           // assume the hatch is open
  if (!digitalRead(hatchPin)) hatchStt = true;                // if the latch button is pushed, it is closed
  if (hatchStt != hatchOld) somethingNew = true;              // if this is a change from the last cycle, flag it

  leverStt = 0;                                               // assume the locking lever is nowhere
  bool levN = digitalRead(leverPin[0]);                       // mark if it reads north (up)
  bool levS = digitalRead(leverPin[1]);                       // mark if it reads south (down)
  if      (levN && !levS) leverStt = 1;                       // if he north reads without the south, it is up
  else if (levS && !levN) leverStt = -1;                      // othrwise if the south reads without the north, it's down
  if (leverStt != leverOld) somethingNew = true;              // if this is a change from the last cycle, flag it

  execStt = false;                                            // assume that the execute button isn't pressed
  if (!digitalRead(execPin)) execStt = true;                  // if it is, mark it as such
  if (execStt != execOld) somethingNew = true;                // if this is a change from the last cycle, flag it

  // NOTE: the Rotary Encoder's input comes from the readDial interrupt function

//============== MAIN "GAME" FLOW ============================//

  switch (tromStatus){

//.............. Sleep Mode ..................................//
    case 0:
    
      grid.clear();
      statusLED.setPixelColor(0,0);
      statusLED.show();
      digitalWrite(relayPin,LOW);
      
      if (somethingNew){
        tromStatus++;
      }
      break;
 //.............. (Un)Lock & Load Mode .......................//
    case 1:

      printText("OPEN",2);

      if (leverStt == -1){
        statusLED.setPixelColor(0,200,0,0);
        statusLED.show();
        digitalWrite(relayPin,LOW);
      }

      else if (leverStt == 1){
        statusLED.setPixelColor(0,0,200,0);
        statusLED.show();
        digitalWrite(relayPin,HIGH);
      }

      if (!hatchStt){
        gameStage++;
      }
      break;
//...........................................................//
// allow for toggling of locking lever
    case 2:

      statusLED.setPixelColor(0,0,200,0);
      statusLED.show();
      digitalWrite(relayPin,HIGH);
    
      settingReadout();
      
      if (digitalRead(levPin[0])){
        digitalWrite(relayPin,HIGH);
        statusLED.setPixelColor(0,200,0,0);
        statusLED.show();
      }
      else if (digitalRead(levPin[1]) && digitalRead(hatchPin) == 0){
        digitalWrite(relayPin,LOW);
        statusLED.setPixelColor(0,0,200,0);
        statusLED.show();

        btnStt = false;
        if (!digitalRead(buttonPin)) btnStt = true;
        int holdTime = 0;
        while(btnStt){
          holdTime++;
          delay(100);
          if (holdTime >= 25){
            tromStatus++;
            break;
          }
        }
      }
      break;

    case 2:
      //
      break;
      
  }

//-


//-
  dbts();
  cycleReset();

}
