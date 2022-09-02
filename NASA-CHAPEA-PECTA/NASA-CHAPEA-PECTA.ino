/*
 * GRATUITOUS SETS LABORATORIES
 * Dallas, TX, USA
 * 
 * NASA CHAPEA PECTA (Portable Emergency Communications & Telemetry Array)
 * Johnson Space Center, TX, USA
 * 
 * MABOB I (1.0) architecture
 * 
 * NOTES:
 * This version is functionally the same as the one marked Aug 2022
 * and uploaded to the PECTA,
 * I've just added comments for clarity.
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
  const String myNameIs = "NASA-CHAPEA-PECTA";                // name of sketch
  const String versionNum = "1.0";                            // version of sketch
  const String lastUpdate = "2022 Sept 01";                   // last update
  
//-------------- PIN DEFINITIONS  ----------------------------//
/* Most of the I/O pins on the Arduino Nano are hard-wired to various components on the MABOB.
 * Pins not used for their standard fuction have header pins for alternate uses.
 */

  #define latchPin    4           // parallel connection to all 74HC595 SIPO shift registers, pin 12
  #define dataOutPin  6           // serial connection to nearest 74HC595 SIPO shift register, pin 14
  #define clockPin    7           // parallel connection to all shift registers (74HC165 pin 2 / 74HC595 pin 11)
  #define relay1Pin   8           // relay to communitate to PLC
  #define relay2Pin   9           // relay to communitate to PLC
  #define powerLED    10          // 12V light on power button, uses piezo-buzzer transistor
  #define powerPin    11          // power button momentary switch, active LOW
  #define cablePin    12          // antenna cable "switch" shorts when plugged into both ends, active LOW
  #define visSigPin   13          // onboard LED
  #define masterPin   A7          // input signal from PLC, active LOW

//-------------- HARDWARE PARAMETERS -------------------------//

//-------------- GLOBAL VARIABLES ----------------------------//
/* Decrlare variables used by various functions.
 */
  
  bool powerOn;                   // PECTA is on or off, controlled by power button
  bool txIsGo;                    // PECTA can "transmit" (antenna is plugged in)
  bool rxIsGo;                    // PECTA can "receive" (control has approved alignment via PLC)

  bool somethingNew = true;       // something has changed in the current cycle of the void loop
  bool txOld;                     // previous cycle's txIsGo value
  bool rxOld;                     // previous cycle's rxIsGo value

  byte blinkLEDs;                 // byte to send to the SIPO register for the PECTA's indicator LEDs (doesn't need to be global)

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
  pinMode (latchPin, OUTPUT);
  pinMode (dataOutPin, OUTPUT);
//.............. Other Pin Modes .............................//
  pinMode (powerPin, INPUT_PULLUP);                           // all input pins are active LOW
  pinMode (cablePin, INPUT_PULLUP);
  pinMode (masterPin, INPUT_PULLUP);
  pinMode (powerLED, OUTPUT);
  pinMode (relay1Pin, OUTPUT);
  pinMode (relay2Pin, OUTPUT);
  pinMode (visSigPin, OUTPUT);

//-------------- HARDWARE SETUP -------------------------------//

  randomSeed(analogRead(A0));                                  // generate a (pseudo)random seed by reading a foating analog pin
  
//-------------- A/V FEEDBACK --------------------------------//

  Serial.println("Setup complete.");
  Serial.println();
  dbts();                                                     // give an initial report through the Serial monitor

}

//============================================================//
//============== LOOP ========================================//
//============================================================//

void loop() {

//.............. Visual Troubleshooting Feedback .............// This would be more useful if it were part of the hardware.

  if (!digitalRead(masterPin)) digitalWrite(visSigPin,HIGH);  // if the masterPin from the PLC is low, light the onboard LED
  else digitalWrite(visSigPin,LOW);                           // otherwise turn it off

//-------------- WHEN PECTA IS OFF ---------------------------//

  if (!powerOn){                                              // if the PECTA is off...
//.............. Set Variables & Indicators ..................//
    txIsGo = false;                                           // transmit is off
    rxIsGo = false;                                           // receive is off
    digitalWrite(relay1Pin,LOW);                              // relay 1 (indicate PWR to PLC) is off
    digitalWrite(relay2Pin,LOW);                              // relay 2 (indicate TX to PLC) is off
    digitalWrite(powerLED,LOW);                               // power button's LED is off
    sendSIPO(0);                                              // all 3 indicator LEDs are set to off
    pulsePin(latchPin,10);                                    // latch that byte through the SIPO register
//.............. Read Power Button ...........................//
    int holdTime = 0;                                         // reset the button's hold time
    while (!digitalRead(powerPin)){                           // while the button is being held down (active LOW)...
      holdTime++;                                             // incriment the hold time
      delay(10);                                              // wait 10 milliseconds
      if (holdTime >= 50){                                    // if the hold time incriments to at least 50 (1/2 second)...
        powerOn = true;                                       // the power is now on
        somethingNew = true;                                  // something has changed
        bootAnimation();                                      // trigger the boot-up animation
        break;                                                // break out of the while loop
      }
    }
  }

//-------------- WHEN PECTA IS ON ----------------------------//

  else {                                                      // otherwise (if it's not off, it's gotta be on)...
//.............. Set Variables & Indicators ..................//
    digitalWrite(relay1Pin, HIGH);                            // close the relay to tell the PLC that PECTA is on
    digitalWrite(powerLED,HIGH);                              // light the button's LED
    bitWrite(blinkLEDs,0,1);                                  // write the PWR indicator LED to be steady on
//.............. Check for TX/RX Conditions ..................//
    txIsGo = false;                                           // assume that the antenna cable is not circuited
    if (!digitalRead(cablePin)) txIsGo = true;                // if it is (active LOW) raise the transmit boolean flag
    
    if (!digitalRead(masterPin) && txIsGo){                   // if control approves of the antenna's aim AND it's hooked up...
      rxIsGo = true;                                          // we are go to receive data!
    }
//.............. Make With Blinky Lights .....................//
    if (rxIsGo){                                              // if we're receiving...
      bitWrite(blinkLEDs,2,blinkOrNo(2,5));                   // give the RX light a 2:5 chance of being lit (for this loop)
    }
    else {                                                    // otherwise...
      bitWrite(blinkLEDs,2,0);                                // the RX light is off
    }

    if (txIsGo){                                              // if we're transmitting...
      digitalWrite(relay2Pin,HIGH);                           // close the relay to tell the control PLC
      bitWrite(blinkLEDs,1,blinkOrNo(3,5));                   // give the TX light a 3:5 chance of being lit (for this loop)
    }
    else {                                                    // otherwise...
      digitalWrite(relay2Pin,LOW);                            // open the rleay to tell the control PLC
      bitWrite(blinkLEDs,1,0);                                // set the TX indicator to off
      bitWrite(blinkLEDs,2,0);                                // set the RX indicator to off (overriding the previous setting)
    }
    
    sendSIPO(blinkLEDs);                                      // send the current blinky light data to the SIPO register
    pulsePin(latchPin,10);                                    // latch that byte through the SIPO register
  }
//.............. Read Power Button ...........................//
    int holdTime = 0;                                         // reset the button's hold time
    while (!digitalRead(powerPin)){                           // while the button is being held down (active LOW)...
      holdTime++;                                             // incriment the hold time
      delay(10);                                              // wait 10 milliseconds
      if (holdTime >= 50){                                    // if the hold time incriments to at least 50 (1/2 second)...
        powerOn = false;                                      // the power is now off
        somethingNew = true;                                  // something has changed
        shutdownAnimation();                                  // trigger the boot-up animation
        break;                                                // break out of the while loop
      }
    }
//-------------- ROUTINE MAINTAINENCE ------------------------//
    
  dbts();                                                     // report relevent info via the Serial monitor (at 19200 baud)
  cycleReset();                                               // end of cycle maintainence (variable resets, cycle delay, etc)
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

//============== BOOT UP ANIMATION ===========================//
/*
 * This will blink the PWR indicator light 5 times
 * before returning to the void loop's main ON sequense
 */
void bootAnimation(){
  for (int x = 0; x < 5; x++){
    sendSIPO(1);
    pulsePin(latchPin,10);
    delay(500);
    sendSIPO(0);
    pulsePin(latchPin,10);
    delay(500);
  }
}

//============== SHUT DOWN ANIMATION =========================//
/*
 * This will flash all 3 LED indicators 3 times
 * before returning to the void loop's main OFF sequense
 */
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
