void no_calib_led(){
  for(uint8_t i = 0; i<NUM_LEDS; i++){
    if(i%2 == 0){
      leds[i] = CRGB::Black;
    }
    else{
      leds[i] = CRGB::Blue;
    }
  }
  FastLED.show(); 
}

void calibration(){
  if(button_1.pressed){
    button_1.pressed = false;
    pulse_1.calibration_done = false;

    uint8_t period_arr[CALIB_MEASURE_COUNT] = {0};
    uint8_t last_period, new_period;
    uint16_t i = 0;

    no_calib_led();

    Serial.println("kalibrasyon");

    do {
      new_period = pulse_1.period;
      if(last_period != new_period){
        period_arr[i] = new_period;
        Serial.printf("Arr %d. = %d --------- period = %d\n",i,period_arr[i],new_period);
        i = i+1;
        last_period = new_period;
        yield();
      }
      yield();  
    }while(i<CALIB_MEASURE_COUNT);

    //Serial.println("done");
    
    //for (int i = 0; i < CALIB_MEASURE_COUNT; i++){ Serial.println(period_arr[i]); }
  
    //Serial.print("--------------------------------\n");
  
    sortArray(period_arr,CALIB_MEASURE_COUNT);

    //for (int i = 0; i < CALIB_MEASURE_COUNT; i++){ Serial.println(period_arr[i]); } 

    pulse_1.max_period = period_arr[CALIB_MEASURE_COUNT-1];
    pulse_1.min_period = period_arr[1];

  Serial.printf("-----------------------\n Max = %d\n Min = %d", pulse_1.max_period,pulse_1.min_period);
  pulse_1.calibration_done = true;

  EEPROM.put(0,pulse_1.max_period);
  EEPROM.put(1,pulse_1.min_period);
  EEPROM.put(2,pulse_1.calibration_done); 
  EEPROM.commit();
  }
}

void check_pulse(){
  pulse_1.last_period = pulse_1.period;
  if(pulse_1.period == pulse_1.last_period){
    pulse_1.x += 1;
    if(pulse_1.x > 20 && (pulse_1.period < pulse_1.min_period || pulse_1.period == 0)){
      for(uint8_t i = 0; i<NUM_LEDS; i++){                  
        leds[i] = CRGB::White;
      }
      FastLED.show();
    }
  }
}

void check_calibration(){
  for(uint8_t i = 0; i<4; i++){
    EEPROM.get(i, EEPROM_data[i]);
  }
  pulse_1.max_period = EEPROM_data[0];
  pulse_1.min_period = EEPROM_data[1];
  if(EEPROM_data[2] == true){
    pulse_1.calibration_done = true;
  }
  button_2.press_num= EEPROM_data[3];
  if(button_2.press_num == 0){
    color = CRGB::Red;
  }
  else if(button_2.press_num == 1){  
    color = CRGB::Green;
  }
  else if(button_2.press_num == 2){
    color = CRGB::Blue;
  }
  else if(button_2.press_num == 3){
    color = CRGB::Aqua;
  }
  else if(button_2.press_num == 4){  
    color = CRGB::Orange;
  }
  else if(button_2.press_num == 5){ 
    color = CRGB::Gray;
  }
  else if(button_2.press_num == 6){
    color = CRGB::Gold;
  }
  else if(button_2.press_num == 7){
    color = CRGB::Magenta;
  } 
  else if(button_2.press_num == 8){
    color = CRGB::DeepPink;
  }
  else if(button_2.press_num == 9){
    color = CRGB::DarkViolet;
  }
  Serial.printf("\n min = %d\n max = %d\n done = %d\n color = %d",pulse_1.min_period,pulse_1.max_period,pulse_1.calibration_done,button_2.press_num);
}

void save_color(){
  if(last_save != button_2.press_num){
    last_save = button_2.press_num;
    EEPROM.put(3,button_2.press_num);
    EEPROM.commit(); 
  }
}