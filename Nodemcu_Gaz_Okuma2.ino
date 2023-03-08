#include "Struct.h"
#include "isr.h" 
#include "Functions.h"

void setup() {
  Serial.begin(115200);
  EEPROM.begin(3);   

  pinMode(button_1.PIN, INPUT_PULLUP);
  pinMode(button_2.PIN, INPUT_PULLUP);
  pinMode(pulse_1.PIN , INPUT);
  
  attachInterrupt(digitalPinToInterrupt(button_1.PIN), button_1_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(button_2.PIN), button_2_isr, FALLING);
  attachInterrupt(pulse_1.PIN, pulse_1_isr, RISING);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

  check_calibration();
}

void loop() {
  calibration();
//  Serial.println(pulse_1.period);
}