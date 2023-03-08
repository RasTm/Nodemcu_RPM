void no_calib_led(){
  if(pulse_1.calibration_done == false){
    for(uint8_t i = 0; i<NUM_LEDS; i++){                  
      leds[i] = CRGB::White;
    }
    FastLED.show();
  }
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

void check_calibration(){
  for(uint8_t i = 0; i<3; i++){
    EEPROM.get(i, EEPROM_data[i]);
    Serial.println(EEPROM_data[i]);
    delay(200);
  }
  pulse_1.max_period = EEPROM_data[0];
  pulse_1.min_period = EEPROM_data[1];
  if(EEPROM_data[2] == 1) pulse_1.calibration_done = true;
  else pulse_1.calibration_done = false;

  no_calib_led();

  Serial.printf("min = %d\n max = %d\n done = %d\n",pulse_1.min_period,pulse_1.max_period,pulse_1.calibration_done);
}

