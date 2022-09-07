/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA Soil & Water Processor (Sled)
 * Johnson Space Center, TX, USA
 * 
 * MABOB I (1.0) architecture
 */

//============== DEFINITIONS & DECLAIRATIONS =================//

//-------------- LIBRARIES -----------------------------------//
/* Call all function libraries required by the sketch. 
 */
  
//-------------- SETTINGS & GLOBAL CONSTANTS -----------------//
/* Define constraits used by various functions.
 * Variables using '#define' are defined by hardware, and should be left alone.
 * Variables using 'const' can be changed to tune the puzzle.
 */
 
//.............. Identifier Data .............................//
  const String myNameIs = "NASA-CHAPEA-SW2";                 // name of sketch
  const String versionNum = "Beta";                            // version of sketch
  const String lastUpdate = "2022 Sept 06";                   // last update

  #define numPISOregs 2
  
//.............. Game Tuning Factors .........................//

//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */
  
  #define loadPin     2           // parallel connection to all 74HC165 PISO shift registers, pin 1
  #define dataInPin   3           // serial connection to nearest 74HC165 PISO shift register, pin 9
  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)
  #define audioTxPin 11           // data pin for DF Player Mini, pin 11
  #define audioRxPin 12           // data pin for DF Player Mini, pin 10

//============== HARDWARE PARAMETERS =========================//
                                                            

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
//.............. Raw Input Data ..............................//
  byte PISOdata[numPISOregs];
  byte PISOprev[numPISOregs];
  bool somethingNew;

  byte counter;

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

  pinMode (clockPin,OUTPUT);
  pinMode (latchPin,OUTPUT);
  pinMode (loadPin,OUTPUT);
  pinMode (dataInPin,INPUT);
  
  pinMode (dataOutPin,OUTPUT);     


//-------------- HARDWARE PARAMETERS -------------------------//
  
//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();
}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {
  
//  Serial.println(counter);
  sendSIPO(0);
  sendSIPO(counter);
  pulsePin(latchPin,10);
  counter++;
  delay(100);
  
  readPISO(0,1);

  for (int reg = 0; reg < numPISOregs; reg++){
    if (PISOdata[reg] != PISOprev[reg]){
      somethingNew = true;
    }
  }

  dbts();
  cycleReset();
 
}
