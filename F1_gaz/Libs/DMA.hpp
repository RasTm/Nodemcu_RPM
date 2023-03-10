#include "stm32f10x.h"

//enum DMA_Select:			uint8_t {DMA_1,DMA_2};
enum DMA_Priority:			uint8_t {LOW_P,MED_P,HIGH_P,V_HIGH_P};
enum DMA_Transfer_Direction:uint8_t {P_M, 						//Peripheral to Memory
									 M_P,						//Memory to Peripheral
									 M_M};						//Memory to Memory

enum DMA_Periph_Datasize:	uint8_t {P8,						//8 Bit Peripheral Data Size
									 P16,						//16 Bit Peripheral Data Size
									 P32};						//32 Bit Peripheral Data Size

enum DMA_Mem_Datasize:		uint8_t {M8,						//8 Bit Memory Data Size
									 M16,						//16 Bit Memory Data Size
									 M32};						//32 Bit Memory Data Size

enum DMA_Circular:			uint8_t {Disable, Enable};

enum DMA_Interrupt_Types:	uint8_t {Not,
									 Transfer_Complete=2,		//Transfer Complete Interrupt Enable
									 Half_Trasnfer=4,			//Half Transfer Complete Interrupt Enable
									 Transfer_Error=8,			//Transfer Error Interrupt Enable
									 TC_HT=6,					//Transfer Complete and Half Transfer Interrupt Enable
									 TC_TE=10,					//Transfer Complete and Transfer Error Interrupt Enable
									 HT_TE=12,					//Half Transfer and Transfer Error Interrupt Enable
									 ALL=0xE};					//All DMA Interrupts Enable

enum DMA_Peripheral:		uint8_t {ADC_1,
									 SPI_1_RX,
									 SPI_1_TX,
									 SPI_2_RX,
									 SPI_2_TX,
									 USART_3_TX,
									 USART_3_RX,
									 USART_1_TX,
									 USART_1_RX,
									 USART_2_RX,
									 USART_2_TX,
									 I2C_2_TX,
									 I2C_2_RX,
									 I2C_1_TX,
									 I2C_1_RX};

class DMA_Config;
