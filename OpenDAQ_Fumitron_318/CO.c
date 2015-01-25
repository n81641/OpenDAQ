/// \file CO.c
/// \brief Power-on setup of LMP91000 potenstat chip   
/// \details Setup via I2c, then analog output.
/// \author University of Illinois Department and Environmental and Civil Engineering
/// \copyright Copyright (c) 2014 University of Illinois Department and Environmental and Civil Engineering.
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
#include "CO.h"

void Init_CO(void)
{
		//reading the CO chip registers:
//		i2c_start_wait(0x90+I2C_WRITE);     // set device address and write mode
//		i2c_write(0x00);	//tell it what register is being read or written: 0x00(status), 0x01(lock), 0x10, 0x11, or  0x12 (See page 14 of pdf)
//		i2c_start_wait(0x90+I2C_READ);     // set device address and read mode
//		uint8_t ret1 = i2c_readAck();// data 
//		i2c_stop(); //
		
		i2c_start_wait(0x90+I2C_WRITE);     // set device address and write mode
		i2c_write(0x01);	//pointer to lock register, needs to be unlocked to write any other registers
		i2c_write(0x00); //write "unlock" 0x00
		i2c_stop(); //
		
		i2c_start_wait(0x90+I2C_WRITE);     // set device address and write mode
		i2c_write(0x10);	//pointer to TIACN register
		i2c_write(0x13); //write  (See page 14 of pdf)
		i2c_stop(); //
		
		i2c_start_wait(0x90+I2C_WRITE);     // set device address and write mode
		i2c_write(0x11);	//pointer to REFCN register
		i2c_write(0x00); //write (See page 14 of pdf)
		i2c_stop(); //
		
        i2c_start_wait(0x90+I2C_WRITE);     // set device address and write mode
		i2c_write(0x12);	//pointer to MODECN register
		i2c_write(0x03); //write (See page 14 of pdf)
		i2c_stop(); //
		 
}

void CO_to_temp(uint8_t yes)// switch controller to oytput temp.
{
	if (yes==1)//change to temperature
	{
		i2c_start_wait(0x90+I2C_WRITE);     // set device address and write mode
		i2c_write(0x12);	//pointer to MODECN register
		i2c_write(0x07); //write (See page 14 of pdf)
		i2c_stop(); //
	}
	else//change to potenstat
	{
		i2c_start_wait(0x90+I2C_WRITE);     // set device address and write mode
		i2c_write(0x12);	//pointer to MODECN register
		i2c_write(0x03); //write (See page 14 of pdf)
		i2c_stop(); //
	}	 
}
