#define NUM_LEDS 14
#define BUTTON_DELAY 350
#define LED_PIN 4                         //D2
#define CALIB_MEASURE_COUNT 300

uint8_t last_save = 0;
uint8_t EEPROM_data[4] = {0,0,0,0};
uint8_t led_on_count;
CRGB leds[NUM_LEDS];
CRGB color;

Button_General button_1 {14};             //D5
Button_General button_2 {12};             //D6

Pulse_Counter_General pulse_1 {5};        //D1
