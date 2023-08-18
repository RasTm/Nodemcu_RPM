void ICACHE_RAM_ATTR button_1_isr(){          //Calibration
  if((millis() - button_1.last_press_time) > BUTTON_DELAY){
    button_1.last_press_time = millis();
    button_1.pressed = true;
  } 
}

void ICACHE_RAM_ATTR button_2_isr(){         //Colour
  if((millis() - button_2.last_press_time) > BUTTON_DELAY){
    button_2.last_press_time = millis();
    button_2.press_num += 1;
    button_2.pressed = true;
  }
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
  else if(button_2.press_num > 9){
    button_2.press_num = 0;
  }
}

void ICACHE_RAM_ATTR pulse_1_isr(){
  if(pulse_1.i == false){
    pulse_1.pulse_time_start = millis();
    pulse_1.i = true;
  }
  else{
    pulse_1.period = millis() - pulse_1.pulse_time_start;
    pulse_1.i = false;

    if(pulse_1.calibration_done == true){
      led_on_count = map(pulse_1.period, pulse_1.min_period, pulse_1.max_period, NUM_LEDS, 0);

      if(pulse_1.period == pulse_1.min_period || pulse_1.period == pulse_1.min_period-1){         //LED Flash Start
        for(uint8_t i = 0; i<NUM_LEDS; i++){                  
          leds[i] = color;
        }
        FastLED.show();
        delay(25);
        for(uint8_t i = 0; i<NUM_LEDS; i++){                  
          leds[i] = CRGB::Black;
        }
        FastLED.show();
        delay(25);
      }                                                                                          //LED Flash End

      else{                                                                                      //Normal LED Start
        for(uint8_t i = 0; i<led_on_count; i++){                  
          leds[i] = color;
        }
        for(uint8_t i = led_on_count; i<NUM_LEDS; i++){
          leds[i] = CRGB::Black;
        }
        FastLED.show();
      }                                                                                         //Normal LED End
    }   
  } 
}