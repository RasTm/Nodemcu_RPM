#include "Struct.h"
#include "isr.h" 
#include "Functions.h"
//#include "Web.h"

void setup() {
  Serial.begin(115200);
  EEPROM.begin(3);   

  EEPROM.put(0,0);
  EEPROM.put(1,0);
  EEPROM.put(2,0); 
  EEPROM.commit();

  pinMode(button_1.PIN, INPUT_PULLUP);
  pinMode(button_2.PIN, INPUT_PULLUP);
  pinMode(pulse_1.PIN , INPUT);
  
  attachInterrupt(digitalPinToInterrupt(button_1.PIN), button_1_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_2.PIN), button_2_isr, FALLING);
  attachInterrupt(pulse_1.PIN, pulse_1_isr, RISING);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  check_calibration();
  if(pulse_1.calibration_done == false){
    no_calib_led();
  }
}

void loop() {
  calibration();
  check_pulse();
  //Serial.println(pulse_1.period);
}