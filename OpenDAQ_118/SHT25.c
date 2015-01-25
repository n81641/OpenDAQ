///
/// \file SHT25.c
/// \brief Communication with Senserion SHT25 RH sensor 
/// \details I2c commands to read SHT25 sensor 
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
#include "SHT25.h"
#include <util/delay.h>
#include "uart.h"        
struct hum_dat ReadSHT25(void)
{
	struct hum_dat data;

		i2c_start_wait(0x80+I2C_WRITE);     // set device address and write mode
		i2c_write(0xE5);	//get rh
		_delay_ms(45);//delay for getting RH 12 bit
		i2c_start_wait(0x80+I2C_READ);   // set device address and read mode
		uint8_t ret1 = i2c_readAck();// data MSB
		uint8_t ret2 = i2c_readAck();// data LSB
		uint8_t ret3 = i2c_readNak();// checksum
		data.SHT_rh = ((ret1<<8) +ret2);
		i2c_stop(); // 
		
	
		ret1=ret2=ret3=0;
		i2c_start_wait(0x80+I2C_WRITE);     // set device address and write mode			
		i2c_write(0xE3);	// get temperature
		_delay_ms(95);//delay for getting T 14 bit
		i2c_start_wait(0x80+I2C_READ);   // set device address and read mode
		 ret1 = i2c_readAck();// data MSB
		 ret2 = i2c_readAck();// data LSB
		 ret3 = i2c_readNak();// checksum
		data.SHT_temp = ((ret1<<8) +ret2);
		i2c_stop(); //


		return data;
		 
}
