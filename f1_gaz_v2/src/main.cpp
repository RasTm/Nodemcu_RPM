#include "stm32f10x.h"

#include "../Libs/RCC.hpp"
#include "../Libs/ADC.hpp"
#include "../Libs/Timers.hpp"

void Gpio_Config(){
	Set_Gpio(GPIOA,8,ALTER,PUSH,MED,NOT);

	Set_Gpio(GPIOC,13,OUTPUT,PUSH,MED,NOT);
	Set_Gpio_Pin(GPIOC,13,0);
}

uint16_t scale(uint16_t data, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max){
	return (data-in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#define PRE_SC 360-1			//PRE_Scaler / 72 = x uS tick
#define AUTO_R 5000-1
#define OUT_MIN 0
#define OUT_MAX 4600

int main(void){

	uint16_t adc_data;

	Clock_init();
	Gpio_Config();

	ADC_Base pot(ADC_1,GPIOA,1);

	Timer_Base output(TIM_1,PRE_SC,AUTO_R);
	output.Timerx->CCR1 = 1;
	output.PWM(1,PWM_1);


//	lcd ekran(I2C1);
//	ekran.lcd_init();

	while (1){
		adc_data = pot.ADC_ScanConv();

		output.Timerx->ARR = AUTO_R - scale(adc_data, 0, 4095, OUT_MIN, OUT_MAX);

/*		char arr[4];
		arr[3] = ((adc_data) % 10) + 48;
		arr[2] = ((adc_data/10) % 10) + 48;
		arr[1] = ((adc_data/100) % 10) + 48;
		arr[0] = ((adc_data/1000) % 10) + 48;

		for(uint8_t i = 0; i<4; i++){
			ekran.lcd_put_cur(0,i);
			ekran.lcd_send_data(arr[i]);
		}
		ekran.lcd_send_string("    ");
		delay(50);
*/	}
}
