///
/// \file sixteen_bit.c
/// \brief Communication with LTC2495 ADC
/// \details I2c commands to setup and read LTC2495 Delta Sigma ADC by Linear Technology
/// \author Karl Walter.  karl (at) waltech.com
/// \copyright Copyright (c) 2014 Karl Walter.  karl (at) waltech.com.
///This software is distributed under the MIT License. 
///Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
///associated documentation files (the "Software"), to deal in the Software without restriction, 
///including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
///and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
///subject to the following conditions:
///
///\copyright The above copyright notice and this permission notice shall be included in all copies or substantial 
///portions of the Software.
///The software is provided "as is", without warranty of any kind expressed or implied, 
///including but not limited to the warranties of merchantability, fitness for a particular 
///purpose and noninfringement.  In no event shall the authors or copyright holders be liable 
///for any claim, damages or other liability whether in an action of contract, tort or 
///otherwise, arising from, out of or in connection with the software or the use or other 
///dealings in the software.
///
#include "sixteen_bit.h"

volatile uint16_t ADC_volts; //16 bit i2c ADC result



/// \brief Reads the value from the I2c ADC
/// \param  3 channel numbers 
/// \return signed value or 65536 if overscale
int32_t read_adc(uint8_t ADC_A, uint8_t ADC_B, uint8_t ADC_C)
{
    int32_t ADC_signed = 0; 
    uint8_t sign = i2c_adc(ADC_A, ADC_B, ADC_C);//get value from ADC
    if (sign == 2){return 65536;}//overscale
    if (sign == 1){ADC_signed = (0-ADC_volts);}//negative result
    if (sign == 0){ADC_signed = ADC_volts;}//positive result
    return ADC_signed;
}


//read i2c ADC:
	//send i2c address, mux1, mux2 (see page 17 ltc2487 datasheet)
	//returns 0 for + value, 1 for -value, 2 for overvoltage
uint8_t i2c_adc(uint8_t address, uint8_t mux1, uint8_t mux2)
{
	//write mux and setup bits to ADC:
	i2c_start_wait(address+I2C_WRITE);     // set device address and write mode			
	i2c_write(mux1);	// Transmit first (of 2) input configure byte: 
	i2c_write(mux2);	//Transmit second (of 2) input configure byte
	i2c_stop();
	_delay_ms(80); // 80 ok for normal channels, not enough for temp	
	if (mux2 == 0xC0){_delay_ms(80);}//internal temp read needs more time
    //delay(55000); //time for conversion to happen after stop command.  tried 25000, too little 35000, OK
    //uart_puts_p(PSTR(" address sent "));
    i2c_start_wait(address+I2C_READ);   // set device address and read mode
    uint16_t ret1 = i2c_readAck();// for what is read from i2c
	uint16_t ret2 = i2c_readAck();// for i2c
	uint8_t ret3 = i2c_readNak();// for i2c
    i2c_stop();
					//bit checks here:
				if (bitisset(ret1,7) && bitisset(ret1,6) )
					{
						ADC_volts = 0xffff;
						return(2);//over fullscale +
					}
				if (!bitisset(ret1,7) && !bitisset(ret1,6) )
					{
						return(2);//over fullscale -
					}
				if (bitisset(ret1,7) && !bitisset(ret1,6) )
					{
						//bits 21...6 from 24 bit adc register, read into 3 registers in avr, ret1,2,3
						//which is bits 5,4,3,2,1,0 of ret1 (ret1<<10);//ret1 left shifted 10 sopts
						//	and			7.....0 of ret2 (ret2<<2);//ret2 left shifted 2 sopts
						//	and			7,6 of ret3 (ret3>>6);//ret3 right shifted 6 sopts
					ADC_volts = ((ret1<<10)+(ret2<<2)+(ret3>>6));
					return(0);//positive volatge +
					}
				if (!bitisset(ret1,7) && bitisset(ret1,6) )
					{
					ADC_volts = ~((ret1<<10)+(ret2<<2)+(ret3>>6));//flip all bits, pg15 ltc2487 datasheet
					if (ADC_volts != 0xFF) //don't add 1 if 16 bit reg is full
						{
						ADC_volts = ADC_volts + 1;//actual value off by 1, i.e, -1 will be 0, but w/ neg sign
						} 	
					return(1);//negative voltage -
					}	
		return(0);
}




