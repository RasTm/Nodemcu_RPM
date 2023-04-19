#include <ArduinoSort.h>
#include <FastLED.h>
#include <ESP_EEPROM.h>

#include "Struct.h"
#include "Config.h"
#include "isr.h" 
#include "Functions.h"

void setup() {
  Serial.begin(115200);
  EEPROM.begin(4);   

  pinMode(button_1.PIN, INPUT_PULLUP);
  pinMode(button_2.PIN, INPUT_PULLUP);
  pinMode(pulse_1.PIN , INPUT);
  
  attachInterrupt(digitalPinToInterrupt(button_1.PIN), button_1_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_2.PIN), button_2_isr, FALLING);
  attachInterrupt(pulse_1.PIN, pulse_1_isr, RISING);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  check_calibration();
  if(pulse_1.calibration_done == false){
    no_calib_led();
  }
}

void loop() {
  calibration();
  check_pulse();
  save_color();
  //Serial.println(pulse_1.period);
}
