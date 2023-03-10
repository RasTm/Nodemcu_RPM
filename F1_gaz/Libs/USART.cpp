#include "USART.hpp"

class USART_Base{
	public:
	USART_TypeDef *USARTx;

	float Usart_Div;
	uint16_t Usart_int;

	uint32_t Periph_clk;
	uint16_t Div_Mantissa;
	uint8_t Div_Fraction;

	USART_Base(USART_TypeDef *USARTxn, uint32_t target_baud = 9600){
		if(USARTxn == USART1){
			USARTx = ((USART_TypeDef*) USART1_BASE);
			RCC-> APB2ENR |= 0x00004000;
			Set_Gpio(GPIOA, 9, ALTER, PUSH, LOW, NOT);			//TX
			Set_Gpio(GPIOA, 10, INPUT, FLOATING, LOW, NOT);		//RX
			#undef Periph_clk
			#define Periph_clk 72000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART2){
			USARTx = ((USART_TypeDef*) USART2_BASE);
			RCC-> APB1ENR |= 0x00020000;
			Set_Gpio(GPIOA, 2, ALTER, PUSH, LOW, NOT);			//TX
			Set_Gpio(GPIOA, 3, INPUT, FLOATING, LOW, NOT);		//RX
			#undef Periph_clk
			#define Periph_clk 36000000							//Later (APB1 or APB2 clocks are writing)
		}
		else if(USARTxn == USART3){
			USARTx = ((USART_TypeDef*) USART3_BASE);
			RCC-> APB1ENR |= 0x00040000;
			Set_Gpio(GPIOB, 10, ALTER, PUSH, LOW, NOT);			//TX
			Set_Gpio(GPIOB, 11, INPUT, FLOATING, LOW, NOT);		//RX
			#undef Periph_clk
			#define Periph_clk 36000000							//Later (APB1 or APB2 clocks are writing)
		}

		Usart_Div = ((float)Periph_clk/(16*target_baud));		//
		Usart_int = Usart_Div;									//
		Div_Mantissa = Usart_int;								//
																//	Calculate USART Baud Timings
		Usart_Div = Usart_Div - (float)Usart_int;				//
		Div_Fraction = Usart_Div*16;							//

		USARTx-> BRR |= (Div_Mantissa<<4)+Div_Fraction;			//	Write to the USART_BAUDRATE Register
	}

	template <typename T> void USART_Transmit(std::vector<T> &data);
	void USART_Transmit(uint8_t *data, uint8_t size);

	void USART_Receive(std::vector<uint8_t> data);
	void USART_Receive(uint8_t *data, uint8_t size);
};

template <typename T> void USART_Base::USART_Transmit(std::vector<T> &data){

	uint8_t data_size = sizeof(data[0]);

	USARTx-> CR1  = 0;							//Reset USART CR1 Register
	USARTx-> CR1 |= 0x00002008;					//USART Enable TX Mode Set

	if(data_size == 1){
		for(T Usart_data : data){
			while(!(USARTx-> SR & 0x0000080));	//Check TXE bit
			USARTx-> DR = Usart_data;
		}
	}
	else if(data_size == 2){
		for(T Usart_data : data){
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x00FF);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0xFF00);
		}
	}
	else if(data_size == 4){
		for(T Usart_data : data){
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x000000FF);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x0000FF00);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0x00FF0000);
			while(!(USARTx-> SR & 0x00000080)); //Check TXE bit
			USARTx-> DR = (Usart_data & 0xFF000000);
		}
	}

	while(!(USARTx-> SR & 0x000000040));	//Wait Until Transmission Complete Bit Set
	USARTx-> CR1 &= 0xFFFFFFF7; 			//USART Disable TX Mode Clear
}

void USART_Base::USART_Transmit(uint8_t *data, uint8_t size){
	USARTx-> CR1  = 0;						//Reset USART CR1 Register
	USARTx-> CR1 |= 0x00002008;				//USART Enable TX Mode Set

	for(uint8_t i=0; i<size; i++){
		while(!(USARTx-> SR & 0x0000080));	//Check TXE bit
		USARTx-> DR = (uint8_t)data[i];
	}

	while(!(USARTx-> SR & 0x000000040));	//Wait Until Transmission Complete Bit Set
	USARTx-> CR1 &= 0xFFFFFFF7; 			//USART Disable TX Mode Clear

}

void USART_Base::USART_Receive(std::vector<uint8_t> data){
	USARTx-> CR1  = 0;						//Reset USART CR1 Register
	USARTx-> CR1 |= 0x00002004;				//USART Enable RX Mode Set

	while(!(USARTx-> SR & 0x00000020)){		//Check RXNE bit
		if(USARTx->DR == '\0' && USARTx-> DR & 0x00000020){
			break;
		}
		data.push_back(USARTx-> DR);
	}

	USARTx-> CR1 &= 0xFFFFFFFB;				//USART Disable RX Mode Clear
}

void USART_Base::USART_Receive(uint8_t *data, uint8_t size){
	USARTx-> CR1  = 0;						//Reset USART CR1 Register
	USARTx-> CR1 |= 0x00002004;				//USART Enable RX Mode Set

	for(uint8_t i=0; i<size; i++){
		while(!(USARTx-> SR & 0x00000020));	//Check RXNE bit
		data[i] = USARTx->DR;
	}

	USARTx-> CR1 &= 0xFFFFFFFB;				//USART Disable RX Mode Clear
}
