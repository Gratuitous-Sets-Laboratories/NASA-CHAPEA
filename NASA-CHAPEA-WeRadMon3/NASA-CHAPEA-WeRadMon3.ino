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
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
//.............. Identifier Data .............................//
  const String myNameIs = "NASA-CHAPEA-WeRadMon3";            // name of sketch
  const String versionNum = "1.0";                            // version of sketch
  const String lastUpdate = "2022 Sept 09";                   // last update

//.............. Game Tuning .................................//

  String settingName[4] = {"WIND","TEMP","PRES"," RAD "};


//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */

  #define loadPin     2           // parallel connection to all 74HC165 PISO shift registers, pin 1
  #define dataInPin   3           // serial connection to nearest 74HC165 PISO shift register, pin 9
  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)
  #define powerLED    10          // trigger pin for piezoelectric buzzer via 1K resistor & 2N2222 or 2N7000
  #define gridClk     A1
  #define gridCS      A2
  #define gridDat     A3

//-------------- CHARACTER SPECS  ----------------------------//

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
  2, 8, B00001000, B00001000, B00000000, B00000000, B00000000, // -
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

  #define numPISOregs 2

  #define gridsInUse 3
  MaxMatrix grid(gridDat,gridCS,gridClk,gridsInUse);

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
  byte buffer[10];

  float windNow = 0;
//  float windPrev;
  float tempNow = -80;
//  float tempPrev;
  float presNow = .1;
//  float presPrev;
  float radNow = 0;
//  float radPrev;
  
  byte PISOdata[numPISOregs];
  byte PISOprev[numPISOregs];
  byte controlMode;
  int wxDataType;
  byte dialSetting = 128;
  bool dialReady;
  bool displayData;
  uint32_t lastInputTime;
  uint32_t lastMinTick;

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

//-------------- HARDWARE SETUP -------------------------------//

  randomSeed(analogRead(A0));
  wxDataType = 1;
  genWx();
  wxDataType = 0;

  grid.init();
  grid.setIntensity(6);
  
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

  readPISO(0,1);                                              // read both PISO registers
  updateControlMode(1);                                       // check for a command from the PLC on the second register
  if (!controlMode) controlMode = 1;                          // default to basic operation

//-------------- MAD SCIENCE WEATHER CONTROL! ----------------//

  if (millis() >= lastMinTick + 60000){                       // if a minute has elapsed since the last weather fluctuiation...
    genWx();                                                  // generate new weather data (meniacal laugh)
    dbts();                                                   // report to Serial monitor
    lastMinTick = millis();                                   // update the timestamp
  }

//-------------- SLEEP MODE ----------------------------------//

  if (!wxDataType){                                           // if there is no data type selected...
    
    grid.clear();                                             // clear the grid
    digitalWrite(powerLED,LOW);                               // turn the power button off

//.............. Power Button Wake ...........................//
    int holdTime = 0;                                         // reset hold time               
    while(bitRead(PISOdata[0],0) == 0){                       // while the power button is held down...
      holdTime++;                                             // incriment the counter
      delay (10);                                             // wait 0.01 sconds 
      if (holdTime >= 50){                                    // if it's been 1/2 second...
        wxDataType++;                                         // make data type non-zero
        printText("Hello",1);                                 // be friendly
        delay(1000);                                          // wait a sec
        grid.clear();                                         // clear the screen
        printText("STBY",2);
        break;                                                // exit the while loop
      }
      readPISO(0,0);                                          // re-read the first PISO register
    }
  }

//-------------- ACTIVE MODE ---------------------------------//

  else {                                                      // otherwise (there IS a selected weather data type)...
    digitalWrite(powerLED,HIGH);                              // illuminate the power button

//.............. Ersatz Rotary Encoder .......................//
                                                    
    readPISO(0,0);
    bool reBtn = bitRead(PISOdata[0],3);
    while (!reBtn){
      ersatzRotaryEncoder();
      reBtn = bitRead(PISOdata[0],3);
    }

//.............. Display .....................................//

    if (bitRead(PISOdata[0],4) == 0){
      displayData = true;
//      genWx();
      lastInputTime = millis();
      grid.clear();
    }
    
    if (displayData){                                         // otherwise it's data time!..
      showMeTheDigits();
    }
  
//.............. Go The F*ck to Sleep ........................//

    int holdTime = 0;                                         // reset hold time               
    while(bitRead(PISOdata[0],0) == 0){                       // while the power button is held down...
      holdTime++;                                             // incriment the counter
      delay (10);                                             // wait 0.01 sconds 
      if (holdTime >= 50){                                    // if it's been 1/2 second...
        wxDataType = 0;                                       // set data mode to sleepy
        displayData = false;
        printText("Sleep",0);                                 // be friendly
        delay(1500);                                          // wait a sec
        grid.clear();                                         // clear the screen
        break;                                                // exit the while loop
      }
      readPISO(0,0);                                          // re-read the first PISO register
    }

    if (millis() >= lastInputTime + 300000){                  // if it's been 5 minutes since an input...
      wxDataType = 0;                                         // go to sleep
      displayData = false;
      printText("Sleep",0);
      delay(1500);
      grid.clear();
    }
  }
//------------------------------------------------------------//

  dbts();
  cycleReset();

}
