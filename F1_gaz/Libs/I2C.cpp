#include "I2C.hpp"

class I2C_Base{
	private:
	uint8_t own_i2c_addr = 0;
	uint16_t trash;

	public:
	I2C_TypeDef *I2Cx;
	I2C_Base(I2C_TypeDef *I2Cxn, uint8_t i2c_mode = STANDART){
		if(I2Cxn == I2C1){
			RCC-> APB1ENR |= 0x00200000;
			I2Cx = ((I2C_TypeDef *) I2C1_BASE);
			Set_Gpio(GPIOB, 6, ALTER, DRAIN, MED, UP);
			Set_Gpio(GPIOB, 7, ALTER, DRAIN, MED, UP);
		}
		else if(I2Cxn == I2C2){
			RCC-> APB1ENR |= 0x00400000;
			I2Cx = ((I2C_TypeDef *) I2C2_BASE);
			Set_Gpio(GPIOB, 10, ALTER, DRAIN, MED, UP);
			Set_Gpio(GPIOB, 11, ALTER, DRAIN, MED, UP);
		}
		else while(1);

		I2Cx-> CR2 |= 36;			//Sonra d�zeltilecek APB1_CLK yaz�lacak
		I2Cx-> OAR1|= own_i2c_addr;

		switch(i2c_mode){
			case STANDART:
				I2Cx-> CCR   |= 0x001E;
				I2Cx-> TRISE |= 0x0022;
				break;
			case FAST:
				I2Cx-> CCR   |= 0x8000;
				break;
			default:
				while(1);
		}

	}
	void write_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data);
	void multi_byte_write(uint8_t dev_addr, uint8_t dev_reg_addr, std::vector<uint8_t> &data);

	void read_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data);
	void multi_byte_read(uint8_t dev_addr, uint8_t dev_reg_addr, std::vector<uint8_t> &data, uint8_t byte_count);

	void write_byte_small(uint8_t dev_addr, uint8_t data);
	void multi_byte_write_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count);

	void read_byte_small(uint8_t dev_addr, uint8_t *data);
	void multi_byte_read_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count);

	void multi_byte_read_extend(uint8_t dev_addr, uint16_t dev_reg_addr, uint8_t *data, uint8_t byte_count);
};

void I2C_Base::write_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> DR = data;				//Register Data Write to I2C1_DR Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> CR1 |= 0x0200; 			//Stop Generated And Peripheral Disabled
}

void I2C_Base::multi_byte_write(uint8_t dev_addr, uint8_t dev_reg_addr, std::vector<uint8_t> &data){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i:data){
		I2Cx-> DR = i;					//Register Data Write to I2C1_DR Register
		while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
		while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register
	}
	I2Cx-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

void I2C_Base::read_byte(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *data){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> CR1 |= 0x0100;			//ReStart Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register
	dev_addr |= 1;					//Mpu6050's address value OR operation for I2C Read Sequence
	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while (!(I2Cx-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	*data = I2Cx-> DR; 				//Takes Data From I2C1_DR Register

	I2Cx-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

void I2C_Base::multi_byte_read(uint8_t dev_addr, uint8_t dev_reg_addr, std::vector<uint8_t> &data, uint8_t byte_count){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	I2Cx-> CR1 |= 0x0400;			//Ancknowledge Sending Enable
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> DR = dev_reg_addr;		//Register Address Write to I2C1_DR Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> CR1 |= 0x0100;			//ReStart Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register
	dev_addr |= 1;					//Device address value OR operation for I2C Read Sequence
	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while (!(I2Cx-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i = 0; i < byte_count; i++){
		while(!(I2Cx-> SR1 & 0x0040)); //Check RXNE Flag From I2C1_SR1 Register

		if(i == (byte_count-2)){
			I2Cx-> CR1 &= 0xFBFF;		//Anknowledge Disabled
		}

		data.push_back(I2Cx-> DR); 		//Takes Data From I2C1_DR Register
		trash = I2Cx-> SR1;
		trash = I2Cx-> SR2;

		if(i == (byte_count-1)){
			I2Cx-> CR1 |= 0x0200;		//Stop Generated And Peripheral Disabled
		}
	}
}
/*-------------------------------------FOR SMALL WRITE OPERATION------------------------------------*/
/**************************************USE FOR REGISTERLESS SLAVES***********************************/

void I2C_Base::write_byte_small(uint8_t dev_addr, uint8_t data){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> DR = data;				//Register Data Write to I2C1_DR Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	I2Cx-> CR1 |= 0x0200; 			//Stop Generated And Peripheral Disabled
}

void I2C_Base::multi_byte_write_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i=0; i<byte_count; i++){
		I2Cx-> DR = data[i];			//Register Data Write to I2C1_DR Register
		while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
		while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register
	}
	I2Cx-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

/*-------------------------------------FOR SMALL READ OPERATION-------------------------------------*/
/**************************************USE FOR REGISTERLESS SLAVES***********************************/

void I2C_Base::read_byte_small(uint8_t dev_addr, uint8_t *data){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	dev_addr |= 1;					//Mpu6050's address value OR operation for I2C Read Sequence
	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while (!(I2Cx-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	*data = I2Cx-> DR; 				//Takes Data From I2C1_DR Register

	I2Cx-> CR1 |= 0x0200;			//Stop Generated And Peripheral Disabled
}

void I2C_Base::multi_byte_read_small(uint8_t dev_addr, uint8_t *data, uint8_t byte_count){

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	dev_addr |= 1;					//Device address value OR operation for I2C Read Sequence
	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	I2Cx-> CR1 |= 0x0400;			//Ancknowledge Sending Enable
	while (!(I2Cx-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i = 0; i < byte_count; i++){
		while(!(I2Cx-> SR1 & 0x0040)); //Check RXNE Flag From I2C1_SR1 Register

		if(i == (byte_count-2)){
			I2Cx-> CR1 &= 0xFBFF;		//Anknowledge Disabled
//			I2Cx-> CR1 |= 0x0800;
		}

		data[i] = I2Cx-> DR; 			//Takes Data From I2C1_DR Register
		trash = I2Cx-> SR1;
		trash = I2Cx-> SR2;

		if(i == (byte_count-1)){
			I2Cx-> CR1 |= 0x0200;		//Stop Generated And Peripheral Disabled
		}
	}
}

/*--------------------------------FOR LARGE ADDRESS WRITE OPERATION---------------------------------*/
/*---------------------------------USE FOR 2 BYTE ADDRESSED SLAVES----------------------------------*/

/*
 * This function for use like eeprom devices etc.
 */

void I2C_Base::multi_byte_read_extend(uint8_t dev_addr, uint16_t dev_reg_addr, uint8_t *data, uint8_t byte_count){

	uint8_t dev_reg_addr_divider[2];

	dev_reg_addr_divider[0] = ((dev_reg_addr &= 0x0FFF)>>8);
	dev_reg_addr_divider[1] =  (dev_reg_addr &= 0x00FF);

	I2Cx-> CR1 |= 0x0001;			//Peripheral Enable
	I2Cx-> CR1 |= 0x0100;			//Start Generation
	I2Cx-> CR1 |= 0x0400;			//Ancknowledge Sending Enable
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register

	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	while(!(I2Cx-> SR1 & 0x0080));	//Clearing Data Register Flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(short int i = 0; i<2; i++){

		I2Cx-> DR = dev_reg_addr_divider[i];	//Register Address Write to I2C1_DR Register
		while(!(I2Cx-> SR1 & 0x0080));			//Clearing Data Register Flag from I2C1_SR1 Register
		while(I2Cx-> SR1 & 0x0400);				//Check Anknowledge Failure From I2C1_SR1 Register
	}

	I2Cx-> CR1 |= 0x0100;			//ReStart Generation
	while(!(I2Cx-> SR1 & 0x0001));	//Clear Start Bit Flag from I2C1_SR1 Register
	dev_addr |= 1;					//Device address value OR operation for I2C Read Sequence
	I2Cx-> DR = dev_addr;			//Write to I2Cx Data Register
	while(!(I2Cx-> SR1 & 0x0002));	//Clearing Addr flag from I2C1_SR1 Register
	trash = I2Cx-> SR1;
	trash = I2Cx-> SR2;
	while (!(I2Cx-> SR1 & 0x0040)); //Check Byte Transfer Finish Flag From I2C1_SR1 Register
	while(I2Cx-> SR1 & 0x0400);		//Check Anknowledge Failure From I2C1_SR1 Register

	for(uint8_t i = 0; i < byte_count; i++){
		while(!(I2Cx-> SR1 & 0x0040));  //Check RXNE Flag From I2C1_SR1 Register

		if(i == (byte_count-2)){
			I2Cx-> CR1 &= 0xFBFF;		//Anknowledge Disabled
		}

		data[i] = I2Cx-> DR; 			//Takes Data From I2C1_DR Register
		trash = I2Cx-> SR1;
		trash = I2Cx-> SR2;

		if(i == (byte_count-1)){
			I2Cx-> CR1 |= 0x0200;		//Stop Generated And Peripheral Disabled
		}
	}
}
