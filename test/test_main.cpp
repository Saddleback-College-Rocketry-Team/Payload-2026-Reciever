/**
 * @file test_main.cpp
 * @brief file that will be to test things
 * @date 2026-03-07
 * 
 */
#ifndef MAIN_CPP
#define MAIN_CPP
#include <Arduino.h>
#include <SoftwareSerial.h>   // for recieving

#define RADIO_RX 4    // nano D-pin connected to radio RX pin
#define RADIO_TX 5    // nano D-pin connected to radio TX pin

#endif
SoftwareSerial Radio(RADIO_RX, RADIO_TX);   // radio 

/**
 * @brief Arduino-style test runner.
 */
void setup() {
    // BAUD RATES
  Serial.begin(115200);     // computer / testing
  Radio.begin(9600);        // radio

  // DELAY, milli seconds (allow sensors to config)
  delay(300);
}

void loop() {
    Serial.println(Radio.read());
}
