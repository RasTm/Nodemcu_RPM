//General button structure
struct Button_General{
  const uint8_t PIN;
  uint8_t press_num = 0;;                       //Save eeprom
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
