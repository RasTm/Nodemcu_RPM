#include "ADC.hpp"

class ADC_Base{
	public:
	ADC_TypeDef	*ADCx;
	ADC_Base(adc_num adc, GPIO_TypeDef *GPIO_port, uint8_t Pin){

		if(adc==ADC_1){
			ADCx = ((ADC_TypeDef*) ADC1_BASE);
			RCC-> APB2ENR |= 0x00000200;			//ADC 1 Clock Enable
		}
		else if(adc==ADC_2){
			ADCx = ((ADC_TypeDef*) ADC2_BASE);
			RCC-> APB2ENR |= 0x00000400;			//ADC 2 Clock Enable
		}
		else if(adc==ADC_3){
			if(((GPIO_port == GPIOA) || (GPIO_port == GPIOC)) && ((Pin == 0) || (Pin == 1) || (Pin == 2) || (Pin == 3))){
				ADCx = ((ADC_TypeDef*) ADC3_BASE);
				RCC-> APB2ENR |= 0x00008000;		//ADC 3 Clock Enable
			}
		}
		else if(!((GPIO_port == GPIOA) || (GPIO_port == GPIOB) || (GPIO_port == GPIOC)) && !((Pin == 0) || (Pin == 1) || (Pin == 2) || (Pin == 3) || (Pin == 4) || (Pin == 5) || (Pin == 6) || (Pin == 7))){
			while(1);
		}
		Set_Gpio(GPIO_port,Pin,INPUT,ANALOG);
		ADCx-> CR2 |= 0x00000001;					//ADC X Enable
	}

	 uint16_t ADC_ScanConv();
	 uint16_t ADC_ScanConv(std::vector<uint16_t> &data);
	 uint16_t ADC_ContinuousConv();
};

uint16_t ADC_Base::ADC_ScanConv(){
//	ADCx-> CR1 |= 0x00000100;		//Scan Mode Enable
	ADCx-> SQR2|= 0x00000000;
	ADCx-> SQR1|= 0x00100000;
	ADCx-> SQR3|= 0x00000001;
	ADCx-> CR2 |= 0x000E0000;
	ADCx-> CR2 |= 0x40000000;		//Regular conv start
	return ADCx-> DR;
}

uint16_t ADC_Base::ADC_ScanConv(std::vector<uint16_t> &data){
	ADCx-> CR1 |= 0x40000002;		//Regular conv start, Continuous mode enable

	return ADCx-> DR;
}

uint16_t ADC_Base::ADC_ContinuousConv(){

}
