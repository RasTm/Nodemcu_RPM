#include "Timers.hpp"

uint32_t Systick_counter=0;

class Timer_Base{
	private:
	uint8_t MOE_Status;
	uint8_t Timer;
	public:
	TIM_TypeDef *Timerx;

	Timer_Base(uint8_t const TimerXn, uint32_t Prescaler_Value, uint32_t ARR_Value){
		Timer = TimerXn;
		switch (TimerXn){
		 case TIM_1:
			 Timerx = ((TIM_TypeDef *) TIM1_BASE);
			 RCC-> APB2ENR |= 0x00000800;
			 break;
		 case TIM_2:
			 Timerx = ((TIM_TypeDef *) TIM2_BASE);
			 RCC-> APB1ENR |= 0x00000001;
		 	 break;
		 case TIM_3:
			 Timerx = ((TIM_TypeDef *) TIM3_BASE);
			 RCC-> APB1ENR |= 0x00000002;
			 break;
		 case TIM_4:
			 Timerx = ((TIM_TypeDef *) TIM4_BASE);
			 RCC-> APB1ENR |= 0x00000004;
			 break;
		 case TIM_5:
			 Timerx = ((TIM_TypeDef *) TIM5_BASE);
			 RCC-> APB1ENR |= 0x00000008;
			 break;
		 case TIM_6:
			 Timerx = ((TIM_TypeDef *) TIM6_BASE);
			 RCC-> APB1ENR |= 0x00000010;
			 break;
		 case TIM_7:
			 Timerx = ((TIM_TypeDef *) TIM7_BASE);
			 RCC-> APB1ENR |= 0x00000020;
			 break;
		 case TIM_8:
			 Timerx = ((TIM_TypeDef *) TIM8_BASE);
			 RCC-> APB2ENR |= 0x00002000;
			 break;
		 case TIM_9:
			 Timerx = ((TIM_TypeDef *) TIM9_BASE);
			 RCC-> APB2ENR |= 0x00080000;
			 break;
		 case TIM_10:
			 Timerx = ((TIM_TypeDef *) TIM10_BASE);
			 RCC-> APB2ENR |= 0x00100000;
			 break;
		 case TIM_11:
			 Timerx = ((TIM_TypeDef *) TIM11_BASE);
			 RCC-> APB2ENR |= 0x00200000;
			 break;
		 case TIM_12:
			 Timerx = ((TIM_TypeDef *) TIM12_BASE);
			 RCC-> APB1ENR |= 0x00000040;
			 break;
		 case TIM_13:
			 Timerx = ((TIM_TypeDef *) TIM13_BASE);
			 RCC-> APB1ENR |= 0x00000080;
			 break;
		 case TIM_14:
			 Timerx = ((TIM_TypeDef *) TIM14_BASE);
			 RCC-> APB1ENR |= 0x00000100;
			 break;
		 default:
			 while(1);
		}

		Timerx-> ARR = ARR_Value;
		Timerx-> PSC = Prescaler_Value;
	}
	void Counter_Start();
	void PWM(uint8_t CH, uint8_t Pwm_mode);
	void Pulse_Generator(uint32_t ARR_Val);
};

void Timer_Base::Counter_Start(){
	Timerx-> CR1 |= 0x0001;
}

void Timer_Base::PWM(uint8_t CH, uint8_t Pwm_mode){

	if(Timer < 6 || Timer == 8){
		if(CH > 15){ while(1);}
	}
	else if(Timer == 6 || Timer == 7) while(1);
	else if(Timer == 9 || Timer == 12){
		if(CH > 2){  while(1);}
	}
	else if(Timer == 10 || Timer == 11 || Timer == 13 || Timer == 14){
		if(CH > 1){  while(1);}
	}

	std::bitset<4> channel (CH);

    for(uint8_t i=0;i<4;i++){
        if(channel[i] == 1 && i<2){
            Timerx-> CCMR1 |= ((((Pwm_mode)<<4)+(0x8))<<(8*i));
            Timerx-> CCER  |= (1<<(i*4));
        }
        else if(channel[i] == 1 && i>1){
        	Timerx-> CCMR2 |= ((((Pwm_mode)<<4)+(0x8))<<(8*(i-2)));
            Timerx-> CCER  |= (1<<(i*4));
        }
    }

    if(Timer == 8 || Timer == 1){
    	Timerx-> BDTR |= 0xA000;		//Main Output Enable (MOE) and Break Polarity Active High
    }

	Timerx -> CR1   |= 0x0080;			//Auto Reload Preload (ARPE) Enable

	Counter_Start();
}

void Timer_Base::Pulse_Generator(uint32_t ARR_Val){

}

void delay(int count){
	Systick_counter = count;
	while(Systick_counter != 0);
}

extern "C" {void SysTick_Handler(void){
	if(Systick_counter > 0){Systick_counter--;}
}}
