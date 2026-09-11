/**
 * @file main.cpp
 * @details Main code, what will be uploaded to microcontroller. This is
 *          for the reciever. It will do the following:
 * 
 *  1. Receive Nmea sentences from a transmitter of save radio
 *     (data is transmitted 1 byte at a time so we will need to
 *     contruct the sentence or any data received)
 * 
 *  2. Save sentences to an SD card
 * ---------------------------------------------------------------------
 * TO DO:  
 * X = need to do | O = done
 * 
 * [O] Code reciever
 * [O] Code sd
 * [O] Check if Compiles
 * [X] Upload (gives error)
 * [X] Test reciever
 * [X] Test sd
 * [X] Adjust buffer size to be larger if possible
 * 
 * Note: try changing board if all troubleshooting steps dont work
 * ---------------------------------------------------------------------
 * MICROCONTROLLER: 
 *  - Raspberry Pi Pico
 *  https://docs.platformio.org/en/latest/boards/raspberrypi/pico.html#board-raspberrypi-pico
 * 
 * SENSORS:
 *  - Radio DRF4463D20 (transmitter should have same radio)
 *  - Adafruit SD Card Breakout (with SD)
 * ---------------------------------------------------------------------
 * CONNECTIONS:
 *  NANO  | RADIO
 *    D4  |   RX  / 4th from bottom
 *    D5  |   TX  / 5th from bottom
 *    GND |   GND / 1st from bottom
 *    VCC |   VCC / 2nd from bottom
 * Note: ask why we aren't using nano TX, RX pins
 * 
 *  NANO  | SD
 *    GND |   GND
 *    5V  |   5V
 *    D13 |   CLK
 *    D12 |   DO / 4th from top
 *    D11 |   DI / 3rd from top
 *    D10 |   CS / 2nd from top
 * ---------------------------------------------------------------------
 * LIBRARIES:
 * 
 *  - Recieving
 *      https://docs.arduino.cc/learn/built-in-libraries/software-serial/
 * 
 *  - SD:
 *      https://docs.arduino.cc/libraries/sd/
 */
#ifndef MAIN_CPP
#define MAIN_CPP
/***********************************************************************
 * LIBRARIES
 **********************************************************************/
#include <Arduino.h>  // To make it compatible for Platform.IO
                      // delete if you're using Arduino IDE

#include <SoftwareSerial.h>   // for recieving
// #include <SPI.h>              // SD interface
#include <SD.h>               // SD card

/***********************************************************************
 * CONSTANTS NOT TO CHANGE : pins, serials etc.
 * Things that shoud NOT be changed unless you know what you're doing!
 **********************************************************************/
// PINS, DO NOT CHANGE UNLESS YOU CHANGE WIRING
#define RADIO_RX 4    // nano D-pin connected to radio RX pin
#define RADIO_TX 5    // nano D-pin connected to radio TX pin

#define SD_PIN 10      // nano D-pin connected to SD breakout

#endif

/***********************************************************************
 * VARAIBLES TO CHANGE : rates, serials, etc.
 * Things that we might need to change
 **********************************************************************/
// SIZES: size = send rate (second) * read rate (Hz)
const uint8_t SIZE_BUFFER_RAM = 10;   // number of sentences saved
const uint8_t SIZE_NMEA_LEN = 120;    // (max) length of nmea sentences

// RATES: rate (millis) = 1000 millis /  times to run per second
const unsigned long RATE_SD = 1000;          // 1Hz, save to sd

/***********************************************************************
 * CONTANTS AND VARIABLES TO NOT MODIFY
 **********************************************************************/
// SENSOR OBJECTS
SoftwareSerial Radio(RADIO_RX, RADIO_TX);   // radio 

// COUNTER
uint8_t counter_ram = 0;    // index for ram buffer
uint8_t indexSave = 0;      // index for saving to sd
uint8_t indexCurrNmea = 0;  // index for nmea sentence we are constructing

// VARIABLES FOR KEEPING TRACK OF TIME
unsigned long currentTime;    // current time, from when sensor was turned on
unsigned long lastSDSave;     // last time data was saved to sd

File fileNmea;                    // file to save nmea sentences to

/***********************************************************************
 * BUFFERS
 **********************************************************************/
// Buffer for all sensors
char nmeaSentence[SIZE_BUFFER_RAM][SIZE_NMEA_LEN]; // nmea sentence we are recieving in pieces

/***********************************************************************
 * setup() : CODE THAT RUNS ONCE
 * ---------------------------------------------------------------------
 * start USB Serial
 * Start radio serial
 * Initialize SD
 * Open/create NMEA file
 **********************************************************************/
void setup() {
  // BAUD RATES
  Serial.begin(115200);     // computer / testing
  Radio.begin(9600);        // radio

  /*********************************************************************
   * IF SD NOT CONNECTED
   ********************************************************************/
  if (!SD.begin(SD_PIN)) {
    Serial.println("SD initialization failed!");
  } // END if (!SD.begin(SD_PIN))
  /*********************************************************************
   * IF SD IS CONNECTED
   ********************************************************************/
  else {
    Serial.println("SD ready :D");
  } // END else

  // CSV FILE CRATED OR OPENED 
  fileNmea = SD.open("nmea.csv", FILE_WRITE);

  // DEBUGG: Check if file can open
  if (!fileNmea) {
      Serial.println("Failed to open nmea.csv!");
  } // END if (!fileNmea)
  
  // DELAY, milli seconds (allow sensors to )
  delay(300);

} // END setup() -------------------------------------------------------

/***********************************************************************
 * loop() : CODE THAT WILL RUN CONTINUOUSLY
 * Make faster tasks first and slower tasks last
 * ---------------------------------------------------------------------
 * recieves nmea sentences from transmitter
 * save to sd
 **********************************************************************/
void loop() {
  currentTime = millis();

  /*********************************************************************
   * RECIEVING DATA FROM TRANSMITTER
   * -------------------------------------------------------------------
   * will continue to run until theres nothing else to recieve from
   * transmitter
   ********************************************************************/
  while (Radio.available()) {
    char incomingByte = Radio.read(); // byte recieved from radio

    // BUILD SENTENCE
    if (indexCurrNmea < SIZE_NMEA_LEN - 1) {
      nmeaSentence[counter_ram][indexCurrNmea] = incomingByte;
      indexCurrNmea++;
    } // END if (indexCurrNmea < SIZE_NMEA_LEN - 1) 

    /*******************************************************************
     * SAVE FULLY CONSTRUCTED SENTENCE
     * -----------------------------------------------------------------
     * if nmea sentence is fully constructed (ends with /n) we save it
     * to buffer
     ******************************************************************/
    if (incomingByte == '\n') {

      // ADD NULL TERMINATOR
      nmeaSentence[counter_ram][indexCurrNmea] = '\0';

      // INCREMENT BUFFER COUNTER
      counter_ram++;

      // RESET FOR NEXT SENTENCE 
      indexCurrNmea = 0;
        
      // DEBUGGING (hide): OUTPUT WHAT WAS RECIEVED TO COMPUTER
      Serial.print("Recieved: ");
      Serial.println(nmeaSentence[counter_ram][indexCurrNmea]);

    } // END if (incomingByte == '\n')
  } // END while (Radio.available()) 

  /*********************************************************************
   * SAVING TO SD
   * -------------------------------------------------------------------
   * if its time to save OR if our buffer is full
   ********************************************************************/
  if (currentTime - lastSDSave >= RATE_SD || counter_ram >= SIZE_BUFFER_RAM) {

    // DEBUGGING (hide): OUTPUT HOW MANY WE READ
    Serial.print("Number of sentences recieved: ");
    Serial.println(counter_ram);

    /*******************************************************************
     * SAVE ALL BUFFER CONTENTS TO SD CARD
     ******************************************************************/
    while (indexSave < counter_ram) {

      // DEBUGGING (hide): OUTPUT IF WE ARE SAVING AND THE TIME
      Serial.print("Saving at: ");
      Serial.println(currentTime);

      size_t saveNmea = 0;
      // SAVE TO SD
      while (nmeaSentence[indexSave][saveNmea] != '\0') {  
        fileNmea.print(nmeaSentence[indexSave][saveNmea]);

        // DEBUGGING (hide): OUTPUT WHAT WE ARE SAVING
        Serial.println(nmeaSentence[indexSave][saveNmea]);

        // move to next character
        saveNmea++;
      } // END while

      // INCREMENT COUNTER
      indexSave++;

    } // END while (indexSave < SIZE_BUFFER_RAM)

    // FLUSH SD (makes sure the data buffered by the SD library gets written out)
    fileNmea.flush();
    
    // REST COUNTERS
    counter_ram = 0;
    indexSave = 0;
    lastSDSave = currentTime;     // updates last saved time

    // DEBUGGING (hide): ENSURE THINGS GOT UPDATED
    Serial.print("Last time saved: ");
    Serial.println(lastSDSave);

  } // END if (currentTime - lastSDSave >= RATE_SD)

} // END loop() --------------------------------------------------------

/***********************************************************************
 * FUNCTION DEFINITIONS
 **********************************************************************/