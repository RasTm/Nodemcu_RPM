#include <ArduinoSort.h>
#include <FastLED.h>
#include <ESP_EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define NUM_LEDS 12
#define BUTTON_DELAY 250
#define LED_PIN 4                         //D2
#define CALIB_MEASURE_COUNT 300

uint8_t EEPROM_data[3] = {0,0,0};
uint8_t led_on_count;
CRGB leds[NUM_LEDS];

const char *ssid = "RPM_Meter_V1";
const char *password = "12345678";

//General button structure
struct Button_General{
  const uint8_t PIN;
  uint16_t press_num = 0;
  unsigned long press_time = 0;
  unsigned long last_press_time = 0;
  bool pressed = false;
};

//General pulse counter structure
struct Pulse_Counter_General{
  const uint8_t PIN;
  uint8_t period;
  uint8_t last_period;
  uint8_t min_period;                           //Save eeprom
  uint8_t max_period;                           //Save eeprom
  uint8_t x = 0;
  unsigned long pulse_time_start;
  bool i = false;
  bool calibration_done = false ;               //Save eeprom
};

Button_General button_1 {14};             //D5
Button_General button_2 {12};             //D6

Pulse_Counter_General pulse_1 {5};        //D1