/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA TROM (Trash Recepticle On Mars)
 * Johnson Space Center, TX, USA
 * 
 * MABOB I (1.0) architecture
 * 
 * NOTES:
 * Add sleep mode
 */

//============== DEFINITIONS & DECLAIRATIONS =================//

//-------------- LIBRARIES -----------------------------------//
/* Call all function libraries required by the sketch. 
 */
  #include <MaxMatrix.h>          // MAX7219 for 1088AS LED grids
  #include <avr/pgmspace.h>
  #include <Adafruit_NeoPixel.h>  // WS2812 (NeoPixel) addressable LEDs
  #include <SoftwareSerial.h>
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
//.............. Identifier Data .............................//
  const String myNameIs = "NASA-CHAPEA-TROM2";                // name of sketch
  const String versionNum = "1.1";                            // version of sketch
  const String lastUpdate = "2022 Sept 07";                   // last update
  
//.............. Game Tuning Factors .........................//
  String settingName[5] = {"TRSH","RECY","3DPW","P.I.B","FECL"}; // names for each procSetting
  const int processTime[5] = {70,75,110,60,25};               // time in seconds for each process to complete
  const int processSound[5] = {1,2,4,3,5};                    // transducer MP3 track number for each process
  const int sleepDelay = 10;                                  // time since last new input for the TROM to go into sleep mode (in min)
  
//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */
  #define reClk       2           // rotary encoder clock
  #define reDat       3           // rotary encoder data
  #define reBtn       4           // rotary encoder button
  #define neoPixelPin 5           // WS2812 NeoPixel
  const int leverPin [2] = {6,7}; // PHES lever
  #define execPin     8           // execute button
  #define relayPin    9           // trigger pin for relay K1 via 1K resistor & 2N2222 or 2N7000
  #define hatchPin    10          // hatch limit switch
  #define audioTxPin  11          // data pin for DF Player Mini, pin 11
  #define audioRxPin  12          // data pin for DF Player Mini, pin 10
  #define gridClk     A1          // MAX7219 1088S clock
  #define gridCS      A2          // MAX7219 1088S chip select
  #define gridDat     A3          // MAX7219 1088S data

//-------------- CHARACTER SPECS  ----------------------------//
/*
 * The following is the bitmap for each printable character on the 1088AS
 * #ofCols, #ofRows, 5 byte values for each significant col
 */
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

//============== HARDWARE PARAMETERS =========================//

//.............. MAX 7219 1088AS LED Matrix ..................//
  #define gridsInUse 3                                        // number of grids in the daisy chain
  MaxMatrix grid(gridDat,gridCS,gridClk,gridsInUse);          // name of grid (various pin numbers)
  
//.............. WS2812B NeoPixels ...........................//
  #define numLEDs 1                                           // number of pixels in the daisy chain
  Adafruit_NeoPixel statusLED = Adafruit_NeoPixel(            // neoPixel object name
    numLEDs, neoPixelPin, NEO_GRB + NEO_KHZ800);              // # of pixels, signal pin, type
                                                            
//.............. Serial for DFPlayer Mini ....................//
  SoftwareSerial mp3Serial(audioRxPin, audioTxPin);           // RX, TX on Arduino side

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
//.............. Raw Input Data ..............................//
  bool hatchStt;                // main hatch (0 = closed, 1 = open)
  bool hatchOld;                // same from previous cycle
  int leverStt;                 // PHES lever (-1 = down, 1 = up)
  int leverOld;                 // same from previous cycle
  bool execStt;                 // execute button (active LOW)
  bool execOld;                 // same from previous cycle
  
//.............. Derived Input Data ..........................//
  byte dialStt = 128;           // the "absolute" location of the rotary encodery, arbitrarily starting in the middle
  byte dialOld;                 // the previous cycle's dialStt
  
//.............. Primary Control Variables ...................//
  bool inProc;
  byte tromStatus;              // general mode (sleep, input, process, etc)
  byte procSetting;             // the trash process setting the the TROM has selected

//.............. Misc ........................................//
  bool somethingNew;            // used to trigger Serial monitor feedback - made global to wake the system from sleep mode
  uint32_t newInputTimeStamp;   // updated whenever there has been an input, used to trigger sleep mode
  int countdown;                // how far the TROM is in its current trash process
  byte buffer[10];              // used for the MAX7219 1088AS LED grids

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

  pinMode (reClk,INPUT);
  pinMode (reDat,INPUT);
  pinMode (reBtn,INPUT_PULLUP);
  for (int x=0; x<2; x++) pinMode (leverPin[x],INPUT);
  pinMode (execPin,INPUT_PULLUP);
  pinMode (hatchPin,INPUT_PULLUP);
  pinMode (relayPin,OUTPUT);
  pinMode (audioRxPin, INPUT);
  pinMode (audioTxPin, OUTPUT);

//============== HARDWARE SETUP ===============================//

  attachInterrupt(digitalPinToInterrupt(reClk), readDial, FALLING);

  grid.init();
  grid.setIntensity(6);
  
  statusLED.begin();
  statusLED.setBrightness(255);
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

//-------------- MANUAL RESET --------------------------------//

  int resetHold = 0;
  while (!digitalRead(reBtn)){
    resetHold++;
    delay(10);
    if (resetHold >= 500){
      tromStatus = 0;
      printText("XXXX",1);
      Serial.println("Manual Reset");
      dbts();
      delay(1000);
      break;
    }
  }
  
//-------------- UPDATE VARIABLES ----------------------------//
  
  hatchStt = false;                                           // assume the hatch is open
  if (!digitalRead(hatchPin)) hatchStt = true;                // if the latch button is pushed, it is closed
  if (hatchStt != hatchOld) somethingNew = true;              // if this is a change from the last cycle, flag it

  leverStt = 0;                                               // assume the locking lever is nowhere
  bool levN = digitalRead(leverPin[0]);                       // read the North pin of the PHES
  bool levS = digitalRead(leverPin[1]);                       // read the South pin of the PHES
  if      (levN && !levS) leverStt = 1;                       // if the north reads without the south, it is up
  else if (levS && !levN) leverStt = -1;                      // othrwise if the south reads without the north, it's down
  if (leverStt != leverOld) somethingNew = true;              // if this is a change from the last cycle, flag it

  execStt = false;                                            // assume that the execute button isn't pressed
  if (!digitalRead(execPin)) execStt = true;                  // if it is, mark it as such
  if (execStt != execOld) somethingNew = true;                // if this is a change from the last cycle, flag it
/*
 * NOTE:
 * The rotary encoder's input comes from the readDial interrupt function
 * which will update the 'dialStt' global byte variable
 */

//============== MAIN "GAME" FLOW ============================//

  if (inProc){

  tromStatus = 6;
    
    char printDig[4];
    String printNum;
    printNum=String(countdown);
    printNum.toCharArray(printDig,4);

    int firstCol;
    if (countdown < 10) firstCol = 13;
    else if (countdown < 100) firstCol = 8;
    else  firstCol = 3;
    
    printText(printDig,firstCol);
    printText("s",18);


    Serial.print ("Countdown: ");
    Serial.println(countdown);
    
    delay(1000);
    countdown--;
    grid.clear();
    if (countdown <= 0){
      printText("DONE",2);
      inProc = false;
      tromStatus = 1;
      delay (1500);
    }
  }

  switch (tromStatus){

//.............. Sleep Mode ..................................//
    case 0:                                                   // the TROM is in sleep mode
    
      grid.clear();                                           // clear the 1088 grids
      statusLED.setPixelColor(0,0);                           // write the NPX dark
      statusLED.show();                                       // execute the NPX updates
      digitalWrite(relayPin,LOW);                             // engage the hatch's maglock
      
      if (somethingNew){                                      // of any interface has been updated
        tromStatus++;                                         // advance the tromStatus (to wake up)
      }
      break;                                                  // END CASE 0
//.............. Lock & Unlock Mode ..........................//
    case 1:                                                   // the TROM is ready for loading

      printText("STBY ",2);                                    // print to the 1088 grids

      if (!hatchStt){
        statusLED.setPixelColor(0,100,100,0);                 // write the NPX to red
        statusLED.show();                                     // execute the NPX updates
      }

      else if (leverStt == -1){                                    // if the lever is down (lock)...
        statusLED.setPixelColor(0,0,200,0);                   // write the NPX to red
        statusLED.show();                                     // execute the NPX updates
        digitalWrite(relayPin,LOW);                           // engage the hatch's maglock
      }

      else if (leverStt == 1){                                // otherwise, if the lever is up (unlock)...
        statusLED.setPixelColor(0,200,0,0);                   // write the NPX to green
        statusLED.show();                                     // execute the NPX updates
        digitalWrite(relayPin,HIGH);                          // disengage the hatch's maglock
        if (execStt){
          grid.clear();
          printText("LOCK",2);
          delay(1500);
        }
      }

      if (hatchStt && leverStt == -1){
        int holdTime = 0;
        while (!digitalRead(execPin)){
          delay(10);
          holdTime++;
          if (holdTime >= 50){
            grid.clear();
            printText("SELC",2);
            delay(1500);
            tromStatus++;
            break;
          }
        }
      }
      break;

//.............. Select Mode .................................//
    case 2:

      settingReadout();

      int holdTime = 0;
      while (!digitalRead(execPin)){
        holdTime++;
        delay(10);
        if (holdTime >= 300){
          somethingNew = true;
          countdown = processTime[procSetting];
          playTrack(processSound[procSetting]);
          tromStatus=1;
          inProc = true;
          grid.clear();
          break;
        }
      }
      break;
  }

//-------------- CHECK SLEEP TIMER ---------------------------//

  if (millis() >= newInputTimeStamp + (sleepDelay * 60000)){
    tromStatus = 0;
  }

//============== ROUTINE MAINTAINENCE ========================//

  dbts();
  cycleReset();
}
