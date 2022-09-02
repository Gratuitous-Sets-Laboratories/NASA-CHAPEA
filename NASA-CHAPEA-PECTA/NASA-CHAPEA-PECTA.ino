

/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA PECTA (Portable Emergency Communications & Telemetry Array)
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
  const String myNameIs = "NASA-CHAPEA-PECTA: Aug 2022";    // nametag for Serial monitor setup
  
//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */
//.............. Digital Pins ................................//

  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)
  #define relay1Pin   8
  #define relay2Pin   9
  #define powerLED    10          
  #define powerPin    11
  #define cablePin    12
  #define visSigPin   13
  #define masterPin   A7

//-------------- HARDWARE PARAMETERS -------------------------//

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
  
  bool powerOn;
  bool txIsGo;
  bool rxIsGo;

  bool somethingNew = true;
  bool txOld;
  bool rxOld;

  byte blinkLEDs;

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
  pinMode (latchPin, OUTPUT);
  pinMode (dataOutPin, OUTPUT);
//.............. Other Pin Modes .............................//
  pinMode (powerPin, INPUT_PULLUP);
  pinMode (cablePin, INPUT_PULLUP);
  pinMode (masterPin, INPUT_PULLUP);
  pinMode (powerLED, OUTPUT);
  pinMode (relay1Pin, OUTPUT);
  pinMode (relay2Pin, OUTPUT);
  pinMode (visSigPin, OUTPUT);

//-------------- HARDWARE SETUP -------------------------------//

  randomSeed(analogRead(A0));
  
//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();
  dbts();

}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {

  if (!digitalRead(masterPin)) digitalWrite(visSigPin,HIGH);
  else digitalWrite(visSigPin,LOW);

  if (!powerOn){

    txIsGo = false;
    rxIsGo = false;
    digitalWrite(relay1Pin,LOW);
    digitalWrite(relay2Pin,LOW);

    digitalWrite(powerLED,LOW);
    sendSIPO(0);
    pulsePin(latchPin,10);

    int holdTime = 0;
    while (!digitalRead(powerPin)){
      holdTime++;
      delay(10);
      if (holdTime >= 50){
        powerOn = true;
        somethingNew = true;
        bootAnimation();
        break;
      }
    }
  }

  else {

    digitalWrite(relay1Pin, HIGH);
    digitalWrite(powerLED,HIGH);
    bitWrite(blinkLEDs,0,1);

    txIsGo = false;
    if (!digitalRead(cablePin)){
      txIsGo = true;
    }

    if (!digitalRead(masterPin) && txIsGo){
      rxIsGo = true;
    }

    if (rxIsGo){
      bitWrite(blinkLEDs,2,blinkOrNo(2,5));
    }
    else {
      bitWrite(blinkLEDs,2,0);
    }

    if (txIsGo){
      digitalWrite(relay2Pin,HIGH);
      bitWrite(blinkLEDs,1,blinkOrNo(3,5));
    }
    else {
      digitalWrite(relay2Pin,LOW);
      bitWrite(blinkLEDs,1,0);
      bitWrite(blinkLEDs,2,0);
    }
    
    sendSIPO(blinkLEDs);
    pulsePin(latchPin,10);
  }

    int holdTime = 0;
    while (!digitalRead(powerPin)){
      holdTime++;
      delay(10);
      if (holdTime >= 50){
        powerOn = false;
        somethingNew = true;
        shutdownAnimation();
        break;
      }
    }
    
  dbts();
  cycleReset();
}

bool blinkOrNo(int dif, int die){
  long roll = random(1,die+1);
  if (roll >= dif) return true;
  else return false;
}

void bootAnimation(){
  for (int x = 0; x < 6; x++){
    sendSIPO(1);
    pulsePin(latchPin,10);
    delay(500);
    sendSIPO(0);
    pulsePin(latchPin,10);
    delay(500);
  }
}

void shutdownAnimation(){
  for (int x = 0; x < 3; x++){
    sendSIPO(7);
    pulsePin(latchPin,10);
    delay(500);
    sendSIPO(0);
    pulsePin(latchPin,10);
    delay(500);
  }
  digitalWrite(powerLED,LOW);
}
